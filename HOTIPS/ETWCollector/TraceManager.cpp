#include "TraceManager.h"

TraceManager::~TraceManager() {
	Stop();
}

bool TraceManager::Start(std::function<void(PNetworkEvent)> cb) {
	if (_handle || _hTrace)
		return true;

	_callback = cb;

	auto size = sizeof(EVENT_TRACE_PROPERTIES) + sizeof(KERNEL_LOGGER_NAME);
	_propertiesBuffer = std::make_unique<BYTE[]>(size);
	::memset(_propertiesBuffer.get(), 0, size);

	_properties = reinterpret_cast<EVENT_TRACE_PROPERTIES*>(_propertiesBuffer.get());
	_properties->EnableFlags = EVENT_TRACE_FLAG_NETWORK_TCPIP;
	_properties->Wnode.BufferSize = (ULONG)size;
	_properties->Wnode.Guid = SystemTraceControlGuid;
	_properties->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
	_properties->Wnode.ClientContext = 1;
	_properties->LogFileMode = EVENT_TRACE_REAL_TIME_MODE;
	_properties->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
	
	WCHAR loggerName[] = KERNEL_LOGGER_NAME;

	// Flush Events
	auto error = ControlTrace(NULL, loggerName, _properties, EVENT_TRACE_CONTROL_STOP);
	if (error != ERROR_SUCCESS && error != ERROR_MORE_DATA && error != ERROR_WMI_INSTANCE_NOT_FOUND)
	{
		throw;
	}



	error = ::StartTrace(&_handle, KERNEL_LOGGER_NAME, _properties);
	if (error != ERROR_SUCCESS && error != ERROR_ALREADY_EXISTS)
		return false;

	
	_traceLog.Context = this;
	_traceLog.LoggerName = loggerName;
	_traceLog.ProcessTraceMode = PROCESS_TRACE_MODE_EVENT_RECORD | PROCESS_TRACE_MODE_REAL_TIME;
	_traceLog.EventRecordCallback = [](PEVENT_RECORD record) {
		((TraceManager*)record->UserContext)->OnEventRecord(record);
	};
	_hTrace = ::OpenTrace(&_traceLog);
	if (!_hTrace)
		return false;
	
	// create a dedicated thread to process the trace
	_hProcessThread.reset(::CreateThread(nullptr, 0, [](auto param) {
		return ((TraceManager*)param)->Run();
		}, this, 0, nullptr));

	// Flush Events
	error = ControlTrace(_hTrace, loggerName, _properties, EVENT_TRACE_CONTROL_FLUSH);
	if (ERROR_SUCCESS != error)
	{
		throw;
	}

	return true;
}

bool TraceManager::Stop() {
	if (_hTrace) {
		::CloseTrace(_hTrace);
		_hTrace = 0;
	}
	if (_handle) {
		::StopTrace(_handle, KERNEL_LOGGER_NAME, _properties);
		_handle = 0;
	}
	return true;
}

void TraceManager::OnEventRecord(PEVENT_RECORD rec) {

	EventParser parser(rec);
	NetworkEvent parsedEvent = {};

	parsedEvent.EventTime = *(FILETIME*)&parser.GetEventHeader().TimeStamp.QuadPart;
	parsedEvent.EventType = parser.GetEventHeader().EventDescriptor.Opcode;

	if (parser.GetEventHeader().EventDescriptor.Task)
		parsedEvent.Protocol = (KERNEL_NETWORK_TASK_TCPIP == parser.GetEventHeader().EventDescriptor.Task) ? IPPROTO_TCP : IPPROTO_UDP;
	else
		parsedEvent.Protocol = 0;
	
 	for (auto& prop : parser.GetProperties()) {
		if (prop.Name == L"saddr")
			parsedEvent.SourceAddress = prop.GetIpAddress().c_str();
		else if (prop.Name == L"daddr")
			parsedEvent.DestAddress = prop.GetIpAddress().c_str();
		else if (prop.Name == L"sport") 
			parsedEvent.SourcePort = _byteswap_ushort(prop.GetValue<uint16_t>());			
		else if (prop.Name == L"dport")
			parsedEvent.DestPort = _byteswap_ushort(prop.GetValue<uint16_t>());
		else if (prop.Name == L"PID")
			parsedEvent.PID = prop.GetValue<uint32_t>();
		else if (prop.Name == L"size")
			parsedEvent.PacketSize = prop.GetValue<uint32_t>();
 	}

	if (_callback)
		_callback(&parsedEvent);
}

DWORD TraceManager::Run() {
	auto error = ::ProcessTrace(&_hTrace, 1, nullptr, nullptr);
	return error;
}