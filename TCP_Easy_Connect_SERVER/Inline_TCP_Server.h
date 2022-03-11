#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <WS2tcpip.h>
#include <iostream>
#define PORT 9009

class Inline_tcp_server
{
public:
	Inline_tcp_server() {
		m_error = WSAStartup(MAKEWORD(2, 2), &m_wsa);
		if (m_error != 0)
		{
			std::cerr << " Can't start Winsock, Err # " << m_error << std::endl;
			return;
		}
		m_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket == INVALID_SOCKET)
		{
			std::cerr << " Can't create socket, Err # " << WSAGetLastError() << std::endl;
			WSACleanup();
			return;
		}
		m_recv_addr.sin_family = AF_INET;
		m_recv_addr.sin_port = htons(PORT);
		m_recv_addr.sin_addr.S_un.S_addr = INADDR_ANY;
		bind(m_socket, (sockaddr*)&m_recv_addr, sizeof(m_recv_addr));
		listen(m_socket, SOMAXCONN);
		m_socket_client = accept(m_socket, (sockaddr*)&m_send_addr, &m_addr_size);
		ZeroMemory(m_host, NI_MAXHOST);
		ZeroMemory(m_service, NI_MAXSERV);
		if (getnameinfo((sockaddr*)&m_send_addr, m_addr_size, m_host, NI_MAXHOST, m_service, NI_MAXSERV, 0) == 0) {
			std::cout << m_host << " connected on port " << m_service << std::endl;
		}
		else
		{
			inet_ntop(AF_INET, &m_send_addr.sin_addr, m_host, NI_MAXHOST);
			std::cout << m_host << " connected on port " << ntohs(m_send_addr.sin_port) << std::endl;
		}
		closesocket(m_socket);
	}
	~Inline_tcp_server() {
		closesocket(m_socket_client);
		WSACleanup();
	}
	void communicate() {
		while (true)
		{
			ZeroMemory(m_buffer_recv, 4096);
			ZeroMemory(m_buffer_send, 4096);
			int m_bytes_received = recv(m_socket_client, m_buffer_recv, 4096, 0);
			if (m_bytes_received == SOCKET_ERROR) {
				std::cerr << "Error in recv(). Quitting" << std::endl;
				break;
			}
			if (m_bytes_received == 0) {
				break;
			}
			memcpy(m_buffer_send, m_buffer_recv, 4096);
			send(m_socket_client, m_buffer_send, m_bytes_received + 1, 0);
		}
	}

private:
	WSADATA m_wsa;
	int m_error;
	SOCKET m_socket;
	SOCKET m_socket_client;
	sockaddr_in m_recv_addr;
	sockaddr_in m_send_addr;
	int m_addr_size = sizeof(struct sockaddr_in);
	char m_host[NI_MAXHOST];
	char m_service[NI_MAXSERV];
	char m_buffer_recv[4096];
	char m_buffer_send[4096];
	int m_bytes_received;
};