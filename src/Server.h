#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class Server {
public:
    Server();
    ~Server();
    void start(int port);

private:
    std::mutex clientMutex;
    std::map<SOCKET, std::string> clientNames;
    std::vector<SOCKET> clientSockets;

    void handleConnection(SOCKET sock);
};