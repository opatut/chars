#include "ChatMessageRequest.hpp"

ChatMessageRequest::ChatMessageRequest(ChatMessage msg) {
    mChatMessage = msg;
}

std::string ChatMessageRequest::ChatMessageRequest::GetType() const {
    return "request:chatmessage";
}

void ChatMessageRequest::Serialize(IOPacket& p) {
    Request::Serialize(p);
    p & mChatMessage.mMessage;
    p & mChatMessage.mType;
}
