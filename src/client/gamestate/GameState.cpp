#include "GameState.hpp"

#include "common/Logger.hpp"

void GameState::Enable() {
    Logger::GetLogger().Info("Game State" + GetName() + " enabled.");
    OnEnable();
}

void GameState::Disable() {
    Logger::GetLogger().Info("Game State" + GetName() + " disabled.");
    OnDisable();
}

void GameState::LoadResources() {
    OnLoadResources();
}

void GameState::UnloadResources() {
    OnUnloadResources();
}

bool GameState::HandleEvent(Event& e) {
    mPassToNextState = false;
    OnEvent(e);
    return mPassToNextState;
}

bool GameState::Update(float time_delta) {
    mPassToNextState = false;
    OnUpdate(time_delta);
    return mPassToNextState;
}

void GameState::PassToNextState() {
    mPassToNextState = true;
}

int GameState::GetStackLevel() const {
    // TODO!
    return -1;
}
