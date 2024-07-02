#include <iostream>
#include "src/Server.h"
#include "src/Client.h"
#include "src/ServerConfig.h"
#include "src/Utils.h"

const std::string configFileName = "./data/server_config.txt";

int main() {
    WSADATA wsData;
    WSAStartup(MAKEWORD(2, 2), &wsData);

    Server server;
    Client client;

    while (true) {
        std::string mode;
        std::cout << "Enter mode (server, client, save, list, run, exit): ";
        std::cin >> mode;
        std::cin.ignore();

        if (mode == "server") {
            int port;
            std::cout << "Enter port number for the server: ";
            std::cin >> port;
            std::cin.ignore();

            server.start(port);
        }
        else if (mode == "client") {
            std::cout << "1. Browse servers (not implemented)\n2. Enter server IP\nSelect an option: ";
            int choice;
            std::cin >> choice;
            std::cin.ignore();

            if (choice == 1) {
                std::cout << "Server browsing functionality is not implemented yet." << std::endl;
            }
            else if (choice == 2) {
                std::string ipAddress;
                std::cout << "Enter server IP: ";
                std::getline(std::cin, ipAddress);

                int port;
                std::cout << "Enter port number: ";
                std::cin >> port;
                std::cin.ignore();

                std::string username;
                std::cout << "Enter your username: ";
                std::getline(std::cin, username);

                client.start(ipAddress, port, username);
            }
        }
        else if (mode == "save") {
            std::string name, ip;
            int port;
            std::cout << "Enter name for the server config: ";
            std::getline(std::cin, name);
            std::cout << "IP address: " << Utils::getLocalIPAddress() << std::endl;
            ip = Utils::getLocalIPAddress();
            std::cout << "Enter port: ";
            std::cin >> port;
            std::cin.ignore();

            ServerConfig newConfig{ name, ip, port };
            ServerConfig::saveConfig(configFileName, newConfig);
        }
        else if (mode == "list") {
            ServerConfig::listConfigs(configFileName, true);
        }
        else if (mode == "run") {
            auto configs = ServerConfig::listConfigs(configFileName, false);
            std::cout << "Enter the number of the server config to run: ";
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            if (choice > 0 && choice <= configs.size()) {
                const auto& config = configs[choice - 1];
                std::cout << "Running " << config.name << " on " << config.ipAddress << ":" << config.port << std::endl;
                server.start(config.port);
            }
            else {
                std::cerr << "Invalid selection." << std::endl;
            }
        }
        else if (mode == "exit") {
            break;
        }
        else {
            std::cerr << "Invalid mode! Use 'server', 'client', 'save', 'list', 'run' or 'exit'." << std::endl;
        }
    }

    WSACleanup();
    return 0;
}
