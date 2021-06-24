#include <set>

#include "IPSweepDetector.h"
#include "Orchestrator.h"

#include "../ETWCollector/TraceManager.h"

const std::set<uint16_t> IPSweepDetector::m_ports_for_sweep { 21, 22, 23, 53, 137, 139, 445, 3389 };
const std::wstring IPSweepDetector::_alert_name = L"IP Sweep";

std::list<report_event> IPSweepDetector::detect_ip_sweep(std::list<PNetworkEvent>& network_events)
{
    std::wstring src_ip;
    std::list<uint16_t> ports;
    std::list<std::wstring> ip_addresses;
    std::list<report_event> reports;

    for (auto& ipPacket : network_events)
    {
        src_ip = ipPacket->SourceAddress;
        if (m_ports_for_sweep.find(ipPacket->DestPort) != m_ports_for_sweep.end())
        {
            ip_addresses.push_back(ipPacket->DestAddress);
        }
    }

    std::list<std::wstring> distinct_ips(ip_addresses);
    distinct_ips.unique();
    if (distinct_ips.size() >= m_ipSweepThreshold)
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

auto IPSweepDetector::convert_to_set(std::list<std::wstring> v) -> std::set<std::wstring>
{
    // Declaring the  set
    std::set<std::wstring> s;

    // Traverse the list
    for (std::wstring x : v) {

        // Insert each element
        // into the Set
        s.insert(x);
    }

    // Return the resultant Set
    return s;
}
