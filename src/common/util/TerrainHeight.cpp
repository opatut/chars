#include "TerrainHeight.hpp"

bool HeightFunction::initialized = false;
Ogre::RaySceneQuery* HeightFunction::raySceneQuery;
Ogre::Ray HeightFunction::updateRay;
MyRaySceneQueryListener* HeightFunction::raySceneQueryListener;

bool MyRaySceneQueryListener::queryResult(Ogre::SceneQuery::WorldFragment *fragment, Ogre::Real distance) {
	resultDistance = distance;
	return false;
}
bool MyRaySceneQueryListener::queryResult(Ogre::MovableObject *obj, Ogre::Real distance) {
	resultDistance = distance;
	return false;
}

void HeightFunction::initialize(Ogre::SceneManager *sceneMgr) {
	if(!initialized) {
		initialized = true;
		updateRay.setOrigin(Ogre::Vector3::ZERO);
		updateRay.setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);
		raySceneQuery = sceneMgr->createRayQuery(updateRay);
		raySceneQuery->setQueryTypeMask(Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
		raySceneQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
		raySceneQueryListener = new MyRaySceneQueryListener;
	}
}

float HeightFunction::getTerrainHeight(const float x, const float z, void *userData) {
	updateRay.setOrigin(Ogre::Vector3(x, 0.0f, z));
	updateRay.setDirection(Ogre::Vector3::UNIT_Y);
	raySceneQuery->setRay(updateRay);
	raySceneQuery->execute(raySceneQueryListener);

	return raySceneQueryListener->resultDistance;
}
