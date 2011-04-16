#ifndef WORLD_HPP
#define WORLD_HPP

#include "Enums.hpp"

class World {
public:
    World(Mode::Mode mode);
    ~World();

    void Initialize();
    void Deinitialize();

    // = main game loop methods
    void HandleEvent(const Event& e);
    void Networking();
    void Update(float time_diff);
    void Draw();    // only for client side use (maybe we'll add a server side GUI, e.g. with curses)

    bool Load();
    bool Save();
private:
    Mode::Mode mMode;
};

#endif
