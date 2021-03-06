#include "Terrain.hpp"

#include "common/gamestate/GameState.hpp"
#include "common/util/Utils.hpp"
#include "common/util/TerrainHeight.hpp"

std::string Terrain::GetType() {
    return "entity:type:terrain";
}

void Terrain::SetTerrainId(const std::string& id) {
    mTerrainId = id;
}

void Terrain::OnSpawn(GameState* state) {
    Ogre::SceneManager* scene_mgr = state->GetSceneMgr();
    scene_mgr->setFog(Ogre::FOG_LINEAR, state->GetCamera()->getViewport()->getBackgroundColour(), 0, 100.f, 200.f);
    // scene_mgr->setSkyBox(true, "skybox_dusk");

    Ogre::Plane plane;
    plane.d = 25;
    plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
    scene_mgr->setSkyPlane(true, plane, "skyplane_clouds", 400, 1);

    scene_mgr->setWorldGeometry("Terrain_" + mTerrainId + "_config.cfg");


    /*mGeometry = new Forests::PagedGeometry();
    mGeometry->setCamera(state->GetCamera());
    mGeometry->setPageSize(64.f);

    mGeometry->addDetailLevel<Forests::BatchPage>(250, 100);
    mGeometry->addDetailLevel<Forests::ImpostorPage>(600);

	//Create a new TreeLoader3D object
	Forests::TreeLoader3D *treeLoader = new Forests::TreeLoader3D(mGeometry, Forests::TBounds(0, 0, 256, 256));
	mGeometry->setPageLoader(treeLoader);	//Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance

	//Load a tree entity
	Ogre::Entity* myEntity = scene_mgr->createEntity( tostr(mUID) + "_head", "ogrehead.mesh");

	//Setup the height function (so the Y values of trees can be calculated when they are placed on the terrain)
	HeightFunction::initialize(scene_mgr);

	//Randomly place 20,000 copies of the tree on the terrain
	Ogre::Vector3 position;
	Ogre::Radian yaw;
	Ogre::Real scale;
	for (int i = 0; i < 100; i++){
		yaw = Ogre::Degree(Ogre::Math::RangeRandom(0, 360));

		position.x = Ogre::Math::RangeRandom(0, 1024);
		position.z = Ogre::Math::RangeRandom(0, 1024);
		position.y = HeightFunction::getTerrainHeight(position.x, position.z);

		scale = 1;

		treeLoader->addTree(myEntity, position, yaw, scale);
	} */

}

void Terrain::OnDespawn() {
    //mGameState->GetSceneMgr()->setWorldGeometry("");
}

void Terrain::OnUpdate(float time_delta, Input& input) {
    // mGeometry->update();
}

void Terrain::OnEvent(Event e) {

}
