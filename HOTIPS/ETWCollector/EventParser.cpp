#include "EventParser.h"


EventParser::EventParser(PEVENT_RECORD record) : _record(record) {
	ULONG size = 0;
	auto error = ::TdhGetEventInformation(record, 0, nullptr, _info, &size);
	if (error == ERROR_INSUFFICIENT_BUFFER) {
		_buffer = std::make_unique<BYTE[]>(size);
		_info = reinterpret_cast<PTRACE_EVENT_INFO>(_buffer.get());
		error = ::TdhGetEventInformation(record, 0, nullptr, _info, &size);
	}
	::SetLastError(error);
}

PTRACE_EVENT_INFO EventParser::GetEventInfo() const {
	return _info;
}

PEVENT_RECORD EventParser::GetEventRecord() const {
	return _record;
}

const EVENT_HEADER& EventParser::GetEventHeader() const {
	return _record->EventHeader;
}

const std::vector<EventProperty>& EventParser::GetProperties() const {
	if (!_properties.empty())
		return _properties;

	_properties.reserve(_info->TopLevelPropertyCount);
	auto userDataLength = _record->UserDataLength;
	BYTE* data = (BYTE*)_record->UserData;

	for (ULONG i = 0; i < _info->TopLevelPropertyCount; i++) {
		auto& prop = _info->EventPropertyInfoArray[i];
		EventProperty property(prop);
		property.Name.assign((WCHAR*)((BYTE*)_info + prop.NameOffset));
		auto len = prop.length;
		property.Length = len;
		property.Data = data;
		data += len;
		userDataLength -= len;

		_properties.push_back(std::move(property));
	}

	return _properties;
}

const EventProperty* EventParser::GetProperty(PCWSTR name) const {
	for (auto& prop : GetProperties())
		if (prop.Name == name)
			return &prop;
	return nullptr;
}

DWORD EventParser::GetProcessId() const {
	return _record->EventHeader.ProcessId;
}

std::wstring EventParser::GetDosNameFromNtName(PCWSTR name) {
	static std::vector<std::pair<std::wstring, std::wstring>> deviceNames;
	static bool first = true;
	if (first) {
		auto drives = ::GetLogicalDrives();
		int drive = 0;
		while (drives) {
			if (drives & 1) {
				// drive exists
				WCHAR driveName[] = L"X:";
				driveName[0] = (WCHAR)(drive + 'A');
				WCHAR path[MAX_PATH];
				if (::QueryDosDevice(driveName, path, MAX_PATH)) {
					deviceNames.push_back({ path, driveName });
				}
			}
			drive++;
			drives >>= 1;
		}
		first = false;
	}

	for (auto& [ntName, dosName] : deviceNames) {
		if (::_wcsnicmp(name, ntName.c_str(), ntName.size()) == 0)
			return dosName + (name + ntName.size());
	}
	return L"";
}

EventProperty::EventProperty(EVENT_PROPERTY_INFO& info) : Info(info) {}

PCWSTR EventProperty::GetUnicodeString() const {
	return (PCWSTR)Data;
}

PCSTR EventProperty::GetAnsiString() const {
	return (PCSTR)Data;
}

std::wstring EventProperty::GetIpAddress() const {
	uint32_t ipAddress = *(uint32_t*)Data;
	const int NBYTES = 4;
	uint8_t octet[NBYTES];
	WCHAR ipAddressFinal[16];
	for (int i = 0; i < NBYTES; i++)
	{
		octet[i] = ipAddress >> (i * 8);
	}
	swprintf(ipAddressFinal, 16, L"%d.%d.%d.%d", octet[0], octet[1], octet[2], octet[3]);
	return ipAddressFinal;
}