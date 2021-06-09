#include "pch.h"

#include <cassert>

#include "CppUnitTest.h"
#include "../LogAnalyzer/IPSweepDetector.h"
#include "../LogAnalyzer/Orchestrator.h"
#include "../LogAnalyzer/TcpPortScanDetector.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HotipsTests
{
	TEST_CLASS(HotipsTests)
	{
	private:
		uint16_t m_dest_port = 80;
	public:
		
		PNetworkEvent gen_tcp_port_scan_event()
		{
			NetworkEvent parsedEvent = {};
			parsedEvent.SourceAddress = L"192.168.55.1";
			parsedEvent.DestAddress = L"192.168.1.1";
			parsedEvent.EventTime = Orchestrator::get_file_time();
			parsedEvent.PID = 5896;
			parsedEvent.PacketSize = 54;
			parsedEvent.DestPort = m_dest_port;
			parsedEvent.SourcePort = 45582;
			parsedEvent.Protocol = 6;
			parsedEvent.EventType = EVENT_TRACE_TYPE_SEND;
			return &parsedEvent;
		}
		
		std::vector<PNetworkEvent> gen_ip_sweep_event()
		{
			auto ips = { L"192.168.55.1",
			L"192.168.55.2",
			L"192.168.55.3",
			L"192.168.55.4",
			L"192.168.55.5",
			L"192.168.55.6",
			L"192.168.55.7",
			L"192.168.55.8",
			L"192.168.55.9",
			L"192.168.55.10",
			L"192.168.55.11",
			L"192.168.55.12",
			L"192.168.55.13",
			L"192.168.55.14",
			L"192.168.55.15" };
			std::vector<PNetworkEvent> events = {};

			for (std::wstring ip : ips)
			{
				NetworkEvent parsedEvent = {};
				parsedEvent.SourceAddress = L"192.168.55.1";
				parsedEvent.DestAddress = L"192.168.1.1";
				parsedEvent.EventTime = Orchestrator::get_file_time();
				parsedEvent.PID = 5896;
				parsedEvent.PacketSize = 54;
				parsedEvent.DestPort = 22;
				parsedEvent.SourcePort = 45582;
				parsedEvent.Protocol = 6;
				parsedEvent.EventType = EVENT_TRACE_TYPE_SEND;
				events.emplace_back(&parsedEvent);
			}
			
			return events;
		}

		TEST_METHOD(TestMethod1)
		{
			std::vector<PNetworkEvent> network_events = {};
			for (int i = 0; i < 150; i++)
			{
				network_events.push_back(gen_tcp_port_scan_event());
				m_dest_port++;
			}

			try {
				const auto report = TcpPortScanDetector::detect_port_scan(network_events);
				std::cout <<  "Reports Count"<< report.size() << std::endl;
				//assert(report.size() == 1);
			}
			catch (char* s) {
				std::cout << "No Report" << std::endl;
			}
		}

		TEST_METHOD(TestMethod2)
		{
			std::vector<PNetworkEvent> network_events = {};
			for (int i = 0; i < 150; i++)
			{
				network_events.emplace_back(gen_tcp_port_scan_event());
			}

			try {
				const auto report = IPSweepDetector::detect_ip_sweep(network_events);
				std::cout << "Reports Count" << report.size() << std::endl;
			}
			catch (char* s) {
				std::cout << "No Report" << std::endl;
			}
		}
	};
}
