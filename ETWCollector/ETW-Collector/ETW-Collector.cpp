#include <Windows.h>
#include <atltime.h>
#include <atlstr.h>
#include <iostream>
#include <fstream>
#include "EventParser.h"
#include "TraceManager.h"

#pragma comment(lib, "tdh")

TraceManager* g_pMgr;
HANDLE g_hEvent;
std::ofstream csvfile;
char buffer[256];

void OnEvent(PEVENT_RECORD rec) {
	EventParser parser(rec);
	
	auto ts = parser.GetEventHeader().TimeStamp.QuadPart;
	printf("Time: %ws ", (PCWSTR)CTime(*(FILETIME*)&ts).Format(L"%c"));
	snprintf(buffer, sizeof(buffer), "%ws", (PCWSTR)CTime(*(FILETIME*)&ts).Format(L"%c"));
	csvfile << buffer;

	switch (parser.GetEventHeader().EventDescriptor.Opcode) {
	case EVENT_TRACE_TYPE_ACCEPT:					printf("Accept event"); break;
	case EVENT_TRACE_TYPE_CONNECT:					printf("Connect event"); break;
	case EVENT_TRACE_TYPE_DISCONNECT:				printf("Disconnect event"); break;
	case EVENT_TRACE_TYPE_RECEIVE:					printf("Receive event"); break;
	case EVENT_TRACE_TYPE_RECONNECT:				printf("Reconnect event"); break;
	case EVENT_TRACE_TYPE_RETRANSMIT:				printf("Retransmit event"); break;
	case EVENT_TRACE_TYPE_SEND:						printf("Send event"); break;
	}

	auto prop2 = parser.GetProperty(L"saddr");
	if (prop2) {
		printf(" %ws", prop2->GetIpAddress().c_str());
		snprintf(buffer, sizeof(buffer), "%ws", prop2->GetIpAddress().c_str());
		csvfile << "," << buffer;
	}
	else
		csvfile << ",";

	auto prop = parser.GetProperty(L"daddr");
	if (prop) {
		printf(" %ws", prop->GetIpAddress().c_str());
		snprintf(buffer, sizeof(buffer), "%ws", prop->GetIpAddress().c_str());
		csvfile << "," << buffer;
	}
	else
		csvfile << ",";
	auto prop3 = parser.GetProperty(L"PID");
	if (prop3) {
		printf(" %d", prop3->GetValue<DWORD>());
		snprintf(buffer, sizeof(buffer), "%d", prop3->GetValue<DWORD>());
		csvfile << "," << buffer;
	}
	else
		csvfile << ",";
	csvfile << "\n";
	printf("\n");

}

int main() {
	TraceManager mgr;
	
	if (!mgr.Start(OnEvent)) {
		printf("Failed to start trace. Are you running elevated?\n");
		return 1;
	}

	g_pMgr = &mgr;
	csvfile.open("log.csv");
	csvfile << "Time,Source Address, Destination Addresss, Process ID \n";

	g_hEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

	::SetConsoleCtrlHandler([](auto type) {
		if (type == CTRL_C_EVENT) {
			g_pMgr->Stop();
			::SetEvent(g_hEvent);
			return TRUE;
		}
		return FALSE;
		}, TRUE);

	::WaitForSingleObject(g_hEvent, INFINITE);
	::CloseHandle(g_hEvent);
	csvfile.close();

	return 0;
}