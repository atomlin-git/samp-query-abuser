#pragma once

class Request
{
	public:
		static SOCKET createSocketConnection(std::string address);
		static bool   sendRequestPacket(SOCKET SOCKET_, std::string address, unsigned short port);
		static std::string convertAddress(std::string ip);
};