#include "Input.hpp"

Input::Input(OIS::Mouse* m, OIS::Keyboard* k) {
    mMouse = m;
    mKeyboard = k;
}

OIS::Mouse* Input::GetMouse() {
    return mMouse;
}

OIS::Keyboard* Input::GetKeyboard() {
    return mKeyboard;
}
