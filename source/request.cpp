#include "../includes/headers.hpp"

std::string Request::convertAddress(std::string address)
{
	std::string result, part{};
	std::stringstream it(address);

	while (std::getline(it, part, '.'))
		result += atoi(part.c_str());

	return result;
}

SOCKET Request::createSocketConnection(std::string address)
{
	WSADATA wsaData{};
	DWORD toggle = 1;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET SOCKET_CREATED = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (SOCKET_CREATED == INVALID_SOCKET)
		exit(1);

	if (ioctlsocket(SOCKET_CREATED, FIONBIO, &toggle) == INVALID_SOCKET)
		exit(2);

	return SOCKET_CREATED;
}

bool Request::sendRequestPacket(SOCKET SOCKET_, std::string address, unsigned short port)
{
	std::string packet{ "SAMP" };

	struct sockaddr_in sock {};
	struct sockaddr_in from {};

	char buffer[2048]{};
	int fromlen = sizeof(from);
	from.sin_family = AF_INET;

	memset((void*)&sock, '\0', sizeof(sockaddr_in));

	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);
	sock.sin_addr.s_addr = inet_addr(address.c_str());

	packet += convertAddress(address);
	packet += port & 0xFFu;
	packet += port >> 8 & 0xFFu;
	packet += "i";

	sendto(SOCKET_, packet.c_str(), packet.size(), 0, (sockaddr*)&sock, sizeof(sockaddr_in));

	time_t start_time = time(0);
	while (time(0) - start_time < 0.5)
	{
		if (recvfrom(SOCKET_, buffer, sizeof(buffer), 0, (sockaddr*)&from, &fromlen) > 11)
        {
            if(buffer[0] == 'S' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'P')
            {
                unsigned int intermediateByte = ((unsigned char)buffer[8] ^ 0xFF);
                unsigned int secondIntermediateByte = ((unsigned char)buffer[9] << 8 ^ 0xFF);

			    printf("\nValid: %d\n", (secondIntermediateByte - intermediateByte));
            }
        }

		Sleep(1);
	}

	return false;
}