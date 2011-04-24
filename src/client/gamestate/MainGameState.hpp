#ifndef GAMESTATE_MAINGAMESTATE_HPP
#define GAMESTATE_MAINGAMESTATE_HPP

#include <MyGUI.h>

#include "common/util/TerrainHeight.hpp"
#include "common/gamestate/GameState.hpp"
#include "common/entities/Character.hpp"
#include "common/entities/StaticGeometry.hpp"
#include "common/entities/Terrain.hpp"
#include "common/config/Configuration.hpp"
#include "client/render/TerrainDecal.hpp"
#include "client/ChatLogStream.hpp"
#include "client/gui/BorderSnap.hpp"

#include <Ogre.h>

class MainGameState : public GameState {
public:
    MainGameState();
    MainGameState(const std::string& playername, const std::string& motd);
    std::string GetName() const;
    void OnEnable();
    void OnDisable();
    void OnLoadResources();
    void OnUnloadResources();
    void OnInitializeGUI();
    void OnDeinitializeGUI();
    void OnEvent(Event e);
    void OnUpdate(float time_delta, Input& input);

    void ToggleEdit();
    void ToggleMenu();

    // GUI callbacks
    void TestButton(MyGUI::WidgetPtr _sender);
    void ChatMessage(MyGUI::WidgetPtr _sender);
    void EditorCategorySelect(MyGUI::WidgetPtr _sender);
    void EditorObjectSelect(MyGUI::WidgetPtr _sender);
    void QuitButton(MyGUI::WidgetPtr _sender);

    Ogre::Vector3 GetMousePositionOnTerrain();
    Entity* GetObjectAtMousePosition();
    Entity* GetObjectOwnerEntity(Ogre::MovableObject* o);
private:
    bool mEditMode;
    bool mMenuShown;
    Ogre::SceneNode* mCamNode;
    Ogre::Camera* mMinimapCam;
    TerrainDecal mBrush;
    MyGUI::VectorWidgetPtr mDynamicLayout;
    MyGUI::VectorWidgetPtr mMenu;
    Configuration mObjectsConfig;
    std::string mEditorSelectedObject;
    std::string mEditorSelectedTool;

    ChatLogStream mChatLog;

    std::string mPlayerName;
    std::string mMotd;
};

#endif
