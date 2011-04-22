#include "Player.hpp"

Player::Player(const std::string& name, sf::Uint32 id) {

}

void Player::SendMessage(const ChatMessage& msg) {
    ChatMessageRequest r(msg);
    r.Queue();
}

sf::Uint32 Player::GetID() const {

}

const std::string& Player::GetName() const {

}
