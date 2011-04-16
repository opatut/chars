#include "MainGameState.hpp"

#include "client/Client.hpp"

MainGameState::MainGameState() {
    std::cout << "Creating MainGameState object (constructor) " << std::endl;
}

std::string MainGameState::GetName() {
    return "MainGameState";
}

void MainGameState::OnEnable() {
    Ogre::Root* r = Client::get_mutable_instance().GetOgreRoot();
    Ogre::RenderWindow* w = r->getAutoCreatedWindow();

    mSceneMgr = r->createSceneManager("DefaultSceneManager", GetName() + "_SceneManager");

    Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");

    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headNode->attachObject(ogreHead);

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

	mCamera->setPosition(Ogre::Vector3(50, 0, 0));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50000);
}

void MainGameState::OnDisable() {

}

void MainGameState::OnLoadResources() {

}

void MainGameState::OnUnloadResources() {

}

void MainGameState::OnEvent(Event& e) {
    PassToNextState();
}

void MainGameState::OnUpdate(float time_delta) {
    PassToNextState();
}
