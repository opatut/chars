#ifndef CONFIG_CONFIGURATION_HPP
#define CONFIG_CONFIGURATION_HPP

#include <fstream>

#include <yaml-cpp/yaml.h>

#include "ConfigurationNode.hpp"

class Configuration : public ConfigurationNode {
public:
    Configuration();
    Configuration(const std::string& file);
    bool Load();
    bool Save();

    void ParseNode(const YAML::Node& node, const std::string& path = "");
    void SaveNode(YAML::Emitter& out, const std::string& path = "");

    void SetFile(const std::string& file);
    const std::string& GetFile() const;
private:
    std::string mFile;
};

#endif
