#include "IconifiedButton.hpp"

IconifiedButton::IconifiedButton() {
    setState("background");
}

void IconifiedButton::SetDisplayableObject(IDisplayableObject* obj) {
    mDisplayableObject = obj;
}

IDisplayableObject* IconifiedButton::GetDisplayableObject() {
    return mDisplayableObject;
}
