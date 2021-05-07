#include "TcpPortScanDetector.h"
#include "../ETWCollector/TraceManager.h"

bool TcpPortScanDetector::detect_port_scan(const std::vector<PNetworkEvent>& network_events)
{
	std::vector<uint16_t> ports;
	for (const auto& element : network_events)
	{
		ports.emplace_back(element->DestPort);
	}

	if (ports.size() >= c_port_count_threshold)
	{
		return true;
	}

	return false;
}
