#include "MainGameState.hpp"

#include "client/Client.hpp"

MainGameState::MainGameState() {
    std::cout << "Creating MainGameState object (constructor) " << std::endl;
}

std::string MainGameState::GetName() const {
    return "MainGameState";
}

void MainGameState::OnEnable() {
    Ogre::Root* r = Client::get_mutable_instance().GetOgreRoot();
    Ogre::RenderWindow* w = r->getAutoCreatedWindow();

    mSceneMgr = r->createSceneManager("DefaultSceneManager", GetName() + "_SceneManager");

    /*Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");

    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headNode->attachObject(ogreHead); */

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);

	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	// Create one viewport, entire window

	Ogre::Viewport* vp = w->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	mCamera->setPosition(Ogre::Vector3(0, 5, 0));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50000);

	if (r->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
		mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can

	// create character entity
	Character* c = new Character();
	c->GrabUID();
	AddEntity(c);

	Terrain* t = new Terrain();
	t->GrabUID();
	AddEntity(t);
}

void MainGameState::OnDisable() {

}

void MainGameState::OnLoadResources() {
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Test");
}

void MainGameState::OnUnloadResources() {
	Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup("Test");
}

void MainGameState::OnEvent(Event& e) {
    PassToNextState();
}

void MainGameState::OnUpdate(float time_delta, Input& input) {
    PassToNextState();
}
