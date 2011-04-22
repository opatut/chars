#ifndef UTIL_REMOVABLE_HPP
#define UTIL_REMOVABLE_HPP

class Removable {
public:
    Removable();
    ~Removable() {}
    void MarkForRemoval();
    void UnmarkForRemoval();
    bool IsMarkedForRemoval();
private:
    bool mMarkedForRemoval;
};

#endif
