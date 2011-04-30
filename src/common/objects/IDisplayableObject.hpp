#ifndef OBJECTS_IDISPLAYABLEOBJECT_HPP
#define OBJECTS_IDISPLAYABLEOBJECT_HPP

#include <OgreTexture.h>
#include <OgreTextureManager.h>

class IDisplayableObject {
public:
    Ogre::Texture* GetIconTexture() const;
};

#endif
