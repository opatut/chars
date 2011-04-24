#include "Recipient.hpp"

#include "NetworkManager.hpp"

Recipient::Recipient() {
    mAddress = sf::IpAddress::GetLocalAddress();
    mPort = 0;
}

Recipient* Recipient::GetByName(const std::string& name) {
    return NetworkManager::get_mutable_instance().GetClientManager().GetRecipient(name);
}

Recipient* Recipient::GetServerRecipient() {
    return NetworkManager::get_mutable_instance().GetServerRecipient();
}

Recipient::Recipient(sf::IpAddress address, sf::Uint16 port) {
    mAddress = address;
    mPort = port;
}

const sf::IpAddress& Recipient::GetAddress() const {
    return mAddress;
}

sf::Uint16 Recipient::GetPort() const {
    return mPort;
}

const std::string& Recipient::GetName() const {
    return NetworkManager::get_mutable_instance().GetClientManager().GetClientName(mAddress, mPort);
}
