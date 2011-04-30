#include "PingRequest.hpp"

#include "common/util/Utils.hpp"

PingRequest::PingRequest(float starttime, bool is_origin_server) {
    mStartTime = starttime;
    mIsOriginServer = is_origin_server;
}

std::string PingRequest::GetType() const {
    return "request:ping";
}

Request* PingRequest::NewInstance() {
    return new PingRequest();
}

float PingRequest::GetStartTime() const {
    return mStartTime;
}

bool PingRequest::IsOriginServer() const {
    return mIsOriginServer;
}

void PingRequest::Serialize(IOPacket &p) {
    p & mStartTime;
    p & mIsOriginServer;
}
