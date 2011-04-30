#include "Definitions.hpp"

#include "network/LoginRequest.hpp"
#include "network/LoginResultRequest.hpp"
#include "network/ChatMessageRequest.hpp"
#include "network/PingRequest.hpp"

void define_strings() {
    // default client events
    StringManager::Add("input:mouse:moved");
    StringManager::Add("input:mouse:pressed");
    StringManager::Add("input:mouse:released");
    StringManager::Add("input:keyboard:pressed");
    StringManager::Add("input:keyboard:released");
    StringManager::Add("window:resized");

    // network events
    StringManager::Add("network:request:received");

    // entity types
    StringManager::Add("entity:type:character");
    StringManager::Add("entity:type:terrain");
    StringManager::Add("entity:type:staticgeometry");

    // test
    StringManager::Add("char_goto");

    // key bindings
    StringManager::Add("client:key:forward");
    StringManager::Add("client:key:back");
    StringManager::Add("client:key:left");
    StringManager::Add("client:key:right");

    // request types
    StringManager::Add("request:generic");
    StringManager::Add("request:ping");
    StringManager::Add("request:login");
    StringManager::Add("request:loginresult");
    StringManager::Add("request:logout");
    StringManager::Add("request:chatmessage");
}

void register_request_prototypes() {
    NetworkManager::get_mutable_instance().AddRequestPrototype(new LoginRequest());
    NetworkManager::get_mutable_instance().AddRequestPrototype(new ChatMessageRequest());
    NetworkManager::get_mutable_instance().AddRequestPrototype(new LoginResultRequest());
    NetworkManager::get_mutable_instance().AddRequestPrototype(new PingRequest());
}
