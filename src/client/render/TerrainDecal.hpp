/**
  * This class is copied from the Ogre wiki:
  * http://www.ogre3d.org/tikiwiki/Projective+Decals
  * as of 19/04/11
  * thanks, JustBoo, Soldans and jacmoe!
  */
#ifndef RENDER_TERRAINDECAL_HPP
#define RENDER_TERRAINDECAL_HPP

#include <map>
#include <string>

#include <Ogre.h>

class TerrainDecal {
public:
    TerrainDecal();
    ~TerrainDecal();

    void init(Ogre::SceneManager* man, Ogre::Vector2 size, std::string tex);
    void show();
    void hide();
    void updatePosition(Ogre::Vector3 pos);
    void updateSize(Ogre::Vector2 size);

    bool isVisible();
protected:
    Ogre::Vector3 mPos;
    Ogre::Vector2 mSize;
    bool mVisible;
    std::string mTexture;

    Ogre::SceneNode* mNode;
    Ogre::Frustum* mFrustum;
    Ogre::SceneManager* mSceneManager;

    // info about materials that are receiving the decal
    std::map<std::string,Ogre::Pass*> mTargets;

    bool isPosOnTerrain(Ogre::Vector3 pos);
    std::string getMaterialAtPosition(Ogre::Vector3 pos);

    void addMaterial(std::string matName);
    std::map<std::string,Ogre::Pass*>::iterator removeMaterial(std::string matName);
};

#endif
