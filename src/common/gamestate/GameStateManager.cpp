#include "GameStateManager.hpp"

GameStateManager::GameStateManager() {
    mNewState = boost::shared_ptr<GameState>();
    mCurrentState = boost::shared_ptr<GameState>();
}

void GameStateManager::PushState() {
    if(mNewState.get() != NULL) {
        if(mCurrentState.get() != NULL) {
            mCurrentState.get()->DeinitializeGUI();
            mCurrentState.get()->Disable();
            mCurrentState.get()->UnloadResources();
        }
        mCurrentState = mNewState;
        mCurrentState.get()->LoadResources();
        mCurrentState.get()->Enable();
        mCurrentState.get()->InitializeGUI();

        mNewState = boost::shared_ptr<GameState>();
    }
}

void GameStateManager::SetNewState(GameState* new_state) {
    mNewState = boost::shared_ptr<GameState>(new_state);
}

GameState* GameStateManager::GetCurrentState() {
    return mCurrentState.get();
}

void GameStateManager::Update(float time_delta, Input& input) {
    if(mCurrentState.get() == NULL)
        return;

    mCurrentState.get()->Update(time_delta, input);
}

void GameStateManager::HandleEvent(Event e) {
    if(mCurrentState.get() == NULL)
        return;

    mCurrentState.get()->HandleEvent(e);
}
