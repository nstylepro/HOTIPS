#pragma once

#include <atlstr.h>
#include <iostream>
#include <minwindef.h>
#include <ostream>
#include <string>
#include <utility>
#include <list>
#include <xtree>

class report_event
{
	std::wstring _src_ip = L"";
	std::wstring _dst_ip = L"";
	std::list<uint16_t> _src_ports;
	std::list<uint16_t> _dst_ports;
	std::wstring _alert_name = L"";
	FILETIME _event_time;
	
public:
	report_event(const std::wstring& alert_name, const FILETIME& event_time, std::wstring src_ip, std::wstring dst_ip, std::list<uint16_t> src_ports, std::list<uint16_t> dst_ports)
	{
		_alert_name = alert_name;
		_event_time = event_time;
		_src_ip = std::move(src_ip);
		_dst_ip = std::move(dst_ip);
		_src_ports = std::move(src_ports);
		_dst_ports = std::move(dst_ports);
	}

	report_event(const wchar_t* alert_name, const FILETIME& ft, const std::wstring& src_ip, const std::wstring& dst_ip, const std::list<uint16_t> src_ports, const std::list<uint16_t> dst_ports)
	{
		_alert_name = alert_name;
		_event_time = ft;
		_src_ip = src_ip;
		_dst_ip = dst_ip;
		_src_ports = src_ports;
		_dst_ports = dst_ports;
	}

	report_event(std::wstring::const_pointer alert_name, const FILETIME& ft, const std::wstring& src_ip, std::wstring::const_pointer dst_ip, const std::list<unsigned short>& src_ports, const std::list<unsigned short>& dst_ports)
	{
		_alert_name = alert_name;
		_event_time = ft;
		_src_ip = src_ip;
		_dst_ip = dst_ip;
		_src_ports = src_ports;
		_dst_ports = dst_ports;
	}

	// std::ostream& operator<<(std::ostream& os, const report_event& report_event) const;
};
