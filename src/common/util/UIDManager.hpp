#ifndef UTIL_UIDMANAGER_HPP
#define UTIL_UIDMANAGER_HPP

#include <SFML/System.hpp>

class UIDManager {
public:
    static sf::Uint32 GrabNextUID();
private:
    static sf::Uint32 NextUID;
};

#endif
