#include "TcpPortScanDetector.h"

bool TcpPortScanDetector::DetectPortScan(std::vector<EventProperty>& tcpPackets) const
{
	std::vector<uint16_t> ports;
	ports.reserve(tcpPackets.size());
	for (auto& element : tcpPackets)
	{
		ports.emplace_back(element);
	}

	if (ports.size() >= c_port_count_threshold)
	{
		return true;
	}

	return false;
}