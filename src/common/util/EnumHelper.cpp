#include "EnumHelper.hpp"

EnumHelper::EnumHelper(void* e) {
    mEObj = e;
}

sf::Uint32 EnumHelper::get() {
    return (sf::Uint32)(*((int*)mEObj));
}

void EnumHelper::set(sf::Uint32 i) {
   *((int*)mEObj) = i;
}

sf::Packet& operator << (sf::Packet& p, EnumHelper e) {
    sf::Uint32 i = e.get();
    p << i;
    return p;
}

sf::Packet& operator >> (sf::Packet& p, EnumHelper e) {
    sf::Uint32 i;
    p >> i;
    e.set(i);
    return p;
}
