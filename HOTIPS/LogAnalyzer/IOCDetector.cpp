#include "IOCDetector.h"

#include "Orchestrator.h"

const std::set<std::wstring> IOCDetector::m_ioc_ips{ L"130.211.015.150" };
const std::wstring IOCDetector::_alert_name = L"IOC IP";


std::list<report_event> IOCDetector::detect_ioc_ip(std::list<PNetworkEvent>& network_events)
{
	std::list<report_event> reports;

	for (auto& packet : network_events)
	{
		if (m_ioc_ips.find(packet->DestAddress) != m_ioc_ips.end())
		{
			std::list<uint16_t> src_ports;
			src_ports.emplace_back(packet->SourcePort);
			std::list<uint16_t> dst_ports;
			dst_ports.emplace_back(packet->SourcePort);

			report_event report = report_event(_alert_name, Orchestrator::get_file_time(), packet->SourceAddress, packet->DestAddress, src_ports, dst_ports);
			reports.emplace_back(report);

		}
	}

	return reports;
}