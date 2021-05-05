#pragma once

#include <memory>
#include <vector>

#include "../ETWCollector/EventParser.h"

class IPSweepDetector
{
private:
	uint8_t m_ipSweepTreshold = 15;

public:
	bool DetectIpSweep(std::vector<EventProperty>& ipPackets) const;
	static uint32_t CountDistinctIp(std::vector<std::wstring> arr, int n);
};