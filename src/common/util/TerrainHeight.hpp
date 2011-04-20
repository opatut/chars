//This provides functions that can be used to easily get the height of Ogre's terrain at any x/z point.
//Simply call HeightFunction::initialize(), then use HeightFunction::getTerrainHeight() as needed.
//This file is used by the PagedGeometry examples to place trees on the terrain.

#ifndef RENDER_TERRAINHEIGHT_HPP
#define RENDER_TERRAINHEIGHT_HPP

#include "Ogre.h"

class MyRaySceneQueryListener: public Ogre::RaySceneQueryListener {
public:
	bool queryResult(Ogre::SceneQuery::WorldFragment *fragment, Ogre::Real distance);
	bool queryResult(Ogre::MovableObject *obj, Ogre::Real distance);
	float resultDistance;
};


class HeightFunction {
public:
	//Initializes the height function. Call this before calling getTerrainHeight()
	static void initialize(Ogre::SceneManager *sceneMgr);

	//Gets the height of the terrain at the specified x/z coordinate
	//The userData parameter isn't used in this implementation of a height function, since
	//there's no need for extra data other than the x/z coordinates.
	static float getTerrainHeight(const float x, const float z, void *userData = NULL);

	static bool initialized;
	static Ogre::RaySceneQuery* raySceneQuery;
	static Ogre::Ray updateRay;
	static MyRaySceneQueryListener *raySceneQueryListener;
};


#endif
