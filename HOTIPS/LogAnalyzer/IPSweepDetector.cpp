#include "IPSweepDetector.h"

#include <iostream>

#include "../ETWCollector/TraceManager.h"

bool IPSweepDetector::detect_ip_sweep(std::vector<PNetworkEvent>& network_events)
{
    std::vector<std::wstring> ip_addresses;
    ip_addresses.reserve(network_events.size());
    for (auto& ipPacket : network_events)
    {
        ip_addresses.emplace_back(ipPacket->DestAddress);
	}

    const auto distinctIps = count_distinct_ip(ip_addresses, ip_addresses.size());
    if (distinctIps >= m_ipSweepTreshold)
    {
        return true;
    }

    return false;
}

uint32_t IPSweepDetector::count_distinct_ip(std::vector<std::wstring> network_events, int event_count)
{
    uint32_t distinctIpAddresses = 0;
	
    // Pick all elements one by one
    for (auto i = 0; i < event_count; i++)
    {
        // Check if the picked element is already counted
        int j;
        for (j = 0; j < i; j++)
            if (network_events[i] == network_events[j])
                break;

        if (i == j)
            distinctIpAddresses++;
    }

    return distinctIpAddresses;
}