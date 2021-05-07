#include "Orchestrator.h"
#include "DnsExfiltrationDetector.h"
#include "HttpClient.h"
#include "IPSweepDetector.h"
#include "TcpPortScanDetector.h"

void Orchestrator::EventOrchestrator(std::vector<PNetworkEvent> network_events)
{
	if (TcpPortScanDetector::detect_port_scan(network_events))
	{
		// Do something
		// HttpClient::send_event(eventId, ip, datetime);
	}

	if (IPSweepDetector::detect_ip_sweep(network_events))
	{
		// Do something
		// HttpClient::send_event(eventId, ip, datetime);
	}
	
	// Check candidates for DNS exfiltration
	for (auto* packet : network_events)
	{
		if (DnsExfiltrationDetector::is_dns_packet(packet->DestPort) &&
			DnsExfiltrationDetector::detect_dns_exfiltration(packet->PacketSize)) // packet->Protocol == 2 ??
		{
			// do something
			//HttpClient::send_event(eventId, ip, datetime);
		}	
	}	
}
