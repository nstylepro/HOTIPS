#include "Orchestrator.h"
#include "DnsExfiltrationDetector.h"
#include "HttpClient.h"
#include "IPSweepDetector.h"
#include "TcpPortScanDetector.h"

void Orchestrator::event_orchestrator(std::vector<PNetworkEvent> network_events)
{
	auto tcp_port_scan_reports = TcpPortScanDetector::detect_port_scan(network_events);
	if (!tcp_port_scan_reports.empty())
	{
		// Do something
		// HttpClient::send_event(eventId, ip, datetime);
	}

	const auto ip_sweep_reports = IPSweepDetector::detect_ip_sweep(network_events);
	if (!ip_sweep_reports.empty())
	{
		// Do something
		// HttpClient::send_event(eventId, ip, datetime);
	}

	FILETIME time = get_file_time();
	// Check candidates for DNS exfiltration
	for (auto* packet : network_events)
	{
		if (DnsExfiltrationDetector::is_dns_packet(packet->DestPort) &&
			DnsExfiltrationDetector::detect_dns_exfiltration(packet->PacketSize)) // packet->Protocol == 2 ??
		{
			std::vector<uint16_t> src_ports = { 53 };
			std::vector<uint16_t> dns_ports = { 53 };
			report_event* report = new report_event(L"DNS Exfiltration", time, packet->SourceAddress, packet->DestAddress, src_ports, dns_ports);
			std::cout << report << std::endl;
			
			// do something
			//HttpClient::send_event(eventId, ip, datetime);
		}	
	}	
}

FILETIME& Orchestrator::get_file_time()
{
	// Get time
	FILETIME ft;
	SYSTEMTIME st;

	GetSystemTime(&st); // gets current time
	SystemTimeToFileTime(&st, &ft);
	return ft;
}
