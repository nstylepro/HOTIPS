#pragma once

#define INITGUID

#include <Windows.h>
#include <cguid.h>
#include <atlbase.h>
#include <functional>
#include <tdh.h>
#include <memory>
#include <wil\resource.h>

#include "EventParser.h"

#pragma comment(lib, "tdh")

typedef struct NetworkEvent
{
	uint32_t EventType;
	uint32_t Protocol;
	FILETIME EventTime;
	uint32_t PID;
	uint32_t PacketSize;
	std::wstring DestAddress;
	std::wstring SourceAddress;
	uint32_t DestPort;
	uint32_t SourcePort;
} * PNetworkEvent;

class TraceManager final {
public:
	~TraceManager();

	bool Start(std::function<void(PNetworkEvent)> callback);
	bool Stop();

private:
	void OnEventRecord(PEVENT_RECORD rec);
	DWORD Run();

private:
	TRACEHANDLE _handle{ 0 };
	TRACEHANDLE _hTrace{ 0 };
	EVENT_TRACE_PROPERTIES* _properties;
	std::unique_ptr<BYTE[]> _propertiesBuffer;
	EVENT_TRACE_LOGFILE _traceLog = { 0 };
	wil::unique_handle _hProcessThread;
	std::function<void(PNetworkEvent)> _callback;
};