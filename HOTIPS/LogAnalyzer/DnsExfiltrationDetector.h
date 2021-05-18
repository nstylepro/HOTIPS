#pragma once

#include <memory>

class DnsExfiltrationDetector
{
	static const uint32_t m_packetSizeThreshold = 255;
	static const uint16_t m_dnsPort = 53;

public:	
	static bool is_dns_packet(uint16_t d_port);
	static bool detect_dns_exfiltration(uint32_t packet_size);
};

