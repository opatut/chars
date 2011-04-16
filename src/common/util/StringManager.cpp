#include "StringManager.hpp"

std::map<std::string, sf::Uint32> StringManager::StringIds;
sf::Uint32 StringManager::LastId = 0;

void StringManager::Add(const std::string& str) {
    ++LastId;
    StringIds[str] = LastId;
}

sf::Uint32 StringManager::Get(const std::string& str) {
    return StringIds[str];
}

std::string StringManager::Get(int i) {
    for(auto iter = StringIds.begin(); iter != StringIds.end(); ++iter) {
        if(iter->second == i) {
            return iter->first;
        }
    }
    return "";
}
