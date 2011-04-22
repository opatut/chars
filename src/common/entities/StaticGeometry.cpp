#include "StaticGeometry.hpp"

#include "common/util/Utils.hpp"
#include "common/gamestate/GameState.hpp"

StaticGeometry::StaticGeometry(const std::string& model, Ogre::Vector3 pos) {
    mPosition = pos;
    mModel = model;
    mSceneNode = NULL;
}

std::string StaticGeometry::GetType() {
    return "entity:type:staticgeometry";
}

void StaticGeometry::OnSpawn(GameState *state) {
    Ogre::SceneManager* scene_mgr = state->GetSceneMgr();
    mEntity = scene_mgr->createEntity( tostr(mUID) + "_entity" , mModel);
    mSceneNode = scene_mgr->getRootSceneNode()->createChildSceneNode(tostr(mUID) + "_node");
    mSceneNode->attachObject(mEntity);

    mSceneNode->setPosition(mPosition);
    mEntity->setDebugDisplayEnabled(true);
}

void StaticGeometry::OnDespawn() {
    mGameState->GetSceneMgr()->destroyEntity(mEntity);
    mGameState->GetSceneMgr()->destroySceneNode(mSceneNode);
}

void StaticGeometry::OnEvent(Event e) {
}

void StaticGeometry::OnUpdate(float time_delta, Input &input) {
}

bool StaticGeometry::OwnsObject(Ogre::MovableObject *o) {
    return o == mEntity;
}

void StaticGeometry::SetPosition(Ogre::Vector3 pos) {
    mPosition = pos;
    if(mSceneNode != NULL)
        mSceneNode->setPosition(pos);
}
