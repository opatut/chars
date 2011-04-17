#ifndef ENTITIES_CHARACTER_HPP
#define ENTITIES_CHARACTER_HPP

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <SFML/System.hpp>

#include "common/entities/Entity.hpp"

// forward declaration
class GameState;

class Character : public Entity {
public:
    void OnSpawn(GameState* state);
    void OnDespawn();

    void OnUpdate(float time_delta, Input& input);
    void OnEvent(Event& e);

    sf::Uint16 GetPlayerID() const;
    void SetPlayerID(sf::Uint16 id = 0);
private:
    sf::Uint16 mPlayerID;
    Ogre::Entity* mHead;
    Ogre::SceneNode* mHeadNode;
    Ogre::SceneNode* mCamNode;
    Ogre::Vector3 mVelocity;
};

#endif
