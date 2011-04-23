#ifndef GAMESTATE_GAMESTATEMANAGER_HPP
#define GAMESTATE_GAMESTATEMANAGER_HPP

#include <boost/shared_ptr.hpp>

#include "GameState.hpp"

class GameStateManager {
public:
    GameStateManager();

    void PushState();
    void SetNewState(GameState* new_state);
    GameState* GetCurrentState();

    void Update(float time_delta, Input& input);
    void HandleEvent(Event e);
private:
    boost::shared_ptr<GameState> mCurrentState;
    boost::shared_ptr<GameState> mNewState;
};

#endif
