#pragma once
#include <cstdint>

#include "../ETW-Collector/EventParser.h"

class TcpPortScanDetector
{
	const uint8_t c_port_count_threshold = 50;

public:
	bool DetectPortScan(std::vector<EventProperty>& tcpPackets) const;
};

