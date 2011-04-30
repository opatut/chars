#include "MainGameState.hpp"

#include "client/Client.hpp"
#include "common/util/Utils.hpp"

MainGameState::MainGameState() {
    mEditorSelectedObject = "";
    mEditMode = false;
    mMenuShown = false;
    mCamSpeed = Ogre::Vector3::ZERO;
    mChatLog.SetupLogger();
}

MainGameState::MainGameState(const std::string& playername, const std::string& motd) {
    mEditorSelectedObject = "";
    mEditMode = false;
    mMenuShown = false;
    mCamSpeed = Ogre::Vector3::ZERO;
    mChatLog.SetupLogger();
    mPlayerName = playername;
    mMotd = motd;
}


std::string MainGameState::GetName() const {
    return "MainGameState";
}

void MainGameState::OnEnable() {
    mObjectsConfig.SetFile("../data/config/objects.yml");
    mObjectsConfig.Load();

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
	mCamNode->setPosition(Ogre::Vector3(128, 0, 128));
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50000);
	mCamera->pitch(Ogre::Degree(-60));
	mCamera->setPosition(0, 100, 0);
	mZoomLevel = 2;


	// Setup minimap
	// create the texture for the RTT
	Ogre::TexturePtr m_texture;
	m_texture = Ogre::TextureManager::getSingleton().createManual("MinimapTex",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
		128, 128, 0, Ogre::PF_R8G8B8,
		Ogre::TU_RENDERTARGET);

	// the RTT will receive data from a Camera which is set up below
	Ogre::RenderTexture* renderTexture = m_texture->getBuffer()->getRenderTarget();
	mMinimapCam = mSceneMgr->createCamera("MinimapCamera");
	mCamNode->attachObject(mMinimapCam);
	mMinimapCam->setPosition(0, 50, 0);
	mMinimapCam->pitch(Ogre::Degree(-90));
	mMinimapCam->setNearClipDistance(0.1);
	mMinimapCam->setFarClipDistance(50000);
	// mMinimapCam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
	mMinimapCam->setAspectRatio(1);

	renderTexture->addViewport(mMinimapCam);
	renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);

	// Create one viewport, entire window
	Ogre::Viewport* vp = w->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0.47f, 0.67f, 0.96f));
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));


	if (r->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
		mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can

	Terrain* t = new Terrain();
	t->SetTerrainId("1");
	t->GrabUID();
	AddEntity(t);

	// setup Brush Decal
	mBrush.Create(mSceneMgr, GetName() + "_brush_decal", "Editor/BrushDecal");
	mBrush.SetResolution(5);
	mBrush.SetSize(3);
	// mBrush.Hide();
}

void MainGameState::OnDisable() {
    Ogre::Root* r = Client::get_mutable_instance().GetOgreRoot();
    Ogre::RenderWindow* w = Client::get_mutable_instance().GetWindow();
    w->removeAllViewports();
    mSceneMgr->clearScene();
    r->destroySceneManager(mSceneMgr);
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
	Ogre::RenderWindow* w = Client::get_mutable_instance().GetWindow();

	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(w, mSceneMgr, "GUI"); // mWindow is Ogre::RenderWindow*, mSceneManager is Ogre::SceneManager*
	mGUI = new MyGUI::Gui();
	mGUI->initialise();

	mGUI->resizeWindow(MyGUI::IntSize(w->getWidth(), w->getHeight()));

	MyGUI::FactoryManager::getInstance().registerFactory<IconifiedButton>("Widget");
	//MyGUI::FactoryManager::getInstance().registerFactory<ToolbarWidget>("Widget");

	MyGUI::ResourceManager::getInstance().load("tools.resource");

    // Setup basic gui
    mEditMode = true;
    ToggleEdit();

    mGUI->findWidget<MyGUI::Edit>("edit:chatlog")->setCaption("#FFFF00You logged in as #000000" + mPlayerName + "\n#FFFF00MOTD: " + mMotd);
}

void MainGameState::OnDeinitializeGUI() {
	MyGUI::FactoryManager::getInstance().unregisterFactory<IconifiedButton>("Widget");
	//MyGUI::FactoryManager::getInstance().unregisterFactory<ToolbarWidget>("Widget");

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
        char ch = char(e.ReadData<int>());
        // Logger::GetLogger().Info("Key (" + tostr(code) + ") pressed: " + ch);

        if(code == OIS::KC_ESCAPE) {
            ToggleMenu();
        }

        if(!mMenuShown) {
            if(code == OIS::KC_TAB) {
                ToggleEdit();
            } else if(ch == '+' && mZoomLevel > 1) {
                Logger::GetLogger().Info("Zooming in");
                mZoomLevel--;
            } else if(ch == '-' && mZoomLevel < 3) {
                Logger::GetLogger().Info("Zooming out");
                mZoomLevel++;
            } else {
                Logger::GetLogger().Info("Strange key");
            }
        }
    } else if(!mMenuShown && e.GetIdString() == "input:mouse:pressed") {
        bool left,right;
        int x,y;
        e.GetData() >> left >> right >> x >> y;

        if(mEditMode) {
            if(mEditorSelectedTool == "Tools/PlaceObject" && left && mEditorSelectedObject != "") {
                StaticGeometry* s = new StaticGeometry(mEditorSelectedObject, GetMousePositionOnTerrain());
                s->GrabUID();
                AddEntity(s);
            } else if(mEditorSelectedTool == "Tools/DeleteObject" && left) {
                // raycast to object
                Entity* e = GetObjectAtMousePosition();
                if(e != NULL) {
                    RemoveEntity(e);
                }
            }
        } else {
            Logger::GetLogger().Info("Select unit at " + tostr(GetMousePositionOnTerrain()));
        }
    } else if(e.GetIdString() == "window:resized") {
        // snap all widgets
        int w, h;
        e.GetData() >> w >> h;
        BorderSnap::SnapAllWidgets(mGUI, w, h);
    } else if(e.GetIdString() == "network:request:received") {
        Request* raw = NetworkManager::get_mutable_instance().GetLastReceivedRequest();
        if(raw->GetType() == "request:ping") {
            PingRequest* r = (PingRequest*)raw;
            if(r->IsOriginServer()) {
                // we have to return
                NetworkManager::get_mutable_instance().QueueRequest(new PingRequest(r->GetStartTime(), true));
            } else {
                // it returned
                float ping = mLifetime - r->GetStartTime();
                Logger::GetLogger().Debug("Server pinged back: " + tostr(ping));
            }
        }
    }
}

void MainGameState::OnUpdate(float time_delta, Input& input) {
    if(!mEditMode)
        mChatLog.UpdateLog(mGUI->findWidget<MyGUI::Edit>("edit:chatlog"));

    if(mMenuShown)
        return;

    MyGUI::StaticText* l = mGUI->findWidget<MyGUI::StaticText>("label:fps");
    float fps = Client::get_mutable_instance().GetWindow()->getAverageFPS();
    int tri = Client::get_mutable_instance().GetWindow()->getTriangleCount();
    l->setCaption( tostr((int)fps) + " FPS " + tostr(tri) + " Tri"
                   + " - " + (mEditMode ? "Edit" : "Game") + " mode" );

    if(mEditMode) {
        mBrush.SetPosition(GetMousePositionOnTerrain());
    }

    // Ogre::Vector2 move_cam = Ogre::Vector2::ZERO;
    if(IsKeyDown(input, OIS::KC_W)) {
        mCamSpeed.y = -1;
    }
    if(IsKeyDown(input, OIS::KC_S)) {
        mCamSpeed.y = 1;
    }
    if(IsKeyDown(input, OIS::KC_A)) {
        mCamSpeed.x = -1;
    }
    if(IsKeyDown(input, OIS::KC_D)) {
        mCamSpeed.x = 1;
    }
    if(IsKeyDown(input, OIS::KC_Q)) {
        mCamSpeed.z = 1;
        mCamera->yaw(Ogre::Degree(60.f * time_delta));
    }
    if(IsKeyDown(input, OIS::KC_E)) {
        mCamSpeed.z = -1;
    }

    if(input.GetMouse() != NULL) {
        int border = 10;
        const OIS::MouseState& ms = input.GetMouse()->getMouseState();
        if(ms.X.abs <= border) {
            mCamSpeed.x = -1; // move left
        } else if(ms.X.abs >= ms.width - border){
            mCamSpeed.x = 1;  // move right
        }
        if(ms.Y.abs <= border) {
            mCamSpeed.y = -1; // move up
        } else if(ms.Y.abs >= ms.height - border){
            mCamSpeed.y = 1;  // move down
        }
    }

    float camspeed = 8.f; // units per second
    float rotatespeed = 60.f;
    Ogre::Radian cam_dir = mCamera->getOrientation().getYaw();
    Ogre::Vector3 dir = Ogre::Quaternion(cam_dir, Ogre::Vector3::UNIT_Y) * Ogre::Vector3(mCamSpeed.x, 0, mCamSpeed.y);
    mCamNode->translate(dir * camspeed * time_delta);
    mCamera->yaw(Ogre::Degree(rotatespeed * time_delta * mCamSpeed.z));
    mCamSpeed *= 0.95;

    Ogre::Real target_height = 30.f;
    if(mZoomLevel == 1)
        target_height = 10.f;
    else if(mZoomLevel == 3)
        target_height = 80.f;

    Ogre::Real target_pitch = -60.f;
    if(mZoomLevel == 1)
        target_pitch = -15.f;
    else if(mZoomLevel == 3)
        target_pitch = -90.f;

    float s = 0.05;
    mCamera->setPosition(0, mCamera->getPosition().y * (1-s) + target_height * s, 0);
    Ogre::Radian p = mCamera->getDirection().angleBetween(Ogre::Vector3::UNIT_Y) - Ogre::Degree(90);
    mCamera->pitch((p + Ogre::Degree(target_pitch)) * s);
}

void MainGameState::ToggleEdit() {
    mEditMode = !mEditMode;
    mEditorSelectedObject = "";
    if(! mDynamicLayout.empty()) {
        MyGUI::LayoutManager::getInstance().unloadLayout(mDynamicLayout);
    }
    if(mEditMode) {
        Logger::GetLogger().Info("-- Edit mode --");
        mDynamicLayout = MyGUI::LayoutManager::getInstance().loadLayout("editor.layout");

        TestButton(NULL);

        mGUI->findWidget<MyGUI::Button>("Tools/PlaceObject")->eventMouseButtonClick = MyGUI::newDelegate(this, &MainGameState::TestButton);
        mGUI->findWidget<MyGUI::Button>("Tools/DeleteObject")->eventMouseButtonClick = MyGUI::newDelegate(this, &MainGameState::TestButton);
        mGUI->findWidget<MyGUI::Button>("Tools/Brush")->eventMouseButtonClick = MyGUI::newDelegate(this, &MainGameState::TestButton);

        MyGUI::ComboBox* c = mGUI->findWidget<MyGUI::ComboBox>("combobox:categoryselector");
        c->eventComboAccept = MyGUI::newDelegate(this, &MainGameState::EditorCategorySelect);
        std::map<std::string, ConfigurationNode>& cats =  mObjectsConfig.GetSubnode("objects")->GetSubnodes();
        for(auto iter = cats.begin(); iter != cats.end(); ++iter) {
            c->addItem(iter->second.GetName());
        }
        mGUI->findWidget<MyGUI::ComboBox>("combobox:objectselector")->eventComboAccept = MyGUI::newDelegate(this, &MainGameState::EditorObjectSelect);

        if(c->getItemCount() > 0) {
            c->setItemSelectedAt(0);
            EditorCategorySelect(NULL);
        }
    } else {
        Logger::GetLogger().Info("-- Game mode --");
        mDynamicLayout = MyGUI::LayoutManager::getInstance().loadLayout("game.layout");

        MyGUI::EditPtr edit = mGUI->findWidget<MyGUI::Edit>("edit:chat");
        edit->eventEditSelectAccept = MyGUI::newDelegate(this, &MainGameState::ChatMessage);

        MyGUI::StaticImagePtr minimap = mGUI->findWidget<MyGUI::StaticImage>("image:minimap");
        minimap->setImageTexture("MinimapTex");
    }

    BorderSnap::SnapAllWidgets(mGUI, Client::get_mutable_instance().GetWindow()->getWidth(), Client::get_mutable_instance().GetWindow()->getHeight());
}

void MainGameState::ToggleMenu() {
    mMenuShown = !mMenuShown;
    if(mMenuShown) {
        mMenu = MyGUI::LayoutManager::getInstance().loadLayout("game_menu.layout");
        mGUI->findWidget<MyGUI::Button>("button:quit")->eventMouseButtonClick = MyGUI::newDelegate(this, &MainGameState::QuitButton);
        mGUI->findWidget<MyGUI::Button>("button:back")->eventMouseButtonClick = MyGUI::newDelegate(this, &MainGameState::QuitButton);
    } else {
        MyGUI::LayoutManager::getInstance().unloadLayout(mMenu);
    }

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

Entity* MainGameState::GetObjectAtMousePosition() {
    const OIS::MouseState& ms = Client::get_mutable_instance().GetMouse()->getMouseState();
    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(1.0f * ms.X.abs / ms.width, 1.0f * ms.Y.abs / ms.height);

    Ogre::RaySceneQuery* query = mSceneMgr->createRayQuery(mouseRay, Ogre::SceneManager::ENTITY_TYPE_MASK);
    query->setRay(mouseRay);

    Ogre::RaySceneQueryResult& result= query->execute();
    Ogre::RaySceneQueryResult::iterator itr = result.begin();

    for(itr = result.begin(); itr != result.end(); ++itr) {
        if(itr->movable) {
            Entity* e = GetObjectOwnerEntity(itr->movable);
            if(e != NULL)
                return e;
        }
    }
    return NULL;
}

Entity* MainGameState::GetObjectOwnerEntity(Ogre::MovableObject* o) {
    for(auto iter = mEntities.begin(); iter != mEntities.end(); ++iter) {
        if(iter->OwnsObject(o))
            return &(*iter);
    }
    return NULL;
}


void MainGameState::TestButton(MyGUI::WidgetPtr _sender) {
    std::vector<std::string> buttons;
    buttons.push_back("Tools/PlaceObject");
    buttons.push_back("Tools/DeleteObject");
    buttons.push_back("Tools/Brush");

    for(auto iter = buttons.begin(); iter != buttons.end(); ++iter) {
        MyGUI::Button* b = mGUI->findWidget<MyGUI::Button>(*iter);
        b->setEnabled(_sender == NULL || b->getName() != _sender->getName());
    }

    mEditorSelectedTool = (_sender==NULL ? "" : _sender->getName());

    MyGUI::Window* w = mGUI->findWidget<MyGUI::Window>("ObjectSelect");
    w->setVisible(mEditorSelectedTool == "Tools/PlaceObject");

    w = mGUI->findWidget<MyGUI::Window>("BrushSelect");
    w->setVisible(mEditorSelectedTool == "Tools/Brush");
}

void MainGameState::ChatMessage(MyGUI::WidgetPtr _sender) {
    MyGUI::EditPtr edit = mGUI->findWidget<MyGUI::Edit>("edit:chat");
    std::string msg = edit->getCaption();
    if(msg != "") {
        Logger::GetLogger().Info("Chat: " + msg);
        Logger::GetLogger("Chat").Info(msg);
        edit->setCaption("");
    }
}

void MainGameState::EditorCategorySelect(MyGUI::WidgetPtr _sender) {
    MyGUI::ComboBox* cat = mGUI->findWidget<MyGUI::ComboBox>("combobox:categoryselector");

    MyGUI::ComboBox* obj = mGUI->findWidget<MyGUI::ComboBox>("combobox:objectselector");
    std::map<std::string, ConfigurationNode>& objs =  mObjectsConfig.GetSubnode("objects." + cat->getCaption())->GetSubnodes();
    obj->removeAllItems();
    for(auto iter = objs.begin(); iter != objs.end(); ++iter) {
        obj->addItem(iter->second.GetName());
    }
    if(obj->getItemCount() > 0) {
        obj->setItemSelectedAt(0);
        EditorObjectSelect(NULL);
    }
}

void MainGameState::EditorObjectSelect(MyGUI::WidgetPtr _sender) {
    MyGUI::ComboBox* cat = mGUI->findWidget<MyGUI::ComboBox>("combobox:categoryselector");
    MyGUI::ComboBox* obj = mGUI->findWidget<MyGUI::ComboBox>("combobox:objectselector");
    std::string path = "objects." + cat->getCaption() + "." + obj->getCaption();

    ConfigurationNode* n = mObjectsConfig.GetSubnode(path);

    MyGUI::StaticText* model = mGUI->findWidget<MyGUI::StaticText>("label:objectmodel");
    model->setCaption( n->GetSubnode("model")->GetValueString() );

    std::string thumbnail = "thumbnail_none.png";
    if(n->GetSubnode("thumbnail") != NULL && n->GetSubnode("thumbnail")->GetValueString() != "") {
        thumbnail = n->GetSubnode("thumbnail")->GetValueString();
    }
    MyGUI::StaticImage* img = mGUI->findWidget<MyGUI::StaticImage>("image:objectthumbnail");
    img->setImageTexture( thumbnail );

    // set selected item
    mEditorSelectedObject = n->GetSubnode("model")->GetValueString();
}

void MainGameState::QuitButton(MyGUI::WidgetPtr _sender) {
    if(_sender->getName() == "button:quit")
        Client::get_mutable_instance().GetGameStateManager().SetNewState(new LoginState());
    else if(_sender->getName() == "button:back")
        ToggleMenu();
}
