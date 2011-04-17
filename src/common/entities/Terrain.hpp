#ifndef ENTITIES_TERRAIN_HPP
#define ENTITIES_TERRAIN_HPP

#include <Ogre.h>
#include <SFML/System.hpp>

#include "common/entities/Entity.hpp"

// forward declaration
class GameState;

class Terrain : public Entity {
public:
    void OnSpawn(GameState* state);
    void OnDespawn();

    void OnUpdate(float time_delta, Input& input);
    void OnEvent(Event& e);
private:
    Ogre::Entity* mTerrain;
};

#endif
