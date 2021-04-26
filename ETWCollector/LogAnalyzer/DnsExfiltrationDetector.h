#pragma once

#include <Windows.h>
#include <tdh.h>
#include <memory>

class DnsExfiltrationDetector
{
	uint32_t m_packetSizeThreshold = 0;
	uint16_t m_dnsPort = 53;

public:
	bool IsDnsPacket(uint16_t dPort) const;
	bool DetectDnsExfiltration(uint32_t packetSize) const;
};

