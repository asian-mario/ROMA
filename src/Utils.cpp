#include "Utils.h"
#include <iostream>
#include <vector>
#include <WS2tcpip.h>
#include <mutex>

std::string Utils::getLocalIPAddress() {
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    struct addrinfo hints = {}, * info;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    std::string ipAddress;
    if (getaddrinfo(hostname, nullptr, &hints, &info) == 0) {
        for (auto p = info; p != nullptr; p = p->ai_next) {
            sockaddr_in* ipv4 = (sockaddr_in*)p->ai_addr;
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &ipv4->sin_addr, ip, sizeof(ip));
            ipAddress = ip;
            break;
        }
        freeaddrinfo(info);
    }
    return ipAddress;
}

void Utils::broadcastMessage(const std::string& message, SOCKET excludeSock, std::vector<SOCKET>& clientSockets, std::mutex& clientMutex) {
    std::lock_guard<std::mutex> lock(clientMutex);
    for (SOCKET sock : clientSockets) {
        if (sock != excludeSock) {
            send(sock, message.c_str(), message.length(), 0);
        }
    }
}
