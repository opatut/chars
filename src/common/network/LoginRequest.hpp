#ifndef NETWORK_LOGINREQUEST_HPP
#define NETWORK_LOGINREQUEST_HPP

#include "Request.hpp"

class LoginRequest : public Request {
public:
    LoginRequest();
    LoginRequest(const std::string& username, const std::string& password, bool hashed = false);

    virtual Request* NewInstance();

    std::string GetType() const;

    void SetUsername(const std::string& username);
    void SetPassword(const std::string& password, bool hashed = false);

    const std::string& GetUsername();
    const std::string& GetPasswordHash();
private:
    std::string mUsername;
    std::string mPasswordHash;
    void Serialize(IOPacket& p);
};

#endif
