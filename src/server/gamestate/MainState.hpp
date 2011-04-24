#ifndef GAMESTATE_MAINSTATE_HPP
#define GAMESTATE_MAINSTATE_HPP

#include <string>

#include "server/login/LoginManager.hpp"
#include "common/events/Event.hpp"
#include "common/network/Request.hpp"
#include "common/network/LoginRequest.hpp"
#include "common/network/NetworkManager.hpp"
#include "common/gamestate/GameState.hpp"

class MainState : public GameState {
public:
    MainState();
    std::string GetName() const;
    void OnEnable();
    void OnDisable();
    void OnLoadResources();
    void OnUnloadResources();
    void OnInitializeGUI();
    void OnDeinitializeGUI();
    void OnEvent(Event e);
    void OnUpdate(float time_delta, Input& input);

private:
    LoginManager mLoginManager;
};

#endif
