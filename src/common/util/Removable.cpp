#include "Removable.hpp"

Removable::Removable() {
    mMarkedForRemoval = false;
}

void Removable::MarkForRemoval() {
    mMarkedForRemoval = true;
}

void Removable::UnmarkForRemoval() {
    mMarkedForRemoval = false;
}

bool Removable::IsMarkedForRemoval() {
    return mMarkedForRemoval;
}
