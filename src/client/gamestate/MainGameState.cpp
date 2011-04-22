#include "MainGameState.hpp"

#include "client/Client.hpp"
#include "common/util/Utils.hpp"

MainGameState::MainGameState() {
    mEditorSelectedObject = "";
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
	mCamNode->setPosition(Ogre::Vector3(128, 20, 128));
	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50000);


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
	mMinimapCam->setPosition(0, 30, 0);
	mMinimapCam->pitch(Ogre::Degree(-90));
	mMinimapCam->setNearClipDistance(0.1);
	mMinimapCam->setFarClipDistance(50000);
	// mMinimapCam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

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

	MyGUI::ResourceManager::getInstance().load("tools.resource");

    // Setup basic gui
    mEditMode = true;
    ToggleEdit();
    /* MyGUI::StaticTet* l = mGUI->createWidget<MyGUI::StaticText>("StaticText", 10, 780, 800, 20, MyGUI::Align::Left | MyGUI::Align::Bottom, "Main", "label:fps"); */
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
        // char ch = char(e.ReadData<int>());
        // Logger::GetLogger().Info("Key (" + tostr(code) + ") pressed: " + ch);

        if(code == OIS::KC_TAB) {
            ToggleEdit();
            return;
        }
    } else if(e.GetIdString() == "input:mouse:pressed") {
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
    }
    PassToNextState();
}

void MainGameState::OnUpdate(float time_delta, Input& input) {
    MyGUI::StaticText* l = mGUI->findWidget<MyGUI::StaticText>("label:fps");
    float fps = Client::get_mutable_instance().GetWindow()->getAverageFPS();
    l->setCaption( tostr((int)fps) + " FPS"
                   + " - " + (mEditMode ? "Edit" : "Game") + " mode" );

    if(mEditMode) {
        mBrush.SetPosition(GetMousePositionOnTerrain());
    }

    Ogre::Vector2 move_cam = Ogre::Vector2::ZERO;
    if(IsKeyDown(input, OIS::KC_W)) {
        move_cam.y = -1;
    }
    if(IsKeyDown(input, OIS::KC_S)) {
        move_cam.y = 1;
    }
    if(IsKeyDown(input, OIS::KC_A)) {
        move_cam.x = -1;
    }
    if(IsKeyDown(input, OIS::KC_D)) {
        move_cam.x = 1;
    }
    if(IsKeyDown(input, OIS::KC_Q)) {
        mCamera->yaw(Ogre::Degree(60.f * time_delta));
    }
    if(IsKeyDown(input, OIS::KC_E)) {
        mCamera->yaw(Ogre::Degree(- 60.f * time_delta));
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

    float camspeed = 8.f; // units per second
    Ogre::Radian cam_dir = mCamera->getOrientation().getYaw();
    Ogre::Vector3 dir = Ogre::Quaternion(cam_dir, Ogre::Vector3::UNIT_Y) * Ogre::Vector3(move_cam.x, 0, move_cam.y);
    mCamNode->translate(dir * camspeed * time_delta);

    // PassToNextState();
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
    if(edit->getCaption() != "") {
        Logger::GetLogger().Info("Chat: " + edit->getCaption());
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
