#pragma once

#include <set>
#include "report_event.h"

#include "../ETWCollector/EventParser.h"
#include "../ETWCollector/TraceManager.h"

class IPSweepDetector
{
	static const uint8_t m_ipSweepThreshold = 15;
	static const std::set<uint16_t> m_ports_for_sweep;
	static const std::wstring _alert_name;

public:
	static std::list<report_event> detect_ip_sweep(std::list<PNetworkEvent>& network_events);
	static std::set<std::wstring> convert_to_set(std::list<std::wstring> v);
};
