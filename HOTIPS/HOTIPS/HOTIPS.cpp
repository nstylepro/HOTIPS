#include "../ETWCollector/TraceManager.h"

#include <Windows.h>
#include <atltime.h>
#include <atlstr.h>
#include <chrono>
#include <ctime>  
#include <iostream>  

#include "Timer.h"
#include "../LogAnalyzer/Orchestrator.h"

using namespace std::chrono;

TraceManager* g_pMgr;
HANDLE g_hEvent;
char buffer[256];
std::vector<PNetworkEvent> m_events = {};
Timer<milliseconds, steady_clock> m_current_window_start;

void OnEventCallback(PNetworkEvent event) {

	std::wstring protocol(L"TCP"); // Currently default

	std::wstring eventType;
	switch (event->EventType) {
	case EVENT_TRACE_TYPE_ACCEPT:					eventType = L"Accept event"; break;
	case EVENT_TRACE_TYPE_CONNECT:					eventType = L"Connect event"; break;
	case EVENT_TRACE_TYPE_SEND:						eventType = L"Send event"; break;
	case EVENT_TRACE_TYPE_RECONNECT:				eventType = L"Reconnect event"; break;
	default:										return;
	}
	
	//std::wcout << (PCWSTR)CTime(event->EventTime).Format(L"%c") \
	//	<< ", " << eventType \
	//	<< ", Protocol: " << protocol \
	//	<< ", PID: " << event->PID \
	//	<< ", Source Address: " << event->SourceAddress \
	//	<< ", Destination Address: " << event->DestAddress \
	//	<< ", Source Port: " << event->SourcePort \
	//	<< ", Destination Port: " << event->DestPort \
	//	<< ", Packet Size: " << event->PacketSize \
	//	<< std::endl;

	// Initiate 60s time frame
	if (!m_current_window_start.is_set())
	{
		m_current_window_start.tick();
	}

	// Check elapsed time
	m_current_window_start.tock();
	if (m_current_window_start.duration().count() >= 60000)
 	{
		// Pass events to detections
		Orchestrator::event_orchestrator(m_events);
		m_events.clear();
		m_current_window_start.tick();
	}
	
	if (m_events.size() <= m_events.max_size())
	{
		m_events.push_back(event);
	}
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