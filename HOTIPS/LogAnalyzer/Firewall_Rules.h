#ifndef FIREWALL_RULES
#define FIREWALL_RULES

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <comdef.h> 


using namespace std;

void block_ip(wstring ip)
{
	std::wstring command = L"advfirewall firewall add rule name=block_ip" + ip + L" dir=out remoteip=" + ip + L" action=block";
	_bstr_t b(command.c_str());
	const char* c = b;
	ShellExecuteA(NULL, "open", "netsh.exe", (LPCSTR)c, NULL, SW_HIDE);
}

void unblock_ip(wstring ip)
{
	std::wstring command = L"advfirewall firewall delete rule name=block_ip" + ip + L" dir=out remoteip=" + ip;
	_bstr_t b(command.c_str());
	const char* c = b;
	ShellExecuteA(NULL, "open", "netsh.exe", (LPCSTR)c, NULL, SW_HIDE);
}

void block_icmp()
{
	ShellExecuteA(NULL, "open", "netsh.exe", "advfirewall firewall add rule name=block_icmp protocol=icmpv4 dir=out action=block", NULL, SW_HIDE);
}

void unblock_icmp()
{
	ShellExecuteA(NULL, "open", "netsh.exe", "advfirewall firewall delete rule name=block_icmp protocol=icmpv4 dir=out", NULL, SW_HIDE);
}

void block_tcp_port(uint16_t port)
{
	wstring command = L"advfirewall firewall add rule name=block_tcp_port_" + to_wstring(port) + L" protocol=tcp dir=out remoteport=" + to_wstring(port) + L" action=block";
	_bstr_t b(command.c_str());
	const char* c = b;
	ShellExecuteA(NULL, "open", "netsh.exe", (LPCSTR)c, NULL, SW_HIDE);
}

void block_tcp_ip_port(uint16_t port, wstring ip)
{
	std::wstring command = L"advfirewall firewall add rule name=block_tcp_port_" + to_wstring(port) + L" protocol=tcp dir=out remoteport=" + to_wstring(port) + L" remoteip=" + ip + L" action=block";
	_bstr_t b(command.c_str());
	const char* c = b;
	ShellExecuteA(NULL, "open", "netsh.exe", (LPCSTR)c, NULL, SW_HIDE);
}

void block_udp_port(uint16_t port)
{
	std::wstring command = L"advfirewall firewall add rule name=block_udp_port_" + to_wstring(port) + L" protocol=udp dir=out remoteport=" + to_wstring(port) + L" action=block";
	_bstr_t b(command.c_str());
	const char* c = b;
	ShellExecuteA(NULL, "open", "netsh.exe", (LPCSTR)c, NULL, SW_HIDE);
}

void block_udp_ip_port(uint16_t port, wstring ip)
{
	wstring command = L"advfirewall firewall add rule name=block_udp_port_" + to_wstring(port) + L" protocol=udp dir=out remoteport=" + to_wstring(port) + L" remoteip=" + ip + L" action=block";
	_bstr_t b(command.c_str());
	const char* c = b;
	HINSTANCE s = ShellExecuteA(NULL, "open", "netsh.exe", (LPCSTR)c, NULL, SW_HIDE);
}

void unblock_udp(uint16_t port, wstring ip)
{
	wstring command;
	if (!ip.empty())
		command = L"advfirewall firewall delete rule name=block_udp_port_" + to_wstring(port) + L" protocol=udp dir=out remoteport=" + to_wstring(port) + L" remoteip=" + ip;
	else
		command = L"advfirewall firewall delete rule name=block_udp_port_" + to_wstring(port) + L" protocol=udp dir=out remoteport=" + to_wstring(port);
	_bstr_t b(command.c_str());
	const char* c = b;
	ShellExecuteA(NULL, "open", "netsh.exe", (LPCSTR)c, NULL, SW_HIDE);
}

void unblock_tcp(uint16_t port, wstring ip)
{
	wstring command;
	if (!ip.empty())
		command = L"advfirewall firewall delete rule name=block_tcp_port_" + to_wstring(port) + L" protocol=tcp dir=out remoteport=" + to_wstring(port) + L" remoteip=" + ip;
	else
		command = L"advfirewall firewall delete rule name=block_tcp_port_" + to_wstring(port) + L" protocol=tcp dir=out remoteport=" + to_wstring(port);
	_bstr_t b(command.c_str());
	const char* c = b;
	ShellExecuteA(NULL, "open", "netsh.exe", (LPCSTR)c, NULL, SW_HIDE);
}

#endif // FIREWALL_RULES
