#ifndef NETWORK_RECIPIENT_HPP
#define NETWORK_RECIPIENT_HPP

#include <string>
#include <SFML/Network.hpp>

class Recipient {
public:
    static Recipient* GetByName(const std::string& name);
    static Recipient* GetServerRecipient();

    Recipient();
    Recipient(sf::IpAddress address, sf::Uint16 port);

    const sf::IpAddress& GetAddress() const;
    sf::Uint16 GetPort() const;
    const std::string& GetName() const;
private:
    sf::IpAddress mAddress;
    sf::Uint16 mPort;
};

#endif
