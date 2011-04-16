#ifndef EVENTS_EVENT_HPP
#define EVENTS_EVENT_HPP

#include <string>

#include "common/util/StringManager.hpp"
#include <SFML/Network.hpp>

class Event {
public:
    Event();
    Event(sf::Uint32 id);
    Event(const std::string& id);

    std::string GetIdString() const;
    sf::Uint32 GetId() const;

    sf::Packet GetData();
    template <class T> void WriteData(T d) {
        mData << d;
    }
    template <class T> T ReadData() {
        T d;
        mData >> d;
        return d;
    }

private:
    sf::Uint32 mId;
    sf::Packet mData;
};

#endif
