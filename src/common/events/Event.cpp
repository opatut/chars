#include "Event.hpp"

Event::Event() {
    mId = 0;
}

Event::Event(sf::Uint32 id) {
    mId = id;
}

Event::Event(const std::string& id) {
    mId = StringManager::Get(id);
}

std::string Event::GetIdString() const {
    return StringManager::Get(mId);
}

sf::Uint32 Event::GetId() const {
    return mId;
}

sf::Packet Event::GetData() {
    return mData;
}
