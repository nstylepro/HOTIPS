#pragma once

#include <memory>
#include <vector>

#include "../ETWCollector/EventParser.h"
#include "../ETWCollector/TraceManager.h"

class IPSweepDetector
{
	static const uint8_t m_ipSweepTreshold = 15;

public:
	static bool detect_ip_sweep(std::vector<PNetworkEvent>& network_events);
	static uint32_t count_distinct_ip(std::vector<std::wstring> network_events, int event_count);
};
