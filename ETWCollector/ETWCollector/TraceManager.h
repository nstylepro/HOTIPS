#pragma once

#define INITGUID

#include <Windows.h>
#include <functional>
#include <tdh.h>
#include <memory>
#include <wil\resource.h>

DEFINE_GUID( /* 9a280ac0-c8e0-11d1-84e2-00c04fb998a2 */
	TcpIpGuid,
	0x9a280ac0,
	0xc8e0,
	0x11d1,
	0x84, 0xe2, 0x00, 0xc0, 0x4f, 0xb9, 0x98, 0xa2
);

DEFINE_GUID( /* bf3a50c5-a9c9-4988-a005-2df0b7c80f80 */
	UdpIpGuid,
	0xbf3a50c5,
	0xa9c9,
	0x4988,
	0xa0, 0x05, 0x2d, 0xf0, 0xb7, 0xc8, 0x0f, 0x80
);

class TraceManager final {
public:
	~TraceManager();

	bool Start(std::function<void(PEVENT_RECORD)> callback);
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
	std::function<void(PEVENT_RECORD)> _callback;
};

