#include "Terrain.hpp"

#include "common/gamestate/GameState.hpp"

void Terrain::OnSpawn(GameState* state) {
    Ogre::SceneManager* scene_mgr = state->GetSceneMgr();

    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -1);
    Ogre::MeshManager::getSingleton().createPlane("groundplane",
                                                  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                  plane, 512, 512, 20,20, true, 1, 5, 5,
                                                  Ogre::Vector3::UNIT_Z);

    mTerrain = scene_mgr->createEntity("GroundEntity", "groundplane");
    scene_mgr->getRootSceneNode()->createChildSceneNode()->attachObject(mTerrain);
    mTerrain->setMaterialName("RockTerrain");
    mTerrain->setCastShadows(false);
}

void Terrain::OnDespawn() {

}

void Terrain::OnUpdate(float time_delta, Input& input) {

}

void Terrain::OnEvent(Event& e) {

}
