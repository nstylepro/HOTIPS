#include "DnsExfiltrationDetector.h"

bool DnsExfiltrationDetector::is_dns_packet(const uint16_t d_port)
{
	return d_port == m_dnsPort;
}

bool DnsExfiltrationDetector::detect_dns_exfiltration(const uint32_t packet_size)
{
	if (packet_size > m_packetSizeThreshold)
	{
		return true;
	}

	return false;
}
