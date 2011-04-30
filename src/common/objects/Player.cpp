#include "Player.hpp"

#include "common/gamestate/GameState.hpp"

Player::Player(const std::string& name, sf::Uint32 id) {
    mName = name;
    mID = id;
    mPing = 0;
}

void Player::SendMessage(const ChatMessage& msg) {
    NetworkManager::get_mutable_instance().QueueRequest(new ChatMessageRequest(msg));
}

sf::Uint32 Player::GetID() const {
    return mID;
}

const std::string& Player::GetName() const {
    return mName;
}

void Player::SetPing(int ping) {
    Logger::GetLogger().Info("User " + mName + " has ping " + tostr(ping));
    mPing = ping;
}

int Player::GetPing() const {
    return mPing;
}
