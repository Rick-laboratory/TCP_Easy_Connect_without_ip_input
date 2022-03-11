#pragma once
#pragma once
#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include"winsock2.h"
#include <ws2tcpip.h>
#include<iostream>

#define PORT 9009
class Inline_IPv4_Receiver
{
public:
	Inline_IPv4_Receiver() {
		m_error = WSAStartup(MAKEWORD(2, 2), &m_wsa);
		if (m_error != 0) {
			std::cout << "WSAStartup failed with ErrorCode:" << m_error << "\n";
		}

		m_socket = socket(AF_INET, SOCK_DGRAM, 0);

		if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (const char*)&m_broadcast, sizeof(m_broadcast)) < 0) {
			std::cout << "Error in setting Broadcast option";
			closesocket(m_socket);
		}

		m_recv_addr.sin_family = AF_INET;
		m_recv_addr.sin_port = htons(PORT);
		m_recv_addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(m_socket, (sockaddr*)&m_recv_addr, sizeof(m_recv_addr)) < 0) {
			std::cout << "Error in BINDING" << WSAGetLastError();
			closesocket(m_socket);
		}
	}

	~Inline_IPv4_Receiver() {
		closesocket(m_socket);
		WSACleanup();
	}

	std::string Receive() {
		while (m_buffer_recv != 0xB00B5)
		{
			recvfrom(m_socket, (char*)&m_buffer_recv, sizeof(m_buffer_recv), 0, (sockaddr*)&m_sender_addr, &m_addr_size);
			m_buffer_send = m_buffer_recv;
			if (sendto(m_socket, (char*)&m_buffer_send, sizeof(m_buffer_send), 0, (sockaddr*)&m_sender_addr, sizeof(struct sockaddr_in)) < 0) {
				std::cout << "Error in Sending. " << WSAGetLastError();
				closesocket(m_socket);
			}
		}

		Sleep(100);
		
		std::string addr = inet_ntoa(m_sender_addr.sin_addr);
		return addr;

	}

private:
	WSADATA m_wsa;
	SOCKET m_socket;
	int m_error = 0;

	struct sockaddr_in m_recv_addr;
	struct sockaddr_in m_sender_addr;
	int m_addr_size = sizeof(struct sockaddr_in);

	char m_broadcast = '1';
	int m_buffer_recv = 0;
	int m_buffer_send = 0;
};