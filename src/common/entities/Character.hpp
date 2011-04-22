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
    std::string GetType();

    void OnSpawn(GameState* state);
    void OnDespawn();

    void OnUpdate(float time_delta, Input& input);
    void OnEvent(Event e);

    bool OwnsObject(Ogre::MovableObject* o);

    sf::Uint16 GetPlayerID() const;
    void SetPlayerID(sf::Uint16 id = 0);

    void SetPosition(Ogre::Vector3 pos);
private:
    sf::Uint16 mPlayerID;
    Ogre::Entity* mHead;
    Ogre::SceneNode* mHeadNode;
};

#endif
