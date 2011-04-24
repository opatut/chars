#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/serialization/singleton.hpp>

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>
#include <OgreRenderWindow.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "common/config/Configuration.hpp"
#include "common/gamestate/GameStateManager.hpp"
#include "common/events/EventListener.hpp"
#include "common/network/NetworkManager.hpp"
#include "gamestate/MainGameState.hpp"
#include "gamestate/LoginState.hpp"
#include "common/Input.hpp"
#include "common/log/Logger.hpp"

class Client :
        public boost::serialization::singleton<Client>,
        public OIS::KeyListener,
        public OIS::MouseListener,
        public Ogre::WindowEventListener,
        public Ogre::FrameListener,
        public EventListener {
public:
    Client();

    void Go();

    void LoadConfig();
    void SaveConfig();

private:
    bool InitializeNetwork();
    void StartupOgre();
    void InitializeWindow();    
    void RunLoop();
    void Shutdown();

public:
    void HandleEvent(Event e);


	// Ogre::WindowEventListener
	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);

	// Ogre::FrameListener
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // OIS::MouseListener
    bool mouseMoved(const OIS::MouseEvent &arg);
    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    // OIS::KeyListener
    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);

    Ogre::Root* GetOgreRoot();
    OIS::Mouse* GetMouse();
    OIS::Keyboard* GetKeyboard();
    Ogre::RenderWindow* GetWindow();

    GameStateManager& GetGameStateManager();

    void RequestShutdown();
private:
    bool mRunning;
    Configuration mConfiguration;
    GameStateManager mGameStateManager;

	// Ogre basic elements
	Ogre::Root* mOgreRoot;
	Ogre::RenderWindow* mWindow;

	// OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
};

#endif
