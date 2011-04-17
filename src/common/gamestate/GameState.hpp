#ifndef GAMESTATE_GAMESTATE_HPP
#define GAMESTATE_GAMESTATE_HPP

#include <string>
#include <iostream>

#include <boost/ptr_container/ptr_vector.hpp>

#include <OgreSceneManager.h>
#include <OIS.h>

#include "common/entities/Entity.hpp"
#include "common/events/Event.hpp"

class GameState {
public:
    virtual std::string GetName() const { return "noname"; }
    virtual Ogre::SceneManager* GetSceneMgr();
    virtual Ogre::Camera* GetCamera();

    virtual void OnEnable() = 0;
    virtual void OnDisable() = 0;
    virtual void OnLoadResources() {}
    virtual void OnUnloadResources() {}
    virtual void OnEvent(Event& e) {}
    virtual void OnUpdate(float time_delta, Input& input) = 0;

    void Enable();
    void Disable();
    void LoadResources();
    void UnloadResources();
    bool HandleEvent(Event& e);
    bool Update(float time_delta, Input& input);

    void AddEntity(Entity* e);
    void RemoveEntity(Entity* e);
    void RemoveEntity(sf::Uint32 uid);
    Entity* GetEntity(sf::Uint32 uid);

    void PassToNextState();
protected:
    bool mPassToNextState;
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;

    boost::ptr_vector<Entity> mEntities;
};

#endif
