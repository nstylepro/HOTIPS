#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define HTTPCLIENT
#pragma comment (lib, "Ws2_32.lib")

class HttpClient
{
public:
	static void send_event(int eventId, char* ip, char* datetime);
};

