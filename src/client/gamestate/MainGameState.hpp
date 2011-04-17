#ifndef GAMESTATE_MAINGAMESTATE_HPP
#define GAMESTATE_MAINGAMESTATE_HPP

#include "common/gamestate/GameState.hpp"
#include "common/entities/Character.hpp"
#include "common/entities/Terrain.hpp"

#include <Ogre.h>

class MainGameState : public GameState {
public:
    MainGameState();
    std::string GetName() const;
    void OnEnable();
    void OnDisable();
    void OnLoadResources();
    void OnUnloadResources();
    void OnEvent(Event& e);
    void OnUpdate(float time_delta, Input& input);
};

#endif
