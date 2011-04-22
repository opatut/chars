#ifndef NETWORK_NETWORKMANAGER_HPP
#define NETWORK_NETWORKMANAGER_HPP

#include <boost/serialization/singleton.hpp>
#include <SFML/Network.hpp>

#include "common/log/Logger.hpp"
#include "Request.hpp"
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
    void SetMode(Mode m);

    void HandleEvent(Event& e);

    void SendQueuedRequests();
    void HandleIncomingRequests();

    void QueueRequest(Request* r);
private:
    EventListener* mListener;
    Mode mMode;
};

#endif
