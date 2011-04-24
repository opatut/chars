#ifndef NETWORK_REQUEST_HPP
#define NETWORK_REQUEST_HPP

#include <boost/shared_ptr.hpp>

#include <SFML/Network.hpp>
#include "Recipient.hpp"
#include "IOPacket.hpp"
#include "common/util/EnumHelper.hpp"

class Request {
public:
    Request();
    static Request* ParsePacket(sf::Packet& in);

    virtual std::string GetType() const;
    virtual Request* NewInstance() = 0;
    void SetReceivedFrom(const std::string& r);
    const std::string& GetReceivedFrom();

    sf::Uint32 GetTypeId() const;
    const std::vector<std::string>& GetRecipients();
    void ClearRecipients();
    void AddRecipient(std::string r);

    // hi friend !
    friend sf::Packet& operator <<(sf::Packet&, Request&);
    friend sf::Packet& operator >>(sf::Packet&, Request&);

protected:
    virtual void Serialize(IOPacket& p);
    std::vector<std::string> mRecipients; // only care about this when in server mode, contains client names, or is empty for all clients

    // only care about this in server mode
    std::string mReceivedFrom;
};

sf::Packet& operator <<(sf::Packet& p, Request& r);
sf::Packet& operator >>(sf::Packet& p, Request& r);

#endif
