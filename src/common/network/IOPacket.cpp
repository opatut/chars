#include "IOPacket.hpp"

IOPacket::IOPacket(sf::Packet* packet, Mode mode) {
    mPacket = packet;
    mMode = mode;
}
