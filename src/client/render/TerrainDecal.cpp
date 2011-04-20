#include "TerrainDecal.hpp"

TerrainDecal::TerrainDecal() {
    mVisible = false;
    mNode = 0;
    mFrustum = 0;
}

TerrainDecal::~TerrainDecal() {
    hide();

    // delete frustum
    mNode->detachAllObjects();
    delete mFrustum;

    // destroy node
    mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());
}

void TerrainDecal::init(Ogre::SceneManager* man, Ogre::Vector2 size, std::string tex) {
    using namespace Ogre;

    // set SM
    mSceneManager = man;

    // init projective decal
    // set up the main decal projection frustum
    mFrustum = new Ogre::Frustum();
    mNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    mNode->attachObject(mFrustum);
    mFrustum->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    mNode->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));

    // set given values
    updateSize(size);
    mTexture = tex;        // texture to apply

    // load the images for the decal and the filter
    TextureManager::getSingleton().load
        (mTexture, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 1);

    mVisible = false;
}

void TerrainDecal::show() {
    if(!mVisible) {
        mVisible = true;
        updatePosition(mPos);
    }
}

void TerrainDecal::hide() {
    if(mVisible) {
        // remove all added passes
        while( !mTargets.empty() )
            removeMaterial(mTargets.begin()->first);
        mVisible = false;
    }
}

void TerrainDecal::updatePosition(Ogre::Vector3 pos) {
    // don`t do anything if pos didn`t change
    if(pos == mPos)
        return;

    // save the new position
    mPos = pos;
    mNode->setPosition(pos.x,pos.y+1000,pos.z);

    // don`t show if invisible
    if(!isVisible())
        return;

    // check near pages (up to 4)
    std::list<std::string> neededMaterials;
    Ogre::Vector3 t;

    // x high        z high
    t = Ogre::Vector3(mPos.x+mSize.x/2.0f,1000,mPos.z+mSize.y/2.0f);
    neededMaterials.push_back(getMaterialAtPosition(t));


    // x high        z low
    t = Ogre::Vector3(mPos.x+mSize.x/2.0f,1000,mPos.z-mSize.y/2.0f);
    neededMaterials.push_back(getMaterialAtPosition(t));

    // x low        z low
    t = Ogre::Vector3(mPos.x-mSize.x/2.0f,1000,mPos.z-mSize.y/2.0f);
    neededMaterials.push_back(getMaterialAtPosition(t));

    // x low        z high
    t = Ogre::Vector3(mPos.x-mSize.x/2.0f,1000,mPos.z+mSize.y/2.0f);
    neededMaterials.push_back(getMaterialAtPosition(t));

    // remove empties
    neededMaterials.remove("");

    // remove doubles
    neededMaterials.unique();

    // compare needed materials with used

    // for every used material
    std::map<std::string,Ogre::Pass*>::iterator used = mTargets.begin();
    while(true) {
        // stop if we are through
        if(used == mTargets.end())
            break;

        // find in needed
        std::list<std::string>::iterator needed =
            std::find(neededMaterials.begin(),neededMaterials.end(),used->first);

        if(needed == neededMaterials.end()) {
            // material is not needed any longer, so remove it
            used = removeMaterial(used->first);
        } else {
            // remove it from needed list, bedause it is already loaded
            neededMaterials.remove(used->first);

            // go further
            used++;
        }
    }

    // add all remaining needed to used list
    while(!neededMaterials.empty()) {
        addMaterial(neededMaterials.front());
        neededMaterials.erase(neededMaterials.begin());
    }
}

void TerrainDecal::updateSize(Ogre::Vector2 size) {
    if(mSize != size) {
        // save param
        mSize = size;

        // update aspect ratio
        mFrustum->setAspectRatio(mSize.x/mSize.y);

        // update height
        mFrustum->setOrthoWindowHeight(mSize.y);
    }
}

bool TerrainDecal::isVisible() {
    return mVisible;
}


bool TerrainDecal::isPosOnTerrain(Ogre::Vector3 pos) {
    // get the terrain boundaries
    int w = 0;
    int h = 0;
    mSceneManager->getOption("Width",&w);
    mSceneManager->getOption("Height",&h);

    std::cout << w << " x " << h << std::endl;
    return (pos.x >= 0 && pos.x <= w && pos.z >= 0 && pos.z <= h);
}


std::string TerrainDecal::getMaterialAtPosition(Ogre::Vector3 pos) {
    void* myOptionPtr = &pos;

    // check if position is on battlefield
    if(isPosOnTerrain(pos)) {
        mSceneManager->getOption ("getMaterialPageName", &myOptionPtr);
        std::string name = *static_cast<std::string*>(myOptionPtr);
        std::cout << "name: " << name << std::endl;
        return name;
    }
    return "";
}

void TerrainDecal::addMaterial(std::string matName) {
    // check if material is already decalled
    if(mTargets.find(matName) != mTargets.end()) {
        Ogre::LogManager::getSingleton().getDefaultLog()->logMessage("material should be added to decal but was already!");
        return;
    }

    using namespace Ogre;

    // get the material ptr
    MaterialPtr mat = (MaterialPtr)MaterialManager::getSingleton().getByName(matName);

    // create a new pass in the material to render the decal
    Pass* pass = mat->getTechnique(0)->createPass();

    // set up the decal's texture unit
    TextureUnitState *texState = pass->createTextureUnitState(mTexture);
    texState->setProjectiveTexturing(true, mFrustum);
    texState->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
    texState->setTextureFiltering(FO_POINT, FO_LINEAR, FO_NONE);

    // set our pass to blend the decal over the model's regular texture
    pass->setSceneBlending(SBT_TRANSPARENT_ALPHA);
    pass->setDepthBias(1);

    // set the decal to be self illuminated instead of lit by scene lighting
    pass->setLightingEnabled(false);

    // save pass in map
    mTargets[matName] = pass;

    LogManager::getSingleton().getDefaultLog()->logMessage(std::string("added material to decal: ") + matName +
        "(" + Ogre::StringConverter::toString(mTargets.size()) + " materials loaded)");
}

std::map<std::string,Ogre::Pass*>::iterator TerrainDecal::removeMaterial(std::string matName) {
    // remove our pass from the given material
    mTargets[matName]->getParent()->removePass(mTargets[matName]->getIndex());

    Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(std::string("removed material from decal: ") + matName +
        "(" + Ogre::StringConverter::toString(mTargets.size()-1) + " materials loaded)");

    // remove from map
    return mTargets.erase(mTargets.find(matName));
}
