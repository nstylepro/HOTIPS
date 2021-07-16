#define _CRT_SECURE_NO_WARNINGS

#include "Orchestrator.h"
#include "DnsExfiltrationDetector.h"
#include "HttpClient.h"
#include "IPSweepDetector.h"
#include "TcpPortScanDetector.h"
#include "Firewall_Rules.h"

#include <ctime>
#include <comdef.h> 

#include "IOCDetector.h"

void Orchestrator::event_orchestrator(std::list<PNetworkEvent> network_events)
{
	int eventId = 9999;

	time_t now;
	time(&now);
	char datetime[sizeof "2011-10-08T07:07:09Z"];
	strftime(datetime, sizeof datetime, "%FT%TZ", gmtime(&now));

	const auto tcp_port_scan_reports = TcpPortScanDetector::detect_port_scan(network_events);
	if (!tcp_port_scan_reports.empty())
	{
		eventId = 1231;
		// Do something
		const std::list<PNetworkEvent>::iterator it = network_events.begin();
		block_ip((*it)->DestAddress);

		// Convert wchar to char
		const _bstr_t b_ip((*it)->DestAddress.c_str());
		HttpClient::send_event(eventId, b_ip, datetime, "TCP Scan Detected");
	}

	const auto ip_sweep_reports = IPSweepDetector::detect_ip_sweep(network_events);
	if (!ip_sweep_reports.empty())
	{
		eventId = 1232;
		// ICMP ping sweep
		block_icmp();
		const std::list<PNetworkEvent>::iterator it = network_events.begin();
		const _bstr_t b_ip((*it)->DestAddress.c_str());
		HttpClient::send_event(eventId, b_ip, datetime, "IP Sweep Detected");
	}
	
	const auto ip_ioc_reports = IOCDetector::detect_ioc_ip(network_events);
	if (!ip_ioc_reports.empty())
	{
		eventId = 1234;
		// ioc detector
		const std::list<PNetworkEvent>::iterator it = network_events.begin();
		block_ip((*it)->DestAddress);
		const _bstr_t b_ip((*it)->DestAddress.c_str());
		HttpClient::send_event(eventId, b_ip, datetime, "Malicious IOC Detected");
	}

	FILETIME time = get_file_time();
	
	// Check candidates for DNS exfiltration
	for (auto* packet : network_events)
	{
		
		if (DnsExfiltrationDetector::is_dns_packet(packet->DestPort) &&
			DnsExfiltrationDetector::detect_dns_exfiltration(packet->PacketSize)) // packet->Protocol == 2 ??
		{
			eventId = 1233;
			std::list<uint16_t> src_ports = { 53 };
			std::list<uint16_t> dns_ports = { 53 };
			report_event* report = new report_event(L"DNS Exfiltration", time, packet->SourceAddress, packet->DestAddress, src_ports, dns_ports);
			std::cout << report << std::endl;
			
			block_udp_ip_port(53, packet->DestAddress);
			const _bstr_t b_ip(packet->DestAddress.c_str());
			HttpClient::send_event(eventId, b_ip, datetime, "DNS Exfiltration");
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
