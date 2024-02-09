#include "../includes/headers.hpp"

SOCKET Request::createSocketConnection(std::string address)
{
	WSADATA wsaData = {0};
	DWORD toggle = 1;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET SOCKET_CREATED = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (SOCKET_CREATED == INVALID_SOCKET) exit(1);
	if (ioctlsocket(SOCKET_CREATED, FIONBIO, &toggle) == INVALID_SOCKET) exit(2);

	return SOCKET_CREATED;
}

bool Request::sendRequestPacket(SOCKET SOCKET_, std::string address, unsigned short port)
{
	std::string packet{ "SAMP" };

	struct sockaddr_in sock = {0};
	struct sockaddr_in from = {0};

	char buffer[2048] = {0};
	int fromlen = sizeof(from);
	from.sin_family = AF_INET;

	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);
	sock.sin_addr.s_addr = inet_addr(address.c_str());

	packet += Request::convertAddress(address);
	packet += port & 0xFFu;
	packet += port >> 8 & 0xFFu;
	packet += "i";

	sendto(SOCKET_, packet.c_str(), packet.size(), 0, (sockaddr*)&sock, sizeof(sockaddr_in));

	time_t start_time = time(0);

	while (time(0) - start_time < 0.5)
	{
		if (recvfrom(SOCKET_, buffer, sizeof(buffer), 0, (sockaddr*)&from, &fromlen) > 11)
            if(*(unsigned int *)&buffer == 'PMAS')
			    printf("\nValid: %d\n", *(unsigned short *)&buffer[8]);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return false;
}