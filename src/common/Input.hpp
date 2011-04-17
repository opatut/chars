#ifndef INPUT_HPP
#define INPUT_HPP

#include <OISKeyboard.h>
#include <OISMouse.h>

class Input {
public:
    Input(OIS::Mouse* m, OIS::Keyboard* k);
    OIS::Mouse* GetMouse();
    OIS::Keyboard* GetKeyboard();
private:
    OIS::Mouse* mMouse;
    OIS::Keyboard* mKeyboard;
};

#endif
