#include"Inline_IPv4_Broadcast_UDP.h"
#include"Inline_TCP_Server.h"
int main() {
	Inline_IPv4_Broadcast_UDP sockaddr;
	sockaddr.Broadcast();
	Inline_tcp_server init;
	init.communicate();
	return 0;
}