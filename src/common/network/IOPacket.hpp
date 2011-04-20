#ifndef NETWORK_IOPACKET_HPP
#define NETWORK_IOPACKET_HPP

#include <SFML/Network.hpp>

class IOPacket {
public:
    enum Mode {
        MODE_RECEIVE,   // uses operator >>
        MODE_SEND       // uses operator <<
    };

    IOPacket(sf::Packet* packet = new sf::Packet(), Mode mode = MODE_RECEIVE);

    template <typename T> IOPacket& operator & (T t) {
        if(mMode == MODE_RECEIVE)
            *mPacket >> t;
        else
            *mPacket << t;

        return *this;
    }

private:
    sf::Packet* mPacket;
    Mode mMode;
};

#endif
