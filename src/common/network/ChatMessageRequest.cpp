#include "ChatMessageRequest.hpp"

ChatMessageRequest::ChatMessageRequest() {}

ChatMessageRequest::ChatMessageRequest(ChatMessage msg) {
    mChatMessage = msg;
}

Request* ChatMessageRequest::NewInstance() {
    return new ChatMessageRequest(ChatMessage());
}

std::string ChatMessageRequest::ChatMessageRequest::GetType() const {
    return "request:chatmessage";
}

void ChatMessageRequest::Serialize(IOPacket& p) {
    p & mChatMessage.mMessage;
    p & mChatMessage.mType;
}
