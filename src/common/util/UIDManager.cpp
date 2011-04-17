#include "UIDManager.hpp"

sf::Uint32 UIDManager::NextUID = 1000;

sf::Uint32 UIDManager::GrabNextUID() {
    sf::Uint32 i = NextUID;
    ++NextUID;
    return i;
}

