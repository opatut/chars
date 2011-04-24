#ifndef NETWORK_NETWORKMANAGER_HPP
#define NETWORK_NETWORKMANAGER_HPP

#include <boost/serialization/singleton.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <SFML/Network.hpp>

#include "common/util/Utils.hpp"
#include "common/log/Logger.hpp"
#include "Request.hpp"
#include "ClientManager.hpp"
#include "common/events/EventListener.hpp"

class NetworkManager : public boost::serialization::singleton<NetworkManager> {
public:
    enum Mode {
        MODE_NONE,
        MODE_SERVER,
        MODE_CLIENT
    };
    NetworkManager();

    void SetListener(EventListener* listener);
    void SetPort(int port);
    void SetMode(Mode m);
    bool Connect(); // server
    bool Connect(Recipient server); // client

    void HandleEvent(Event& e);

    void SendQueuedRequests();
    void HandleIncomingRequests();

    void QueueRequest(Request* r);
    Request* GetLastReceivedRequest();

    Mode GetMode() const;
    Recipient* GetServerRecipient();

    ClientManager& GetClientManager();

    // Request prototyping
    void AddRequestPrototype(Request* r);
    Request* CreateRequestInstanceOfType(sf::Uint32 type_id);
    Request* CreateRequestInstanceOfType(const std::string& type_id);
private:
    void SendRequest(Request& r);

    Recipient mClient_ServerRecipient;
    ClientManager mServer_ClientManager;

    sf::UdpSocket mSocket;
    Request* mLastReceivedRequest;
    EventListener* mListener;
    Mode mMode;
    int mPort;

    boost::ptr_deque<Request> mQueue;

    boost::ptr_vector<Request> mRequestPrototypes;
};

#endif
