#include "Request.hpp"

#include "common/util/StringManager.hpp"
#include "common/network/NetworkManager.hpp"

Request::Request() {}

Request* Request::ParsePacket(sf::Packet& in) {
    sf::Uint32 type_id;
    in >> type_id;
    Request* r = NetworkManager::get_mutable_instance().CreateRequestInstanceOfType(type_id);
    in >> *r;
    return r;
}

sf::Uint32 Request::GetTypeId() const {
    return StringManager::Get(GetType());
}

std::string Request::GetType() const {
    return "request:generic";
}

void Request::SetReceivedFrom(const std::string&  r) {
    mReceivedFrom = r;
}

const std::string& Request::GetReceivedFrom() {
    return mReceivedFrom;
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

void Request::Serialize(IOPacket& p) {}

const std::vector<std::string>& Request::GetRecipients() {
    return mRecipients;
}

void Request::ClearRecipients() {
    mRecipients.clear();
}

void Request::AddRecipient(std::string r) {
    mRecipients.push_back(r);
}
