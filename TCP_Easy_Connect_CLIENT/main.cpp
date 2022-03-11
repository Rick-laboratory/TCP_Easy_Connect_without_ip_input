#include"Inline_IPv4_Receiver.h"
#include"Inline_TCP_Client.h"
void main()
{
	Inline_IPv4_Receiver sockaddr;
	Inline_tcp_client init(sockaddr.Receive());
	init.communicate();
}
