#include "NetworkManager.hpp"

#include "ChatMessageRequest.hpp"
#include "common/util/UIDManager.hpp"

NetworkManager::NetworkManager() {
    mMode = MODE_NONE;
    SetPort(25469);
}

void NetworkManager::SetListener(EventListener* listener) {
    mListener = listener;
}

void NetworkManager::SetMode(Mode m) {
    mMode = m;
}

void NetworkManager::SetPort(int port) {
    mPort = port;
}

bool NetworkManager::Connect() {
    // server
    if(mMode == MODE_SERVER) {
        if(mSocket.Bind(mPort) != sf::Socket::Done) {
            Logger::GetLogger().Error("Binding server to port " + tostr(mPort) + " failed.");
            return false;
        }
        mSocket.SetBlocking(false);
        Logger::GetLogger().Info("Binding server to port " + tostr(mPort) + " successful.");
        return true;
    } else {
        Logger::GetLogger().Error("NetworkManager cannot connect without server if not in server mode.");
        return false;
    }
}

bool NetworkManager::Connect(Recipient server) {
    // client
    if(mMode == MODE_CLIENT) {
        if(mSocket.Bind(mPort) != sf::Socket::Done) {
            Logger::GetLogger().Error("Binding client to port " + tostr(mPort) + " failed.");
            return false;
        }
        mSocket.SetBlocking(false);
        mClient_ServerRecipient = server;
        Logger::GetLogger().Info("Binding client to port " + tostr(mPort) + " successful.");
        return true;
    } else {
        Logger::GetLogger().Error("NetworkManager cannot connect to a server if not in client mode.");
        return false;
    }
}

void NetworkManager::HandleEvent(Event& e) {
    if(mListener != NULL)
        mListener->HandleEvent(e);
}

void NetworkManager::SendQueuedRequests() {
    while(mQueue.size() > 0) {
        SendRequest(mQueue.front());
        mQueue.pop_front();
    }
}

void NetworkManager::HandleIncomingRequests() {
    // TODO: do it
    sf::Uint16 port;
    sf::IpAddress address;
    sf::Packet packet;

    if(mSocket.Receive(packet, address, port) == sf::Socket::Done) {
        // yeah we got a packet!
        // get the requests from it
        mLastReceivedRequest = Request::ParsePacket(packet);

        if(mMode == MODE_SERVER) {
            // handle clients
            std::string client_name = mServer_ClientManager.GetClientName(address, port);
            if(client_name == "") {
                // remember client
                client_name = "unnamed-client-" + tostr(UIDManager::GrabNextUID());
                mServer_ClientManager.Add(client_name, Recipient(address, port));
            }
            mLastReceivedRequest->SetReceivedFrom(client_name);
        }

        mListener->HandleEvent(Event("network:request:received"));
    }
}

void NetworkManager::QueueRequest(Request* r) {
    mQueue.push_back(r);
}

Request* NetworkManager::GetLastReceivedRequest() {
    return mLastReceivedRequest;
}

NetworkManager::Mode NetworkManager::GetMode() const {
    return mMode;
}


Recipient* NetworkManager::GetServerRecipient() {
    Logger::GetLogger().Debug("Server recipient: " + mClient_ServerRecipient.GetAddress().ToString() + " " + tostr(mClient_ServerRecipient.GetPort()) );
    if(mMode != NetworkManager::MODE_CLIENT) {
        Logger::GetLogger().Error("Cannot get server recipient if NetworkManager not in client mode.");
        return NULL;
    } else {
        return &mClient_ServerRecipient;
    }
}


ClientManager& NetworkManager::GetClientManager() {
    return mServer_ClientManager;
}


void NetworkManager::SendRequest(Request& r) {
    sf::Packet packet;
    packet << r.GetTypeId() <<  r;

    if(mMode == MODE_SERVER) {
        std::vector<std::string> recipients = r.GetRecipients();
        if(recipients.size() == 0) {
            // send to all clients
            recipients = mServer_ClientManager.GetAllClientNames();
        }

        for(auto iter = recipients.begin(); iter != recipients.end(); ++iter) {
            Recipient* re = Recipient::GetByName(*iter);
            if(re != NULL) {
                mSocket.Send(packet, re->GetAddress(), re->GetPort());
            } else {
                Logger::GetLogger().Warning("Could not find recipient: " + *iter);
                for(auto iter2 = mServer_ClientManager.GetAllClientNames().begin(); iter2 != mServer_ClientManager.GetAllClientNames().end(); ++iter2) {
                    Logger::GetLogger().Warning("  On list: " + *iter2);
                }
            }
        }
    } else if(mMode == MODE_CLIENT) {
        mSocket.Send(packet, mClient_ServerRecipient.GetAddress(), mClient_ServerRecipient.GetPort());
    }
}


void NetworkManager::AddRequestPrototype(Request* r) {
    mRequestPrototypes.push_back(r);
}

Request* NetworkManager::CreateRequestInstanceOfType(sf::Uint32 type_id) {
    for(auto iter = mRequestPrototypes.begin(); iter != mRequestPrototypes.end(); ++iter) {
        if(iter->GetTypeId() == type_id) {
            return iter->NewInstance();
        }
    }
    return NULL;
}

Request* NetworkManager::CreateRequestInstanceOfType(const std::string& type_id) {
    return CreateRequestInstanceOfType(StringManager::Get(type_id));
}
