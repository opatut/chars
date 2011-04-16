#ifndef UTIL_STRINGMANAGER_HPP
#define UTIL_STRINGMANAGER_HPP

#include <map>

#include <SFML/System.hpp>

class StringManager {
public:
    static void Add(const std::string& str);
    static sf::Uint32 Get(const std::string& str);
    static std::string Get(int i);
private:
    static std::map<std::string, sf::Uint32> StringIds;
    static sf::Uint32 LastId;
};

#endif
