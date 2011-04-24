#include "Client.hpp"

#include "common/util/Utils.hpp"

Client::Client() {
    mOgreRoot = NULL;
}

void Client::Go() {
    Logger::GetLogger().Info("--- Loading config ---");
    LoadConfig();

    Logger::GetLogger().Info("--- Initializing Network ---");
    if(!InitializeNetwork())
        return;

    Logger::GetLogger().Info("--- Starting render engine ---");
    StartupOgre();

    Logger::GetLogger().Info("--- Starting main loop ---");
    RunLoop();

    Logger::GetLogger().Info("--- Stopping ---");
    Shutdown();
}

void Client::LoadConfig() {
    mConfiguration.SetFile("../data/config/config.yml");
    mConfiguration.Load();
}

bool Client::InitializeNetwork() {
    NetworkManager::get_mutable_instance().SetMode(NetworkManager::MODE_CLIENT);
    NetworkManager::get_mutable_instance().SetListener(this);

    return NetworkManager::get_mutable_instance().Connect(Recipient(sf::IpAddress::GetLocalAddress(), 25567));
}

void Client::StartupOgre() {
	Ogre::LogManager* logMgr = OGRE_NEW Ogre::LogManager;
	logMgr->createLog("DefaultLog", true, false, false);

	mOgreRoot = new Ogre::Root("../data/config/plugins.cfg");

	// setup resources
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load("../data/config/resources.cfg");

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String secName, typeName, archName;
	while(seci.hasMoreElements()) {
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for(i = settings->begin(); i != settings->end(); ++i) {
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}

	// configure
	// Show the configuration dialog and initialise the system
	if(!(mOgreRoot->restoreConfig() || mOgreRoot->showConfigDialog())) {
		exit(0);
	}
	mWindow = mOgreRoot->initialise(true, "Client Window");


	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	// initialise all resource groups
	// Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Basics");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("GUI");

	InitializeWindow();
	// ogre loaded
}

void Client::InitializeWindow() {
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
#ifdef DEBUG
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
#endif

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	// Register for callback
	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);
	//mMouse->setBuffered(true);

	// Set initial mouse clipping size
	windowResized(mWindow);

	// Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	// Register as Frame Listener
	mOgreRoot->addFrameListener(this);
}

void Client::RunLoop() {
	// TODO: extract
	// create main state
	mGameStateManager.SetNewState(new LoginState());

	mRunning = true;
	mOgreRoot->startRendering();
}

void Client::Shutdown() {
	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mOgreRoot;
}

void Client::SaveConfig() {
    mConfiguration.Save();
}

void Client::HandleEvent(Event e) {
    mGameStateManager.HandleEvent(e);
}

void Client::windowResized(Ogre::RenderWindow* rw) {
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;

	Event e("window:resized");
	e.WriteData(width);
	e.WriteData(height);
	e.WriteData(depth);
	e.WriteData(left);
	e.WriteData(top);
	HandleEvent(e);
}

void Client::windowClosed(Ogre::RenderWindow* rw) {
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow ) {
		if( mInputManager ) {
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}

}

bool Client::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	if(mWindow->isClosed())
		return false;

	mGameStateManager.PushState();

	// Network
	NetworkManager::get_mutable_instance().HandleIncomingRequests();
	NetworkManager::get_mutable_instance().SendQueuedRequests();

	// Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	Input in(mMouse, mKeyboard);
	mGameStateManager.Update(evt.timeSinceLastFrame, in);

	return mRunning;
}

// == OIS INPUT ==
bool Client::mouseMoved(const OIS::MouseEvent &arg) {
    if(!mGameStateManager.GetCurrentState()->GetGUI()->injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs)) {
        Event e("input:mouse:moved");
        e.WriteData(arg.state.buttonDown(OIS::MB_Left));
        e.WriteData(arg.state.buttonDown(OIS::MB_Right));
        e.WriteData(arg.state.X.rel);
        e.WriteData(arg.state.Y.rel);
        HandleEvent(e);
    }
}

bool Client::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    if(!mGameStateManager.GetCurrentState()->GetGUI()->injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id))) {
        Event e("input:mouse:pressed");
        e.WriteData(arg.state.buttonDown(OIS::MB_Left));
        e.WriteData(arg.state.buttonDown(OIS::MB_Right));
        e.WriteData(arg.state.X.abs);
        e.WriteData(arg.state.Y.abs);
        HandleEvent(e);
    }
}

bool Client::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    if(!mGameStateManager.GetCurrentState()->GetGUI()->injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id))) {
        Event e("input:mouse:released");
        e.WriteData(arg.state.buttonDown(OIS::MB_Left));
        e.WriteData(arg.state.buttonDown(OIS::MB_Right));
        e.WriteData(arg.state.X.abs);
        e.WriteData(arg.state.Y.abs);
        HandleEvent(e);
    }
}

bool Client::keyPressed(const OIS::KeyEvent &arg) {
    if(! mGameStateManager.GetCurrentState()->GetGUI()->injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text)) {
        Event e("input:keyboard:pressed");
        e.WriteData(arg.key);
        e.WriteData(arg.text);
        HandleEvent(e);
    }
}

bool Client::keyReleased(const OIS::KeyEvent &arg) {
    if(!mGameStateManager.GetCurrentState()->GetGUI()->injectKeyRelease(MyGUI::KeyCode::Enum(arg.key))) {
        Event e("input:keyboard:released");
        e.WriteData(arg.key);
        e.WriteData(arg.text);
        HandleEvent(e);
    }
}


Ogre::Root* Client::GetOgreRoot() {
    return mOgreRoot;
}

OIS::Mouse* Client::GetMouse() {
    return mMouse;
}

OIS::Keyboard* Client::GetKeyboard() {
    return mKeyboard;
}

Ogre::RenderWindow* Client::GetWindow() {
    return mWindow;
}

GameStateManager& Client::GetGameStateManager() {
    return mGameStateManager;
}

void Client::RequestShutdown() {
    Logger::GetLogger().Info("Client shutdown requested");
    mRunning = false;
}
