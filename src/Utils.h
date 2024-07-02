#pragma once

#include <string>
#include <vector>
#include <WS2tcpip.h>
#include <mutex>

class Utils {
public:
    static std::string getLocalIPAddress();
    static void broadcastMessage(const std::string& message, SOCKET excludeSock, std::vector<SOCKET>& clientSockets, std::mutex& clientMutex);
};


