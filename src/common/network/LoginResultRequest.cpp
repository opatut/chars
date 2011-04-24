#include "LoginResultRequest.hpp"

LoginResultRequest::LoginResultRequest() {}

LoginResultRequest::LoginResultRequest(const std::string& username, LoginResultRequest::Result result, const std::string& message) {
    SetUsername(username);
    SetResult(result);
    SetMessage(message);
}

Request* LoginResultRequest::NewInstance() {
    return new LoginResultRequest("", UNKNOWN_RESULT, "");
}

std::string LoginResultRequest::GetType() const {
    return "request:loginresult";
}

void LoginResultRequest::SetResult(Result result) {
    mResult = result;
}

void LoginResultRequest::SetUsername(const std::string& username) {
    mUsername = username;
}

void LoginResultRequest::SetMessage(const std::string& message) {
    mMessage = message;
}

LoginResultRequest::Result LoginResultRequest::GetResult() const {
    return mResult;
}

const std::string& LoginResultRequest::GetUsername() const {
    return mUsername;
}

const std::string& LoginResultRequest::GetMessage() const {
    return mMessage;
}

void LoginResultRequest::Serialize(IOPacket& p) {
    p & mUsername;
    p & EnumHelper(&mResult);
    p & mMessage;
}
