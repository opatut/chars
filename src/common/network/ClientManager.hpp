#ifndef NETWORK_CLIENTMANAGER_HPP
#define NETWORK_CLIENTMANAGER_HPP

#include <string>
#include <boost/ptr_container/ptr_map.hpp>

#include "Recipient.hpp"

class ClientManager {
public:
    bool Add(const std::string& name, Recipient recipient);
    bool Rename(const std::string& from, const std::string& to);
    void Remove(const std::string& name);

    Recipient* GetRecipient(const std::string& name);
    const std::string& GetClientName(sf::IpAddress address, sf::Uint16 port);

    std::vector<std::string> GetAllClientNames();
private:
    bool IsCorrectMode();
    std::map<std::string, Recipient> mClients;
};

#endif
