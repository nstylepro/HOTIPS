#include <set>

#include "IPSweepDetector.h"
#include "Orchestrator.h"

#include "../ETWCollector/TraceManager.h"

const std::set<uint16_t> IPSweepDetector::m_ports_for_sweep { 21, 22, 23, 53, 137, 139, 445, 3389 };
const std::wstring IPSweepDetector::_alert_name = L"IP Sweep";

std::vector<report_event> IPSweepDetector::detect_ip_sweep(std::vector<PNetworkEvent>& network_events)
{
    std::wstring src_ip;
    std::vector<uint16_t> ports;
    std::vector<std::wstring> ip_addresses;
    std::vector<report_event> reports;
    ip_addresses.reserve(network_events.size());
	
    for (auto& ipPacket : network_events)
    {
        src_ip = ipPacket->SourceAddress;
	    if (m_ports_for_sweep.find(ipPacket->DestPort) != m_ports_for_sweep.end())
	    {
            ip_addresses.push_back(ipPacket->DestAddress);
	    }
	}

    const auto distinctIps = count_distinct_ip(ip_addresses, ip_addresses.size());
    if (distinctIps >= m_ipSweepThreshold)
    {
        auto set_ips = convert_to_set(ip_addresses);
        for (auto ip : set_ips)
        {
	        report_event report = report_event(_alert_name, Orchestrator::get_file_time(), src_ip, ip, ports, ports);
            reports.emplace_back(report);
        }
    	
        return reports;
    }

    return reports;
}

uint32_t IPSweepDetector::count_distinct_ip(std::vector<std::wstring> network_events, const int event_count)
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

auto IPSweepDetector::convert_to_set(std::vector<std::wstring> v) -> std::set<std::wstring>
{
    // Declaring the  set
    std::set<std::wstring> s;

    // Traverse the Vector
    for (std::wstring x : v) {

        // Insert each element
        // into the Set
        s.insert(x);
    }

    // Return the resultant Set
    return s;
}
