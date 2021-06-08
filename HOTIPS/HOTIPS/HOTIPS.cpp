#include "../ETWCollector/TraceManager.h"

#include <Windows.h>
#include <atltime.h>
#include <atlstr.h>
#include <iostream>

TraceManager* g_pMgr;
HANDLE g_hEvent;
char buffer[256];

void OnEventCallback(PNetworkEvent event) {

	std::wstring protocol(L"TCP"); // Currently default

	std::wstring eventType;
	switch (event->EventType) {
	case EVENT_TRACE_TYPE_ACCEPT:					eventType = L"Accept event"; break;
	case EVENT_TRACE_TYPE_CONNECT:					eventType = L"Connect event"; break;
	case EVENT_TRACE_TYPE_DISCONNECT:				return; // eventType = L"Disconnect event"; break;
	case EVENT_TRACE_TYPE_RECEIVE:					eventType = L"Receive event"; break;
	case EVENT_TRACE_TYPE_RECONNECT:				eventType = L"Reconnect event"; break;
	case EVENT_TRACE_TYPE_RETRANSMIT:				eventType = L"Retransmit event"; break;
	case EVENT_TRACE_TYPE_SEND:						eventType = L"Send event"; break;
	default:										return;
	}

	if (event->DestPort == 443 || event->DestPort == 5353 || event->DestPort == 80)
		return;

	if (event->SourcePort == 443 || event->SourcePort == 5353 || event->SourcePort == 80)
		return;

	if (event->DestAddress == L"8.8.8.8")
		std::wcout << L"STOP";

	if (event->DestAddress == L"1.1.1.1")
		std::wcout << L"STOP";

	if (event->DestPort == 55555)
		std::wcout << L"STOP";

	if (event->DestPort == 12345)
		std::wcout << L"STOP";

	std::wcout << (PCWSTR)CTime(event->EventTime).Format(L"%c") \
		<< ", " << eventType \
		<< ", Protocol: " << protocol \
		<< ", PID: " << event->PID \
		<< ", Source Address: " << event->SourceAddress \
		<< ", Destination Address: " << event->DestAddress \
		<< ", Source Port: " << event->SourcePort \
		<< ", Destination Port: " << event->DestPort \
		<< ", Packet Size: " << event->PacketSize \
		<< std::endl;		
}

int main() {
	TraceManager mgr;

	if (!mgr.Start(OnEventCallback)) {
		printf("Failed to start trace. Are you running elevated?\n");
		return 1;
	}

	g_pMgr = &mgr;
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

	return 0;
}