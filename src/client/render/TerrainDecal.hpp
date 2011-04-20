#ifndef RENDER_TERRAINDECAL_HPP
#define RENDER_TERRAINDECAL_HPP

#include <Ogre.h>
#include "common/util/TerrainHeight.hpp"

class TerrainDecal {
public:
    TerrainDecal();

    void SetPosition(Ogre::Vector3 xz_pos);
    void SetSize(Ogre::Vector2 xz_size);
    void SetSize(Ogre::Real size);
    void SetResolution(Ogre::Vector2 num_polys);
    void SetResolution(Ogre::Real num_polys);

    void Create(Ogre::SceneManager* scene_mgr, const std::string& name, const std::string& material);
    void Update();
    void Remove();

    void Show();
    void Hide();
private:
    bool initialized;
    std::string mName, mMaterial;

    Ogre::SceneManager* mSceneMgr;
    Ogre::ManualObject* mMesh;
    Ogre::Vector3 mPos;
    Ogre::Vector2 mSize;
    Ogre::Vector2 mResolution;

    Ogre::Real GetTerrainHeight(Ogre::Real x, Ogre::Real z);
};

#endif
