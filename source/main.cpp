#include "../includes/headers.hpp"

std::string address = {0};

int main()
{  
    printf("Write IP: ");
    getline(std::cin, address);

    SOCKET SOCK = Request::createSocketConnection(address);

    for (unsigned short port = 1000; port < 9999; port++)
    {
        std::thread(Request::sendRequestPacket, SOCK, address, port).detach();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    printf("\nAll ports checked! (During: %.2f secs)\n", (long double)(clock() / 1000));
    closesocket(SOCK);
    WSACleanup();

    return system("pause");
}