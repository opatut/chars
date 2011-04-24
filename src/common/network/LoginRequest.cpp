#include "LoginRequest.hpp"

#include "common/log/Logger.hpp"
#include "common/util/Utils.hpp"

LoginRequest::LoginRequest() {}

LoginRequest::LoginRequest(const std::string& username, const std::string& password, bool hashed) {
    SetUsername(username);
    SetPassword(password, hashed);
}

Request* LoginRequest::NewInstance() {
    return new LoginRequest("", "", true);
}

std::string LoginRequest::GetType() const {
    return "request:login";
}

void LoginRequest::SetUsername(const std::string& username) {
    mUsername = username;
}

void LoginRequest::SetPassword(const std::string& password, bool hashed) {
    if(hashed)
        mPasswordHash = password;
    else
        mPasswordHash = sha(password);
}

const std::string& LoginRequest::GetUsername() {
    return mUsername;
}

const std::string& LoginRequest::GetPasswordHash() {
    return mPasswordHash;
}

void LoginRequest::Serialize(IOPacket& p) {
    p & mUsername;
    p & mPasswordHash;
}
