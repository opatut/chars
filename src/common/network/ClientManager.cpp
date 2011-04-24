#include "ClientManager.hpp"

#include "NetworkManager.hpp"

bool ClientManager::Add(const std::string& name, Recipient recipient) {
    if(!IsCorrectMode())
        return false;

    if(mClients.count(name) > 0) {
        Logger::GetLogger().Error("Cannot add client: " + name + ". A client with this name already exists.");
        return false;
    }
    mClients[name] = recipient;
    return true;
}

bool ClientManager::Rename(const std::string& from, const std::string& to) {
    if(!IsCorrectMode())
        return false;

    // Logger::GetLogger().Info("Renaming client from " + from + " to " + to + ".");

    Recipient r = mClients[from];
    mClients.erase(from);
    mClients[to] = r;
    return true;
}

void ClientManager::Remove(const std::string& name) {
    if(!IsCorrectMode())
        return;

    mClients.erase(name);
}

Recipient* ClientManager::GetRecipient(const std::string& name) {
    if(!IsCorrectMode())
        return NULL;

    for(auto iter = mClients.begin(); iter != mClients.end(); ++iter) {
        if(iter->first == name) {
            return &(iter->second);
        }
    }
    return NULL;
}

const std::string& ClientManager::GetClientName(sf::IpAddress address, sf::Uint16 port) {
    for(auto iter = mClients.begin(); iter != mClients.end(); ++iter) {
        if(iter->second.GetAddress() == address && iter->second.GetPort() == port) {
            return iter->first;
        }
    }
    return "";
}

std::vector<std::string> ClientManager::GetAllClientNames() {
    std::vector<std::string> res;
    for(auto iter = mClients.begin(); iter != mClients.end(); ++iter) {
        res.push_back(iter->first);
    }
    return res;
}

bool ClientManager::IsCorrectMode() {
    if(NetworkManager::get_mutable_instance().GetMode() != NetworkManager::MODE_SERVER) {
        Logger::GetLogger().Error("ClientManager cannot operate on NetworkManager in client mode / without mode.");
        return false;
    } else {
        return true;
    }
}
