#ifndef EVENT_EVENTLISTENER_HPP
#define EVENT_EVENTLISTENER_HPP

#include "Event.hpp"

class EventListener {
public:
    virtual void HandleEvent(Event e) = 0;
};

#endif
