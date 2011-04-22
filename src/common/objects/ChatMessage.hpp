#ifndef OBJECTS_CHATMESSAGE_HPP
#define OBJECTS_CHATMESSAGE_HPP

#include <string>

// forward declaration
class ChatMessageRequest;

class ChatMessage {
public:
    enum Type {
        TYPE_GENERAL = 0,
        TYPE_ANNOUNCEMENT = 1,
        TYPE_CHARACTER_STATEMENT = 2,
        TYPE_ERROR = 3
    };

    ChatMessage(const std::string& message = "", Type type = TYPE_GENERAL);

    void SetType(Type type);
    Type GetType() const;

    void SetMessage(const std::string& message);
    const std::string& GetMessage() const;

    friend class ChatMessageRequest;
private:
    Type mType;
    std::string mMessage;
};

#endif
