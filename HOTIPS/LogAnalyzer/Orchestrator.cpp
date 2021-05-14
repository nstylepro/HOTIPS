#include "Orchestrator.h"
#include "DnsExfiltrationDetector.h"
#include "HttpClient.h"
#include "IPSweepDetector.h"
#include "TcpPortScanDetector.h"
#include "Firewall_Rules.h"

void Orchestrator::EventOrchestrator(std::vector<PNetworkEvent> network_events)
{
	if (TcpPortScanDetector::detect_port_scan(network_events))
	{
		// Do something
		std::vector<PNetworkEvent>::iterator it = network_events.begin();
		block_ip((*it)->DestAddress);
		// HttpClient::send_event(eventId, ip, datetime);
	}

	if (IPSweepDetector::detect_ip_sweep(network_events))
	{
		// ICMP ping sweep	
		block_icmp();
		// HttpClient::send_event(eventId, ip, datetime);
	}
	
	// Check candidates for DNS exfiltration
	for (auto* packet : network_events)
	{
		
		if (DnsExfiltrationDetector::is_dns_packet(packet->DestPort) &&
			DnsExfiltrationDetector::detect_dns_exfiltration(packet->PacketSize)) // packet->Protocol == 2 ??
		{
			// do something
			block_udp_ip_port(53, packet->DestAddress);
			//HttpClient::send_event(eventId, ip, datetime);
		}	
	}	
}
