#include "Character.hpp"

#include "common/gamestate/GameState.hpp"
#include "common/util/Utils.hpp"

std::string Character::GetType() {
    return "entity:type:character";
}

void Character::OnSpawn(GameState* state) {
    Ogre::SceneManager* scene_mgr = state->GetSceneMgr();
    mHead = scene_mgr->createEntity( tostr(mUID) + "_head" ,"Buildings.Well1.mesh");
    mHeadNode = scene_mgr->getRootSceneNode()->createChildSceneNode(tostr(mUID) + "_head_node");
    mHeadNode->attachObject(mHead);

    mHeadNode->setPosition(0, 0, 0);
}

void Character::OnDespawn() {
}

void Character::OnUpdate(float time_delta, Input& input) {
}

void Character::OnEvent(Event e) {
}

sf::Uint16 Character::GetPlayerID() const {
    return mPlayerID;
}

void Character::SetPlayerID(sf::Uint16 id) {
    mPlayerID = id;
}

void Character::SetPosition(Ogre::Vector3 pos) {
    mHeadNode->setPosition(pos);
}
