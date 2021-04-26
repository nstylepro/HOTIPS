#include "IPSweepDetector.h"

#include <iostream>

bool IPSweepDetector::DetectIpSweep(std::vector<EventProperty>& ipPackets) const
{
    std::vector<std::wstring> ip_addresses;
    ip_addresses.reserve(ipPackets.size());
    for (auto& ipPacket : ipPackets)
    {
        ip_addresses.emplace_back(ipPacket.GetIpAddress());
	}

    const auto distinctIps = CountDistinctIp(ip_addresses, ip_addresses.size());
    if (distinctIps >= m_ipSweepTreshold)
    {
        return true;
    }

    return false;
}

uint32_t IPSweepDetector::CountDistinctIp(std::vector<std::wstring> arr, int n)
{
    uint32_t distinctIpAddresses = 0;
	
    // Pick all elements one by one
    for (int i = 0; i < n; i++)
    {
        // Check if the picked element is already counted
        int j;
        for (j = 0; j < i; j++)
            if (arr[i] == arr[j])
                break;

        if (i == j)
            distinctIpAddresses++;
    }

    return distinctIpAddresses;
}