#ifndef NETWORK_REQUEST_HPP
#define NETWORK_REQUEST_HPP

#include <boost/shared_ptr.hpp>

#include <SFML/Network.hpp>

#include "IOPacket.hpp"

class Request {
public:
    static boost::shared_ptr<Request> ParsePacket(sf::Packet& in);

    friend sf::Packet& operator <<(sf::Packet&, Request&);
    friend sf::Packet& operator >>(sf::Packet&, Request&);

    sf::Uint32 GetTypeId() const;
    virtual std::string GetType() const;
protected:
    virtual void Serialize(IOPacket& p);
};

sf::Packet& operator <<(sf::Packet& p, Request& r);
sf::Packet& operator >>(sf::Packet& p, Request& r);

#endif
