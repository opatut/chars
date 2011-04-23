#ifndef GAMESTATE_GAMESTATE_HPP
#define GAMESTATE_GAMESTATE_HPP

#include <string>
#include <iostream>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>

#include <OgreSceneManager.h>
#include <OIS.h>

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

#include "common/entities/Entity.hpp"
#include "common/events/Event.hpp"
#include "common/log/Logger.hpp"

class GameState {
public:
    virtual std::string GetName() const { return "noname"; }
    virtual Ogre::SceneManager* GetSceneMgr();
    virtual Ogre::Camera* GetCamera();

    virtual void OnLoadResources() {}
    virtual void OnEnable() = 0;
    virtual void OnInitializeGUI() {}

    virtual void OnDeinitializeGUI() {}
    virtual void OnDisable() = 0;
    virtual void OnUnloadResources() {}

    virtual void OnEvent(Event e) {}
    virtual void OnUpdate(float time_delta, Input& input) = 0;

    // ===============================================

    void LoadResources();
    void Enable();
    void InitializeGUI();

    void DeinitializeGUI();
    void Disable();
    void UnloadResources();

    bool HandleEvent(Event e);
    bool Update(float time_delta, Input& input);

    // ===============================================

    MyGUI::Gui* GetGUI();
    bool IsGUIFocused();
    bool IsKeyDown(Input& input, OIS::KeyCode key);
    void AddEntity(Entity* e);
    void RemoveEntity(Entity* e);
    void RemoveEntity(sf::Uint32 uid);
    Entity* GetEntity(sf::Uint32 uid);
    std::vector<Entity*> GetEntitiesByType(const std::string& type);

    // ===============================================

    template <typename T>
    T* GetEntity(sf::Uint32 uid) {
        return (T*)GetEntity(uid);
    }

    template <typename T>
    std::vector<T*> GetEntitiesByType() {
        T t;
        std::vector<T*> r;
        BOOST_FOREACH(Entity* e, GetEntitiesByType(t.GetType())) {
            r.push_back((T*)e);
        }
        return r;
    }

protected:
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;

	// Gui System
	MyGUI::Gui* mGUI;
	MyGUI::OgrePlatform* mPlatform;

    boost::ptr_vector<Entity> mEntities;
};

#endif
