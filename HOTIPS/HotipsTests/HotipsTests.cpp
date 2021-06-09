#include "pch.h"
#include "CppUnitTest.h"
#include "../LogAnalyzer/Orchestrator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HotipsTests
{
	TEST_CLASS(HotipsTests)
	{
	private:
		uint16_t m_dest_port = 80;
	public:
		
		PNetworkEvent gen_event()
		{
			PNetworkEvent event = nullptr;
			event->SourceAddress = L"";
			event->EventTime = Orchestrator::get_file_time();
			event->PID = 5896;
			event->PacketSize = 54;
			event->DestPort = m_dest_port;
			event->SourcePort = 45582;
			event->Protocol = 6;
			event->EventType = EVENT_TRACE_TYPE_SEND;
			return event;
		}
		
		TEST_METHOD(TestMethod1)
		{
			std::vector<PNetworkEvent> network_events = {};
			for (int i = 0; i < 150; i++)
			{
				network_events.push_back(gen_event());
				m_dest_port++;
			}
			Orchestrator::event_orchestrator(network_events);
		}
	};
}
