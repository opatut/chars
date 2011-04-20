#include "NetworkManager.hpp"

NetworkManager::NetworkManager() {
    mMode = MODE_NONE;
}

void NetworkManager::SetListener(EventListener* listener) {
    mListener = listener;
}

void NetworkManager::SetMode(Mode m) {
    mMode = m;
}

void NetworkManager::HandleEvent(Event& e) {
    if(mListener != NULL)
        mListener->HandleEvent(e);
}

void NetworkManager::SendQueuedRequests() {
    // TODO: do it
}

void NetworkManager::HandleIncomingRequests() {
    // TODO: do it
}
