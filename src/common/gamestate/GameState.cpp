#include "GameState.hpp"

#include "common/log/Logger.hpp"

Ogre::SceneManager* GameState::GetSceneMgr() {
    return mSceneMgr;
}

Ogre::Camera* GameState::GetCamera() {
    return mCamera;
}

void GameState::Enable() {
    Logger::GetLogger().Info("Game State" + GetName() + " enabled.");
    OnEnable();
    InitializeGUI();
}

void GameState::Disable() {
    Logger::GetLogger().Info("Game State" + GetName() + " disabled.");
    DeinitializeGUI();
    OnDisable();
}

void GameState::LoadResources() {
    OnLoadResources();
}

void GameState::UnloadResources() {
    OnUnloadResources();
}

void GameState::InitializeGUI() {
    OnInitializeGUI();
}

void GameState::DeinitializeGUI() {
    OnDeinitializeGUI();
}

bool GameState::HandleEvent(Event e) {
    // pass event to all entities
    for(auto iter = mEntities.begin(); iter != mEntities.end(); ++iter) {
        iter->HandleEvent(e);
    }

    // event callback
    mPassToNextState = false;
    OnEvent(e);
    return mPassToNextState;
}

bool GameState::Update(float time_delta, Input& input) {
    // remove all entities marked for removal
    for(auto iter = mEntities.begin(); iter != mEntities.end(); ++iter) {
        iter->Despawn();
    }
    mEntities.erase_if(boost::bind(&Entity::IsMarkedForRemoval, _1)); // awww yeahh! erase_if ftw.

    // update all entities
    for(auto iter = mEntities.begin(); iter != mEntities.end(); ++iter) {
        iter->Update(time_delta, input);
    }

    // update callback
    mPassToNextState = false;
    OnUpdate(time_delta, input);
    return mPassToNextState;
}

void GameState::AddEntity(Entity* e) {
    mEntities.push_back(e);
    mEntities.back().Spawn(this);
}

void GameState::RemoveEntity(Entity* e) {
    e->MarkForRemoval();
}

void GameState::RemoveEntity(sf::Uint32 uid) {
    Entity* e = GetEntity(uid);
    if(e != NULL) {
        e->MarkForRemoval();
    }
}

Entity* GameState::GetEntity(sf::Uint32 uid) {
    for(auto iter = mEntities.begin(); iter != mEntities.end(); ++iter) {
        if(iter->GetUID() == uid)
            return &(*iter);
    }
    return NULL;
}

std::vector<Entity*> GameState::GetEntitiesByType(const std::string& type) {
    std::vector<Entity*> r;
    BOOST_FOREACH(Entity& e, mEntities) {
        if(e.GetType() == type)
            r.push_back(&e);
    }
    return r;
}

void GameState::PassToNextState() {
    mPassToNextState = true;
}

MyGUI::Gui* GameState::GetGUI() {
    return mGUI;
}

bool GameState::IsGUIFocused() {
    return (MyGUI::InputManager::getInstance().getKeyFocusWidget() != NULL);
}

bool GameState::IsKeyDown(Input& input, OIS::KeyCode key) {
    if(input.GetKeyboard() == NULL)
        return false;
    if(IsGUIFocused())
        return false;
    return input.GetKeyboard()->isKeyDown(key);
}
