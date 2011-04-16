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
#include "gamestate/GameStateManager.hpp"
#include "gamestate/MainGameState.hpp"

class Client :
        public boost::serialization::singleton<Client>,
        public OIS::KeyListener,
        public OIS::MouseListener,
        public Ogre::WindowEventListener,
        public Ogre::FrameListener {
public:
    Client();
    ~Client();

    void LoadConfig();
    void SaveConfig();

    void StartupOgre();
    void InitializeWindow();
    void RunLoop();
    void HandleEvent(Event& e);

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
private:
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
