#pragma once
#include <cstdint>
#include <list>

#include "report_event.h"
#include "../ETWCollector/EventParser.h"
#include "../ETWCollector/TraceManager.h"

class TcpPortScanDetector
{
	static const uint32_t c_port_count_threshold = 32;
	static const std::wstring c_alert_name;

public:
	static std::list<report_event> detect_port_scan(std::list<PNetworkEvent>& network_events);
};
