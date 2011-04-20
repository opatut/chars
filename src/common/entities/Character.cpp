#include "Character.hpp"

#include "common/gamestate/GameState.hpp"
#include "common/util/Utils.hpp"

std::string Character::GetType() {
    return "entity:type:character";
}

void Character::OnSpawn(GameState* state) {
    Ogre::SceneManager* scene_mgr = state->GetSceneMgr();
    mHead = scene_mgr->createEntity( tostr(mUID) + "_head" ,"ogrehead.mesh");
    mHeadNode = scene_mgr->getRootSceneNode()->createChildSceneNode(tostr(mUID) + "_head_node");
    mHeadNode->attachObject(mHead);

    mHeadNode->setScale(0.05, 0.05, 0.05);
    mHeadNode->setPosition(0, 0, 0);
}

void Character::OnDespawn() {
}

void Character::OnUpdate(float time_delta, Input& input) {
    float speed = 15.f;
    if(input.GetKeyboard() != NULL) {
        if(input.GetKeyboard()->isKeyDown(OIS::KC_W)) {
            mHeadNode->translate(speed * time_delta * Ogre::Vector3::UNIT_Z, Ogre::Node::TS_LOCAL);
        }
        if(input.GetKeyboard()->isKeyDown(OIS::KC_S)) {
            mHeadNode->translate(- speed * time_delta * Ogre::Vector3::UNIT_Z, Ogre::Node::TS_LOCAL);
        }
        if(input.GetKeyboard()->isKeyDown(OIS::KC_A)) {
            mHeadNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(+ time_delta * 180));
        }
        if(input.GetKeyboard()->isKeyDown(OIS::KC_D)) {
            mHeadNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(- time_delta * 180));
        }
    }
}

void Character::OnEvent(Event e) {
    if(e.GetIdString() == "input:keyboard:pressed") {
        int key_code = e.ReadData<int>();
        char ch = char(e.ReadData<int>());
        std::cout << "key pressed " << key_code << " / " << ch << std::endl;
    } else if(e.GetIdString() == "input:mouse:moved") {
        /* bool left, right;
        int x,y;
        e.GetData() >> left >> right >> x >> y;
        if(right) {
            float sensitivity = 0.25;
            mCamNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(- x * sensitivity));
            mCamNode->pitch(Ogre::Degree(y * sensitivity));
        } */
    }
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
