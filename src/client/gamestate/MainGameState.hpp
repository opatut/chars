#ifndef GAMESTATE_MAINGAMESTATE_HPP
#define GAMESTATE_MAINGAMESTATE_HPP

#include "GameState.hpp"

#include <Ogre.h>

class MainGameState : public GameState {
public:
    MainGameState();
    const std::string& GetName();
    void OnEnable();
    void OnDisable();
    void OnLoadResources();
    void OnUnloadResources();
    void OnEvent(Event& e);
    void OnUpdate(float time_delta);
private:
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;
};

#endif
