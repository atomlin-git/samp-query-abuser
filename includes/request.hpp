#pragma once

class Request
{
	public:
		static SOCKET createSocketConnection(std::string address);
		static bool   sendRequestPacket(SOCKET SOCKET_, std::string address, unsigned short port);

	private:
		static std::string convertAddress(std::string address)
		{
			std::string result, part{};
			std::stringstream it(address);

			while (std::getline(it, part, '.'))
				result += atoi(part.c_str());

			return result;
		}
};