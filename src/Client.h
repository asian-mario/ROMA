#pragma once
#include <string>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class Client {
public:
    void start(const std::string& ipAddress, int port, const std::string& username);
};