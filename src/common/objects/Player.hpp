#ifndef OBJECTS_PLAYER_HPP
#define OBJECTS_PLAYER_HPP

#include <string>
#include "ChatMessage.hpp"#
#include "common/network/ChatMessageRequest.hpp"
#include "common/network/NetworkManager.hpp"

class Player {
public:
    Player(const std::string& name, sf::Uint32 id);

    // should not be called on client's own player
    // ? called on other player when sending private chat message
    // called on any player in server
    void SendMessage(const ChatMessage& msg);

    sf::Uint32 GetID() const;
    const std::string& GetName() const;
private:
    std::string mName;
    sf::Uint32 mID;
};

#endif
