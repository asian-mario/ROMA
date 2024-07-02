#pragma once
#include <string>
#include <vector>

class ServerConfig {
public:
    std::string name;
    std::string ipAddress;
    int port;

    static std::vector<ServerConfig> loadConfigs(const std::string& filename);
    static void saveConfig(const std::string& filename, const ServerConfig& config);
    static void saveAllConfigs(const std::string& filename, const std::vector<ServerConfig>& configs);
    static void deleteConfig(int index, std::vector<ServerConfig>& configs);
    static void editConfig(int index, std::vector<ServerConfig>& configs);
    static std::vector<ServerConfig> listConfigs(const std::string& filename, bool edit);
};