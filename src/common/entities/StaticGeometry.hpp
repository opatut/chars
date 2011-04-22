#ifndef ENTITIES_STATICGEOMETRY_HPP
#define ENTITIES_STATICGEOMETRY_HPP

#include <Ogre.h>
#include "Entity.hpp"
#include "common/config/ConfigurationNode.hpp"

class StaticGeometry : public Entity {
public:
    StaticGeometry(const std::string& model, Ogre::Vector3 pos);

    std::string GetType();
    void OnSpawn(GameState *state);
    void OnDespawn();
    void OnEvent(Event e);
    void OnUpdate(float time_delta, Input &input);

    void SetPosition(Ogre::Vector3 pos);
private:
    Ogre::SceneNode* mSceneNode;
    Ogre::Entity* mEntity;
    Ogre::Vector3 mPosition;
    std::string mModel;
};

#endif
