#pragma once

#include <set>
#include <vector>
#include "report_event.h"

#include "../ETWCollector/EventParser.h"
#include "../ETWCollector/TraceManager.h"
#include <set>
#include <vector>


class IOCDetector
{
	static const std::wstring _alert_name;
	static const std::set<std::wstring> m_ioc_ips;

public:
	static std::vector<report_event> detect_ioc_ip(std::vector<PNetworkEvent>& network_events);
};