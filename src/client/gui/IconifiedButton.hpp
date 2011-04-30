#ifndef GUI_ICONIFIEDBUTTON_HPP
#define GUI_ICONIFIEDBUTTON_HPP

#include <MyGUI.h>
#include "common/objects/IDisplayableObject.hpp"

class IconifiedButton : public MyGUI::Button {
    MYGUI_RTTI_DERIVED( IconifiedButton )

public:
    IconifiedButton();

    void SetDisplayableObject(IDisplayableObject* obj);
    IDisplayableObject* GetDisplayableObject();
private:
    bool mHover;
    IDisplayableObject* mDisplayableObject;

};

#endif
