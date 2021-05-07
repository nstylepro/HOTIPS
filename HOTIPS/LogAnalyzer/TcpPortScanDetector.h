#pragma once
#include <cstdint>

#include "../ETWCollector/EventParser.h"
#include "../ETWCollector/TraceManager.h"

class TcpPortScanDetector
{
	static const uint32_t c_port_count_threshold = 32;
public:
	static bool detect_port_scan(const std::vector<PNetworkEvent>& network_events);
};
