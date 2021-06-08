#pragma once
#include "HttpClient.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstdio>

void HttpClient::send_event(int eventId, char* ip, char* datetime)
{
	WSAData wsa_data{};
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		return;

	struct addrinfo* s_result, s_hints{};
	RtlZeroMemory(&s_hints, sizeof(s_hints));

	s_hints.ai_socktype = SOCK_STREAM;
	s_hints.ai_protocol = IPPROTO_TCP;
	s_hints.ai_family = AF_INET;

	auto i_result = getaddrinfo("localhost", "4444", &s_hints, &s_result);
	if (i_result != 0)
	{
		WSACleanup();
		return;
	}

	auto s_socket = socket(s_result->ai_family, s_result->ai_socktype, s_result->ai_protocol);
	if (s_socket != INVALID_SOCKET)
	{
		i_result = connect(s_socket, s_result->ai_addr, static_cast<INT>(s_result->ai_addrlen));
		if (i_result == SOCKET_ERROR)
		{
			closesocket(s_socket);
			s_socket = 0;
		}
	}

	CHAR s_header_tpl[] =
		"POST %s HTTP/1.0\r\n"
		"Host: %s\r\n"
		"Content-Type: application/octet-stream\r\n"
		"Content-Encoding: binary\r\n"
		"Content-Length: %i\r\n"
		"Connection: close\r\n"
		"\r\n";

	const auto s_post = "event=" + std::to_string(eventId) + "," + std::string(ip) + "," + std::string(datetime);

	CHAR s_header[sizeof(s_header_tpl) + 100];
	CHAR s_recv[1000];

	if (s_socket != 0)
	{
		RtlZeroMemory(s_header, sizeof(s_header));
		RtlZeroMemory(s_recv, sizeof(s_recv));

		const auto i_data_len = lstrlenA(s_post.c_str());
		const auto i_header_len = wsprintfA(s_header, s_header_tpl, "/removed...", "requestb.in", i_data_len);

		if (i_header_len > 0)
		{
			if (send(s_socket, static_cast<PCHAR>(s_header), i_header_len, 0) != SOCKET_ERROR) // Header
			{
				if (send(s_socket, const_cast<PCHAR>(s_post.c_str()), i_data_len, 0) != SOCKET_ERROR) // Data
				{
					recv(s_socket, static_cast<PCHAR>(s_recv), sizeof(s_recv), 0);
					printf(s_recv);

				}
			}
		}
		closesocket(s_socket);
	}

	freeaddrinfo(s_result);
	WSACleanup();
}

