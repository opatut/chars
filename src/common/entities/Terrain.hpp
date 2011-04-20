#ifndef ENTITIES_TERRAIN_HPP
#define ENTITIES_TERRAIN_HPP

#include <PagedGeometry.h>
#include <BatchPage.h>
#include <ImpostorPage.h>
#include <TreeLoader3D.h>

#include "common/entities/Entity.hpp"

// forward declaration
class GameState;

class Terrain : public Entity {
public:
    void SetTerrainId(const std::string& id);
    void OnSpawn(GameState* state);
    void OnDespawn();

    void OnUpdate(float time_delta, Input& input);
    void OnEvent(Event e);
private:
    Ogre::Entity* mTerrain;
    std::string mTerrainId;
    Forests::PagedGeometry* mGeometry;
};

#endif
