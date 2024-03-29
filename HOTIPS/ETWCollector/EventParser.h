#pragma once

#include <Windows.h>
#include <tdh.h>
#include <memory>
#include <string>
#include <vector>

struct EventProperty {
	EventProperty(EVENT_PROPERTY_INFO& info);

	std::wstring Name;
	BYTE* Data;
	ULONG Length;
	EVENT_PROPERTY_INFO& Info;

	template<typename T>
	T GetValue() const 
	{
		static_assert(std::is_pod<T>() && !std::is_pointer<T>());
		return *(T*)Data;
	}

	PCWSTR GetUnicodeString() const;
	PCSTR GetAnsiString() const;
	std::wstring GetIpAddress() const;
};

class EventParser {
public:
	EventParser(PEVENT_RECORD record);

	PTRACE_EVENT_INFO GetEventInfo() const;
	PEVENT_RECORD GetEventRecord() const;
	const EVENT_HEADER& GetEventHeader() const;
	const std::vector<EventProperty>& GetProperties() const;
	const EventProperty* GetProperty(PCWSTR name) const;

	DWORD GetProcessId() const;

	static std::wstring GetDosNameFromNtName(PCWSTR name);

private:
	std::unique_ptr<BYTE[]> _buffer;
	PTRACE_EVENT_INFO _info{ nullptr };
	PEVENT_RECORD _record;
	mutable std::vector<EventProperty> _properties;
};