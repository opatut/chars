#include "Request.hpp"

#include "common/util/StringManager.hpp"
#include "common/network/NetworkManager.hpp"

void Request::Queue() {
    NetworkManager::get_mutable_instance().QueueRequest(this);
}

sf::Uint32 Request::GetTypeId() const {
    return StringManager::Get(GetType());
}

std::string Request::GetType() const {
    return "request:generic";
}


sf::Packet& operator <<(sf::Packet& p, Request& r) {
    IOPacket io(&p, IOPacket::MODE_SEND);
    r.Serialize(io);
    return p;
}

sf::Packet& operator >>(sf::Packet& p, Request& r) {
    IOPacket io(&p, IOPacket::MODE_RECEIVE);
    r.Serialize(io);
    return p;
}

void Request::Serialize(IOPacket& p) {
    p & GetTypeId();
}
