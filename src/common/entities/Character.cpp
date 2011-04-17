#include "Character.hpp"

#include "common/gamestate/GameState.hpp"
#include "common/util/Utils.hpp"

void Character::OnSpawn(GameState* state) {
    Ogre::SceneManager* scene_mgr = state->GetSceneMgr();
    mHead = scene_mgr->createEntity( tostr(mUID) + "_head" ,"ogrehead.mesh");
    mHeadNode = scene_mgr->getRootSceneNode()->createChildSceneNode(tostr(mUID) + "_head_node");
    mHeadNode->attachObject(mHead);

    mHeadNode->setScale(0.05, 0.05, 0.05);
    mHeadNode->setPosition(0, 0, 0);
    mVelocity = Ogre::Vector3::ZERO;

    mCamNode = mHeadNode->createChildSceneNode(tostr(mUID) + "_cam_node");
    mCamNode->setPosition(0, 0, 0);

    Ogre::Camera* cam = mGameState->GetCamera();
    cam->lookAt(0,0,0);
    cam->setPosition(0,8, -16);
    mCamNode->attachObject(cam);
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

    mVelocity.y -= 0.2 * time_delta;

    mHeadNode->translate(mVelocity, Ogre::Node::TS_PARENT);
    if(mHeadNode->getPosition().y <= 0)  {
        mVelocity.y = 0;
        mHeadNode->setPosition(mHeadNode->getPosition().x, 0, mHeadNode->getPosition().z);
    }

    float scroll = input.GetMouse()->getMouseState().Z.rel / 120.f;
    if(mGameState->GetCamera()->getPosition().length() > Ogre::Math::Abs(scroll) * 5 || scroll < 0) {
        mGameState->GetCamera()->moveRelative(- Ogre::Vector3::UNIT_Z * scroll);
    }

    // follow head
    mGameState->GetCamera()->lookAt(mHeadNode->_getDerivedPosition());
}

void Character::OnEvent(Event& e) {
    if(e.GetIdString() == "key_pressed") {
        int key_code = e.ReadData<int>();
        std::cout << "key pressed " << key_code << std::endl;
        switch(key_code) {
        case OIS::KC_SPACE:
            mVelocity.y += 0.15;
        }
    } else if(e.GetIdString() == "mouse_moved") {
        bool left, right;
        int x,y;
        e.GetData() >> left >> right >> x >> y;
        if(right) {
            float sensitivity = 0.25;
            mCamNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(- x * sensitivity));
            mCamNode->pitch(Ogre::Degree(y * sensitivity));
        }
    }
}

sf::Uint16 Character::GetPlayerID() const {
    return mPlayerID;
}

void Character::SetPlayerID(sf::Uint16 id) {
    mPlayerID = id;
}
