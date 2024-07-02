#include "Client.h"
#include <iostream>
#include <thread>

void Client::start(const std::string& ipAddress, int port, const std::string& username) {
    WSADATA wsData;
    WSAStartup(MAKEWORD(2, 2), &wsData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    if (connect(sock, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    send(sock, username.c_str(), username.size() + 1, 0);

    std::thread t([sock]() {
        char buffer[1024];
        while (true) {
            ZeroMemory(buffer, sizeof(buffer));
            int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
            if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
                std::cout << "Disconnected from server." << std::endl;
                break;
            }
            std::cout << std::string(buffer, 0, bytesReceived) << std::endl;
        }
        });
    t.detach();

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            send(sock, input.c_str(), input.size() + 1, 0);
        }
    }

    closesocket(sock);
    WSACleanup();
}
