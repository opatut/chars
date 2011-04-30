#include "IDisplayableObject.hpp"

Ogre::Texture* IDisplayableObject::GetIconTexture() const {
    return (Ogre::Texture*)Ogre::TextureManager::getSingleton().getByName("icon_unknown.png").getPointer();
}
