#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#define PORT 9009
class Inline_IPv4_Broadcast_UDP
{
public:
	Inline_IPv4_Broadcast_UDP() {
		m_error = WSAStartup(MAKEWORD(2, 2), &m_wsa);
		if (m_error != 0) {
			std::cout << "WSAStartup failed with ErrorCode:" << m_error << "\n";
		}

		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (const char*)&m_broadcast, sizeof(m_broadcast)) < 0) {
			std::cout << "Error setting Broadcast option\n";
			closesocket(m_socket);
		}

		m_send_addr.sin_family = AF_INET;
		m_send_addr.sin_port = htons(PORT);
		m_send_addr.sin_addr.s_addr = INADDR_BROADCAST;

		m_error = ioctlsocket(m_socket, FIONBIO, &iMode);
		if (m_error != NO_ERROR)
			printf("ioctlsocket failed with error: %ld\n", m_error);
	}

	~Inline_IPv4_Broadcast_UDP() {
		closesocket(m_socket);
		WSACleanup();
	}

	void Broadcast() {
		while (m_buffer_recv != m_buffer_send)
		{
			if (sendto(m_socket, (char*)&m_buffer_send, sizeof(m_buffer_send), 0, (sockaddr*)&m_send_addr, sizeof(struct sockaddr_in)) < 0) {
				std::cout << "Error in Sending." << WSAGetLastError();
				closesocket(m_socket);
			}
			recvfrom(m_socket, (char*)&m_buffer_recv, 50, 0, (sockaddr*)&m_recv_addr, &m_addr_size);
		}
	}

private:
	WSADATA m_wsa;
	SOCKET m_socket;
	u_long iMode = 1;
	int m_error = 0;

	struct sockaddr_in m_send_addr;
	struct sockaddr_in m_recv_addr;
	int m_addr_size = sizeof(struct sockaddr_in);

	char m_broadcast = '1';
	int m_buffer_recv = 0;
	int m_buffer_send = 0xB00B5;
};