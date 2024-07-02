#include "Server.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <thread>

Server::Server() {
    WSADATA wsData;
    WSAStartup(MAKEWORD(2, 2), &wsData);
}

Server::~Server() {
    WSACleanup();
}

void Server::start(int port) {
    std::string serverIP = Utils::getLocalIPAddress();
    std::cout << "Starting server on " << serverIP << ":" << port << std::endl;

    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));
    listen(listening, SOMAXCONN);

    while (true) {
        sockaddr_in client;
        int clientSize = sizeof(client);

        SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::thread t(&Server::handleConnection, this, clientSocket);
        t.detach();
    }

    closesocket(listening);
}

void Server::handleConnection(SOCKET clientSocket) {
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        clientSockets.push_back(clientSocket);
    }

    char buffer[1024];
    std::string clientName;

    ZeroMemory(buffer, sizeof(buffer));
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        clientName = std::string(buffer, 0, bytesReceived);
        {
            std::lock_guard<std::mutex> lock(clientMutex);
            clientNames[clientSocket] = clientName;
        }

        std::string joinMsg = clientName + " has joined the chat.\n";
        std::cout << joinMsg;
        Utils::broadcastMessage(joinMsg, clientSocket, clientSockets, clientMutex);
    }
    else {
        std::cout << "Failed to receive username from client." << std::endl;
        closesocket(clientSocket);
        return;
    }

    while (true) {
        ZeroMemory(buffer, sizeof(buffer));
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            {
                std::lock_guard<std::mutex> lock(clientMutex);
                std::cout << clientName << " disconnected." << std::endl;
                clientNames.erase(clientSocket);
            }
            break;
        }

        std::string msg = "[" + clientName + "] " + std::string(buffer, 0, bytesReceived);
        std::cout << std::endl << msg << std::endl;
        Utils::broadcastMessage(msg, clientSocket, clientSockets, clientMutex);
    }

    {
        std::lock_guard<std::mutex> lock(clientMutex);
        clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
        std::string leaveMsg = clientName + " left the chat.\n";
        std::cout << leaveMsg;
        Utils::broadcastMessage(leaveMsg, clientSocket, clientSockets, clientMutex);
    }

    closesocket(clientSocket);
}
