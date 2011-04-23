#ifndef GAMESTATE_LOGINSTATE_HPP
#define GAMESTATE_LOGINSTATE_HPP

#include <Ogre.h>
#include <MyGUI.h>

#include "common/gamestate/GameState.hpp"
#include "client/gui/BorderSnap.hpp"

class LoginState : public GameState {
public:
    LoginState();
    std::string GetName() const;
    void OnEnable();
    void OnDisable();
    void OnLoadResources();
    void OnUnloadResources();
    void OnInitializeGUI();
    void OnDeinitializeGUI();
    void OnEvent(Event e);
    void OnUpdate(float time_delta, Input& input);

    void EditKeyPressed(MyGUI::Widget* _sender, MyGUI::KeyCode _key, MyGUI::Char _char);
    void LoginButton(MyGUI::WidgetPtr _sender);
    void QuitButton(MyGUI::WidgetPtr _sender);
private:

};

#endif
