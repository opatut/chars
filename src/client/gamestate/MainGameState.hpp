#ifndef GAMESTATE_MAINGAMESTATE_HPP
#define GAMESTATE_MAINGAMESTATE_HPP

#include <MyGUI.h>

#include "common/util/TerrainHeight.hpp"
#include "common/gamestate/GameState.hpp"
#include "common/entities/Character.hpp"
#include "common/entities/Terrain.hpp"
#include "client/render/TerrainDecal.hpp"

#include <Ogre.h>

class MainGameState : public GameState {
public:
    MainGameState();
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
    void TestButton(MyGUI::WidgetPtr _sender);

    Ogre::Vector3 GetMousePositionOnTerrain();
private:
    bool mEditMode;
    Ogre::SceneNode* mCamNode;
    TerrainDecal mBrush;
};

#endif
