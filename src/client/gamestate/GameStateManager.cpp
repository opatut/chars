#include "GameStateManager.hpp"

GameStateManager::GameStateManager() {}

void GameStateManager::Add(GameState* new_state) {
    new_state->LoadResources();
    std::cout << "#### ENABLING " << new_state->GetName() << std::endl;
    new_state->Enable();
    mStack.push_back(new_state);
}

void GameStateManager::Pop(int num) {
    mPop += num;
}

GameState& GameStateManager::GetCurrentState() {
    return mStack.back();
}

int GameStateManager::GetStateLevel(GameState* state) {
    int i = 0;
    for(auto iter = mStack.rbegin(); iter != mStack.rend(); ++iter) {
        if(iter->GetName() == state->GetName())
            return i;
        ++i;
    }
    return -1;
}

void GameStateManager::Update(float time_delta) {
    for(auto iter = mStack.rbegin(); iter != mStack.rend(); ++iter) {
        if(! iter->Update(time_delta))
            break;
    }

    // Pop amount of states requested
    for(; mPop > 0 && mStack.size() > 0; --mPop) {
        // pop current state
        GetCurrentState().Disable();
        GetCurrentState().UnloadResources();
        mStack.pop_back();
    }
    mPop = 0;
}

void GameStateManager::HandleEvent(Event& e) {
    for(auto iter = mStack.rbegin(); iter != mStack.rend(); ++iter) {
        if(! iter->HandleEvent(e))
            break;
    }
}