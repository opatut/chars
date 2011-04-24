#ifndef NETWORK_LOGINRESULTREQUEST_HPP
#define NETWORK_LOGINRESULTREQUEST_HPP

#include "Request.hpp"

class LoginResultRequest : public Request {
public:
    enum Result {
        SUCCESS = 0,
        FAIL_BAD_LOGIN = 1,
        FAIL_ALREADY_LOGGED_IN = 2,
        FAIL_BANNED = 3,
        FAIL_UNKNOWN_REASON = 4,
        UNKNOWN_RESULT = 5
    };

    LoginResultRequest();
    LoginResultRequest(const std::string& username, Result result, const std::string& message);

    virtual Request* NewInstance();

    std::string GetType() const;

    void SetResult(Result result);
    void SetUsername(const std::string& username);
    void SetMessage(const std::string& message);

    Result GetResult() const;
    const std::string& GetUsername() const;
    const std::string& GetMessage() const;
private:
    std::string mUsername;
    std::string mMessage;
    Result mResult;

    void Serialize(IOPacket& p);
};

#endif
