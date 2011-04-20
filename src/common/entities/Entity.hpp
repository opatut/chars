#ifndef ENTITIES_ENTITY_HPP
#define ENTITIES_ENTITY_HPP

#include <string>

#include "common/Input.hpp"
#include "common/util/Removable.hpp"
#include "common/util/UIDManager.hpp"
#include "common/events/Event.hpp"

// forward declaration
class GameState;

class Entity : public Removable {
public:
    Entity() {}
    ~Entity() {}

    sf::Uint32 GrabUID();
    sf::Uint32 GetUID();

    virtual std::string GetType() = 0;
    sf::Uint8 GetTypeId();

    virtual void OnSpawn(GameState* state) = 0;
    virtual void OnDespawn() = 0;
    virtual void OnUpdate(float time_delta, Input& input) = 0;
    virtual void OnEvent(Event e) = 0;

    void Spawn(GameState* state);
    void Despawn();
    void Update(float time_delta, Input& input);
    void HandleEvent(Event e);

    float GetLifetime() const;
protected:
    GameState* mGameState;
    sf::Uint32 mUID;
    float mLifetime;
};

#endif
