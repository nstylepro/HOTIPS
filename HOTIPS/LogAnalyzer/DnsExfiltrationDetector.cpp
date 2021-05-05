#include "DnsExfiltrationDetector.h"

bool DnsExfiltrationDetector::IsDnsPacket(uint16_t dPort) const
{
	return dPort == m_dnsPort;
}

bool DnsExfiltrationDetector::DetectDnsExfiltration(uint32_t packetSize) const
{
	if (packetSize > m_packetSizeThreshold)
	{
		return true;
	}

	return false;
}
