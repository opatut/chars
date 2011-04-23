#include "LoginState.hpp"

#include "client/Client.hpp"

LoginState::LoginState() {

}

std::string LoginState::GetName() const {
    return "LoginState";
}

void LoginState::OnEnable() {
    Ogre::Root* r = Client::get_mutable_instance().GetOgreRoot();
    Ogre::RenderWindow* w = r->getAutoCreatedWindow();

	mSceneMgr = r->createSceneManager(Ogre::ST_EXTERIOR_CLOSE, GetName() + "_SceneManager");
	// Create the camera and a cam node
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50000);

	Ogre::Viewport* vp = w->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0.47f, 0.67f, 0.96f));
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	if (r->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
		mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can

}

void LoginState::OnDisable() {
    Ogre::Root* r = Client::get_mutable_instance().GetOgreRoot();
    Ogre::RenderWindow* w = Client::get_mutable_instance().GetWindow();
    w->removeAllViewports();
    mSceneMgr->clearScene();
    r->destroySceneManager(mSceneMgr);
}

void LoginState::OnLoadResources() {}

void LoginState::OnUnloadResources() {}

void LoginState::OnInitializeGUI() {
	Ogre::RenderWindow* w = Client::get_mutable_instance().GetWindow();

	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(w, mSceneMgr, "GUI");
	mGUI = new MyGUI::Gui();
	mGUI->initialise();

	MyGUI::LayoutManager::getInstance().loadLayout("login.layout");
	mGUI->findWidget<MyGUI::Button>("button:close")->eventMouseButtonClick = MyGUI::newDelegate(this, &LoginState::QuitButton);
	mGUI->findWidget<MyGUI::Button>("button:login")->eventMouseButtonClick = MyGUI::newDelegate(this, &LoginState::LoginButton);

	mGUI->resizeWindow(MyGUI::IntSize(w->getWidth(), w->getHeight()));
}

void LoginState::OnDeinitializeGUI() {
	mGUI->shutdown();
	delete mGUI;
	mGUI = 0;
	mPlatform->shutdown();
	delete mPlatform;
	mPlatform = 0;
}

void LoginState::OnEvent(Event e) {}

void LoginState::OnUpdate(float time_delta, Input& input) {}

void LoginState::LoginButton(MyGUI::WidgetPtr _sender) {
	Client::get_mutable_instance().GetGameStateManager().SetNewState(new MainGameState());
}

void LoginState::QuitButton(MyGUI::WidgetPtr _sender) {
	Client::get_mutable_instance().RequestShutdown();
}

