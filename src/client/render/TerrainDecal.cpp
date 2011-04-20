#include "TerrainDecal.hpp"

TerrainDecal::TerrainDecal() {
    initialized = false;
    mName = "";
    mMaterial = "";
    mSceneMgr = NULL;

    mPos = Ogre::Vector3::ZERO;
    mSize = Ogre::Vector2(1, 1);
    mResolution = Ogre::Vector2(4, 4);
}

void TerrainDecal::Create(Ogre::SceneManager *scene_mgr, const std::string &name, const std::string &material) {
    if(scene_mgr == NULL || name == "" || material == "")
        return;

    initialized = true;
    mSceneMgr = scene_mgr;
    mName = name;
    mMaterial = material;

    mMesh = mSceneMgr->createManualObject(name);
    mSceneMgr->getRootSceneNode()->attachObject(mMesh);

    // number of polygons
    int x_size = round(mResolution.x);
    int z_size = round(mResolution.y);

    mMesh->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
    for (int i=0; i<=x_size; i++) {
        for (int j=0; j<=z_size; j++) {
            mMesh->position(Ogre::Vector3(i, 0, j));
            mMesh->textureCoord((float)i / (float)x_size, (float)j / (float)z_size);
        }
    }

    for (int i=0; i<x_size; i++) {
        for (int j=0; j<z_size; j++) {
            mMesh->quad( i * (x_size+1) + j,
                    i * (x_size+1) + j + 1,
                    (i + 1) * (x_size+1) + j + 1,
                    (i + 1) * (x_size+1) + j);
        }
    }
    mMesh->end();
}

void TerrainDecal::SetPosition(Ogre::Vector3 xz_pos) {
    if(mPos.x != xz_pos.x || mPos.z != xz_pos.z) {
        mPos = xz_pos;
        Update();
    }
}

void TerrainDecal::SetSize(Ogre::Vector2 xz_size) {
    mSize = xz_size;
    Update();
}

void TerrainDecal::SetSize(Ogre::Real size) {
    SetSize(Ogre::Vector2(size, size));
}

void TerrainDecal::SetResolution(Ogre::Vector2 num_polys) {
    mResolution = num_polys;
}

void TerrainDecal::SetResolution(Ogre::Real num_polys) {
    SetResolution(Ogre::Vector2(num_polys, num_polys));
    if(initialized) {
        Remove();
        Create(mSceneMgr, mName, mMaterial);
    }
}

void TerrainDecal::Update() {
    if(!initialized)
        return;

    if(!mMesh->isVisible())
        return;

    Ogre::Real x = mPos.x;
    Ogre::Real z = mPos.z;

    Ogre::Real x_rad = mSize.x / 2;
    Ogre::Real z_rad = mSize.y / 2;

    Ogre::Real x1 = x - x_rad;
    Ogre::Real z1 = z - z_rad;

    // number of polygons
    int x_size = round(mResolution.x);
    int z_size = round(mResolution.y);

    Ogre::Real x_step = mSize.x / x_size;
    Ogre::Real z_step = mSize.y / z_size;

    mMesh->beginUpdate(0);
    // redefine vertices
    for (int i=0; i<=x_size; i++) {
        for (int j=0; j<=z_size; j++) {
            mMesh->position(Ogre::Vector3(x1, GetTerrainHeight(x1, z1) + 1, z1));
            mMesh->normal(0, 1, 0);
            mMesh->textureCoord((float)i / (float)x_size, (float)j / (float)z_size);
            z1 += z_step;
        }
        x1 += x_step;
        z1 = z - z_rad;
    }

    // redefine quads
    for (int i=0; i<x_size; i++) {
        for (int j=0; j<z_size; j++) {
            mMesh->quad( i * (x_size+1) + j,
                    i * (x_size+1) + j + 1,
                    (i + 1) * (x_size+1) + j + 1,
                    (i + 1) * (x_size+1) + j);
        }
    }

    mMesh->end();
}

void TerrainDecal::Remove() {
    if(initialized && mSceneMgr->hasManualObject(mName))
        mSceneMgr->destroyManualObject(mMesh);
}


Ogre::Real TerrainDecal::GetTerrainHeight(Ogre::Real x, Ogre::Real z) {
    HeightFunction::initialize(mSceneMgr);
    return HeightFunction::getTerrainHeight(x, z);
}

void TerrainDecal::Show() {
    mMesh->setVisible(true);
    Update();
}

void TerrainDecal::Hide() {
    mMesh->setVisible(false);
}
