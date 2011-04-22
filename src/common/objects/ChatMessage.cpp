#include "ChatMessage.hpp"

ChatMessage::ChatMessage(const std::string& message, Type type) {
    SetMessage(message);
    SetType(type);
}

void ChatMessage::SetType(ChatMessage::Type type) {
    mType = type;
}

ChatMessage::Type ChatMessage::GetType() const {
    return mType;
}

void ChatMessage::SetMessage(const std::string& message) {
    mMessage = message;
}

const std::string& ChatMessage::GetMessage() const {
    return mMessage;
}

