#include "TcpPortScanDetector.h"


#include <map>
#include <unordered_map>

#include "../ETWCollector/TraceManager.h"

const std::wstring TcpPortScanDetector::c_alert_name = L"TcpPortScan";


std::vector<report_event> TcpPortScanDetector::detect_port_scan(std::vector<PNetworkEvent>& network_events)
{
	// Get time
	FILETIME ft;
	SYSTEMTIME st;

	GetSystemTime(&st); // gets current time
	SystemTimeToFileTime(&st, &ft);

	// Makes a vector of SourceAddress, DestAddress, DestPort
	std::vector<std::tuple<std::wstring, std::wstring, uint16_t>> ports_tuple;
	std::vector<report_event> reports;
	
	ports_tuple.reserve(network_events.size());
	reports.reserve(network_events.size());
	for (const auto& element : network_events)
	{
		ports_tuple.emplace_back(std::make_tuple(element->SourceAddress, element->DestAddress, element->DestPort));
	}

	// Key: DestAddress, Value: <Key : Port, Value: Count>
	std::map<std::wstring, std::vector<uint16_t>> addr_port_pair_count_map;
	for (std::tuple<std::wstring, std::wstring, uint16_t> packet : ports_tuple)
	{
		std::wstring src_ip = std::get<0>(packet);
		std::wstring dst_ip = std::get<1>(packet);
		uint16_t dst_port = std::get<2>(packet);

		auto dst_addr_key = addr_port_pair_count_map.find(dst_ip);
		if (dst_addr_key != addr_port_pair_count_map.end())
		{

			dst_addr_key->second.push_back(dst_port);
		}
		else
		{
			std::vector<uint16_t> ports = { dst_port };
			addr_port_pair_count_map.insert(make_pair(dst_ip, ports));
		}
	}


	for (auto addr_port_count : addr_port_pair_count_map)
	{
		if (addr_port_count.second.size() >= c_port_count_threshold)
		{
			std::vector<uint16_t> src_ports_arr;
			auto report = report_event(c_alert_name, ft, network_events.front()->SourceAddress, addr_port_count.first.c_str(), src_ports_arr, addr_port_count.second);
			reports.push_back(report);
		}
	}

	return reports;
}
