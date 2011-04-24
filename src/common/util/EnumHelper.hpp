#ifndef UTILS_ENUMHELPER_HPP
#define UTILS_ENUMHELPER_HPP

#include <SFML/Network.hpp>

/**
  * Very very dirty class :D
  * Helps to serialize/deserialize enums
  *
  * Usage: sfpacket >> EnumHelper(&myEnumObject);
  *        sfpacket << EnumHelper(&myEnumObject);
  */

class EnumHelper {
public:
    EnumHelper(void* e);
    sf::Uint32 get();
    void set(sf::Uint32 i);
private:
    void* mEObj;
};

sf::Packet& operator << (sf::Packet& p, EnumHelper e);
sf::Packet& operator >> (sf::Packet& p, EnumHelper e);

#endif
