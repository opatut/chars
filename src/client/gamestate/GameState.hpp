#ifndef GAMESTATE_GAMESTATE_HPP
#define GAMESTATE_GAMESTATE_HPP

#include <string>
#include <iostream>

#include <OIS.h>

#include "common/events/Event.hpp"

class GameState {
/* public:
    enum TransitionState {
        HIDDEN,
        FADE_IN,
        SHOWN,
        FADE_OUT
    }; */

public:
    GameState() {}
    virtual const std::string& GetName() = 0;

    virtual void OnEnable() = 0;
    virtual void OnDisable() = 0;
    virtual void OnLoadResources() {}
    virtual void OnUnloadResources() {}
    virtual void OnEvent(Event& e) {}
    virtual void OnUpdate(float time_delta) = 0;

    void Enable();
    void Disable();
    void LoadResources();
    void UnloadResources();
    bool HandleEvent(Event& e);
    bool Update(float time_delta);

    void PassToNextState();
protected:
    bool mPassToNextState;

    int GetStackLevel() const;

    /*  */
};

#endif
