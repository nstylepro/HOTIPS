#pragma once
#include <list>

#include "../ETWCollector/TraceManager.h"

class Orchestrator
{
	// char ip[] = "192.168.203.56";
	//char datetime[] = "05/05/2021%2015:44";

public:
	static void event_orchestrator(std::list<PNetworkEvent> network_events);
	static FILETIME& get_file_time();
};

