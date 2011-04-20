#include "Entity.hpp"

void Entity::GrabUID() {
    mUID = UIDManager::GrabNextUID();
}

sf::Uint32 Entity::GetUID() {
    return mUID;
}

void Entity::Spawn(GameState* state) {
    mGameState = state;
    mLifetime = 0;
    OnSpawn(state);
}

void Entity::Despawn() {
    OnDespawn();
}

void Entity::Update(float time_delta, Input& input) {
    mLifetime += time_delta;
    OnUpdate(time_delta, input);
}

void Entity::HandleEvent(Event e) {
    OnEvent(e);
}
