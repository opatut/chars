#include "MainGameState.hpp"

#include "client/Client.hpp"
#include "common/util/Utils.hpp"

MainGameState::MainGameState() {
    std::cout << "Creating MainGameState object (constructor) " << std::endl;
}

std::string MainGameState::GetName() const {
    return "MainGameState";
}

void MainGameState::OnEnable() {
    Ogre::Root* r = Client::get_mutable_instance().GetOgreRoot();
    Ogre::RenderWindow* w = r->getAutoCreatedWindow();

    mSceneMgr = r->createSceneManager(Ogre::ST_EXTERIOR_CLOSE, GetName() + "_SceneManager");

    HeightFunction::initialize(mSceneMgr);

    /*Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");

    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    headNode->attachObject(ogreHead); */

    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);

	// Create the camera and a cam node
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(GetName() + "_camnode");
	mCamNode->attachObject(mCamera);
	mCamNode->setPosition(Ogre::Vector3(512, 80, 512));
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50000);

	// Create one viewport, entire window
	Ogre::Viewport* vp = w->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0.47f, 0.67f, 0.96f));
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));


	if (r->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
		mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can

	// create character entity
	Character* c = new Character();
	c->GrabUID();
	AddEntity(c);

	Terrain* t = new Terrain();
	t->SetTerrainId("1");
	t->GrabUID();
	AddEntity(t);
	std::cout << "# MGS INIT" << std::endl;

	// setup Brush Decal
	mBrush.Create(mSceneMgr, GetName() + "_brush_decal", "Editor/BrushDecal");
	mBrush.SetResolution(40);
	mBrush.SetSize(20);
}

void MainGameState::OnDisable() {

}

void MainGameState::OnLoadResources() {
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Terrain1");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Editor");
}

void MainGameState::OnUnloadResources() {
	Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup("Terrain1");
	Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup("Editor");
}

void MainGameState::OnInitializeGUI() {
	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(Client::get_mutable_instance().GetWindow(), mSceneMgr, "GUI"); // mWindow is Ogre::RenderWindow*, mSceneManager is Ogre::SceneManager*
	mGUI = new MyGUI::Gui();
	mGUI->initialise();

    // Create Button
    // MyGUI::LayoutManager::getInstance().load("test.layout");
    // set callback
    MyGUI::StaticText* l = mGUI->createWidget<MyGUI::StaticText>("StaticText", 10, 10, 100, 30, MyGUI::Align::Left, "Main", "label:fps");
}

void MainGameState::OnDeinitializeGUI() {
	mGUI->shutdown();
	delete mGUI;
	mGUI = 0;
	mPlatform->shutdown();
	delete mPlatform;
	mPlatform = 0;
}

void MainGameState::OnEvent(Event e) {
    if(e.GetIdString() == "input:keyboard:pressed") {
        int code = e.ReadData<int>();
        switch(code) {
        case OIS::KC_TAB:
            ToggleEdit();
            break;
        }
    }
    PassToNextState();
}

void MainGameState::OnUpdate(float time_delta, Input& input) {
    MyGUI::StaticText* l = mGUI->findWidget<MyGUI::StaticText>("label:fps");
    l->setCaption( tostr( Client::get_mutable_instance().GetWindow()->getAverageFPS() ) + " FPS (average)");

    if(mEditMode) {
        mBrush.SetPosition(GetMousePositionOnTerrain());
    }

    Ogre::Vector2 move_cam = Ogre::Vector2::ZERO;
    if(input.GetKeyboard() != NULL) {
        if(input.GetKeyboard()->isKeyDown(OIS::KC_W)) {
            move_cam.y = -1;
        }
        if(input.GetKeyboard()->isKeyDown(OIS::KC_S)) {
            move_cam.y = 1;
        }
        if(input.GetKeyboard()->isKeyDown(OIS::KC_A)) {
            move_cam.x = -1;
        }
        if(input.GetKeyboard()->isKeyDown(OIS::KC_D)) {
            move_cam.x = 1;
        }

        float turnspeed = 120.f; // degrees per second
        if(input.GetKeyboard()->isKeyDown(OIS::KC_Q)) {
            // rotate left
            mCamera->yaw(Ogre::Degree(turnspeed * time_delta));
        }
        if(input.GetKeyboard()->isKeyDown(OIS::KC_E)) {
            // rotate right
            mCamera->yaw(Ogre::Degree(- turnspeed * time_delta));
        }
    }
    if(input.GetMouse() != NULL) {
        int border = 10;
        const OIS::MouseState& ms = input.GetMouse()->getMouseState();
        if(ms.X.abs <= border) {
            move_cam.x = -1; // move left
        } else if(ms.X.abs >= ms.width - border){
            move_cam.x = 1;  // move right
        }
        if(ms.Y.abs <= border) {
            move_cam.y = -1; // move up
        } else if(ms.Y.abs >= ms.height - border){
            move_cam.y = 1;  // move down
        }

        if(ms.buttonDown(OIS::MB_Right)) {
            float sensitivity = 0.25;
            mCamera->yaw(Ogre::Degree(ms.X.rel * sensitivity));
            mCamera->pitch(Ogre::Degree(ms.Y.rel * sensitivity));
        }
    }

    float camspeed = 32.f; // units per second
    Ogre::Radian cam_dir = mCamera->getOrientation().getYaw();
    Ogre::Vector3 dir = Ogre::Quaternion(cam_dir, Ogre::Vector3::UNIT_Y) * Ogre::Vector3(move_cam.x, 0, move_cam.y);
    mCamNode->translate(dir * camspeed * time_delta);

    // PassToNextState();
}

void MainGameState::ToggleEdit() {
    mEditMode = !mEditMode;
}

void MainGameState::TestButton(MyGUI::WidgetPtr _sender) {
    std::cout << "click button" << std::endl;
}

Ogre::Vector3 MainGameState::GetMousePositionOnTerrain() {
    const OIS::MouseState& ms = Client::get_mutable_instance().GetMouse()->getMouseState();
    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(1.0f * ms.X.abs / ms.width, 1.0f * ms.Y.abs / ms.height);

    Ogre::RaySceneQuery* query = mSceneMgr->createRayQuery(mouseRay, Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
    query->setRay(mouseRay);

    Ogre::RaySceneQueryResult& result= query->execute();
    Ogre::RaySceneQueryResult::iterator itr = result.begin();

    for(itr = result.begin(); itr != result.end(); ++itr) {
        if(itr->worldFragment) {
            return itr->worldFragment->singleIntersection;
        }
    }
    return Ogre::Vector3::ZERO;
}
