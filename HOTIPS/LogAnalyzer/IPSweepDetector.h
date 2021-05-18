#pragma once

#include <memory>
#include <set>
#include <vector>
#include "report_event.h"

#include "../ETWCollector/EventParser.h"
#include "../ETWCollector/TraceManager.h"

class IPSweepDetector
{
	static const uint8_t m_ipSweepThreshold = 15;
	static const std::set<uint16_t> m_ports_for_sweep;
	static const std::wstring _alert_name;

public:
	static std::vector<report_event> detect_ip_sweep(std::vector<PNetworkEvent>& network_events);
	static uint32_t count_distinct_ip(std::vector<std::wstring> network_events, int event_count);
	static std::set<std::wstring> convert_to_set(std::vector<std::wstring> v);
};
