#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <WS2tcpip.h>
#include<winsock2.h>
#include <iostream>
#include <string>
#define PORT 9009

class Inline_tcp_client {
public:
	Inline_tcp_client(std::string ipAddress){
		
		int m_error = WSAStartup(MAKEWORD(2, 2), &m_wsa);
		if (m_error != 0){
			std::cerr << " Can't start Winsock, Err # " << m_error << std::endl;
			return;
		}

		m_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket == INVALID_SOCKET){
			std::cerr << " Can't create socket, Err # " << WSAGetLastError() << std::endl;
			WSACleanup();
			return;
		}

		m_send_addr.sin_family = AF_INET;
		m_send_addr.sin_port = htons(PORT);
		inet_pton(AF_INET, ipAddress.c_str(), &m_send_addr.sin_addr);


		m_error = connect(m_socket, (sockaddr*)&m_send_addr, sizeof(m_send_addr));
		if (m_error == SOCKET_ERROR){
			std::cerr << " Can't connect to server, Err # " << WSAGetLastError() << std::endl;
			closesocket(m_socket);
			WSACleanup();
			return;
		}
	}
	~Inline_tcp_client(){
		closesocket(m_socket);
		WSACleanup();
	}
	void communicate() {
		do
		{
			// Prompt the user for some text
			std::cout << "> ";
			getline(std::cin, userInput);

			if (userInput.size() > 0)		// Make sure the user has typed in something
			{
				// Send the text
				int sendResult = send(m_socket, userInput.c_str(), userInput.size() + 1, 0);
				if (sendResult != SOCKET_ERROR)
				{
					// Wait for response
					ZeroMemory(m_buffer_recv, 4096);
					int m_bytes_received = recv(m_socket, m_buffer_recv, 4096, 0);
					if (m_bytes_received > 0)
					{
						// Echo response to console
						std::cout << "SERVER> " << std::string(m_buffer_recv, 0, m_bytes_received) << std::endl;
					}
				}
			}

		} while (userInput.size() > 0);
	}
private:
	WSAData m_wsa;
	int m_error;
	SOCKET m_socket;
	struct sockaddr_in m_send_addr;
	char m_buffer_recv[4096];
	std::string userInput;
	int m_bytes_received;
};