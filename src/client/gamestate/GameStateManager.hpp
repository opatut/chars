#ifndef GAMESTATE_GAMESTATEMANAGER_HPP
#define GAMESTATE_GAMESTATEMANAGER_HPP

#include <boost/ptr_container/ptr_vector.hpp>

#include "GameState.hpp"

class GameStateManager {
public:
    GameStateManager();

    void Add(GameState* new_state);
    void Pop(int num = 1);

    GameState& GetCurrentState();
    int GetStateLevel(GameState* state);

    void Update(float time_delta);
    void HandleEvent(Event& e);
private:
    boost::ptr_vector<GameState> mStack; // back = current
    int mPop;
};

#endif
