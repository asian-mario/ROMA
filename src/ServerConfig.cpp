#include "ServerConfig.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<ServerConfig> ServerConfig::loadConfigs(const std::string& filename) {
    std::vector<ServerConfig> configs;
    std::ifstream inFile(filename);
    std::string line;

    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            std::istringstream iss(line);
            std::string name, ip;
            int port;
            if (getline(iss, name, ',') && getline(iss, ip, ',') && iss >> port) {
                configs.push_back({ name, ip, port });
            }
        }
        inFile.close();
    }
    return configs;
}

void ServerConfig::saveConfig(const std::string& filename, const ServerConfig& config) {
    std::ofstream outFile(filename, std::ios::app);  // Open in append mode
    if (outFile) {
        outFile << config.name << "," << config.ipAddress << "," << config.port << "\n";
    }
    else {
        std::cerr << "Could not open the config file for writing." << std::endl;
    }
}

void ServerConfig::saveAllConfigs(const std::string& filename, const std::vector<ServerConfig>& configs) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& config : configs) {
            outFile << config.name << "," << config.ipAddress << "," << config.port << std::endl;
        }
        outFile.close();
    }
    else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}

void ServerConfig::deleteConfig(int index, std::vector<ServerConfig>& configs) {
    if (index >= 0 && index < configs.size()) {
        configs.erase(configs.begin() + index);
        std::cout << "Configuration deleted." << std::endl;
    }
    else {
        std::cerr << "Invalid configuration index." << std::endl;
    }
}

void ServerConfig::editConfig(int index, std::vector<ServerConfig>& configs) {
    if (index >= 0 && index < configs.size()) {
        std::string name, ip;
        int port;

        std::cout << "Enter new name (current: " << configs[index].name << "): ";
        std::getline(std::cin, name);
        std::cout << "Enter new IP address (current: " << configs[index].ipAddress << "): ";
        std::getline(std::cin, ip);
        std::cout << "Enter new port (current: " << configs[index].port << "): ";
        std::cin >> port;
        std::cin.ignore();

        configs[index] = { name, ip, port };
        std::cout << "Configuration updated." << std::endl;
    }
    else {
        std::cerr << "Invalid configuration index." << std::endl;
    }
}

std::vector<ServerConfig> ServerConfig::listConfigs(const std::string& filename, bool edit) {
    auto configs = loadConfigs(filename);
    std::cout << "Saved server configurations:" << std::endl;
    for (int i = 0; i < configs.size(); ++i) {
        std::cout << i + 1 << ". Name: " << configs[i].name
            << ", IP: " << configs[i].ipAddress
            << ", Port: " << configs[i].port << std::endl;
    }

    if (edit) {
        std::cout << "Enter 'd' to delete, 'e' to edit a configuration, or 'b' to go back: ";
        std::string action;
        std::cin >> action;
        std::cin.ignore();

        if (action == "d" || action == "e") {
            int index;
            std::cout << "Enter the number of the configuration: ";
            std::cin >> index;
            std::cin.ignore();
            index--;  // Adjust for zero-based index

            if (action == "d") {
                deleteConfig(index, configs);
            }
            else if (action == "e") {
                editConfig(index, configs);
            }

            saveAllConfigs(filename, configs); // Save changes back to the file
        }
    }
    return configs;
}
