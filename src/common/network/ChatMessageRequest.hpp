#ifndef NETWORK_CHATMESSAGEREQUEST_HPP
#define NETWORK_CHATMESSAGEREQUEST_HPP

#include "Request.hpp"
#include "common/objects/ChatMessage.hpp"

class ChatMessageRequest : public Request {
public:
    ChatMessageRequest();
    ChatMessageRequest(ChatMessage msg);

    virtual Request* NewInstance();

    std::string GetType() const;

private:
    ChatMessage mChatMessage;

    void Serialize(IOPacket& p);
};

#endif
