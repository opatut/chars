#ifndef NETWORK_PINGREQUEST_HPP
#define NETWORK_PINGREQUEST_HPP

#include "Request.hpp"

class PingRequest : public Request {
public:
    PingRequest(float starttime = 0.f, bool is_origin_server = true);

    std::string GetType() const;
    virtual Request* NewInstance();

    float GetStartTime() const;
    bool IsOriginServer() const;

private:
    void Serialize(IOPacket &p);

    float mStartTime;
    bool mIsOriginServer; // true if server initiated this ping, false if client did
};

#endif
