#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/serialization/singleton.hpp>

#include "common/events/Event.hpp"
#include "common/gamestate/GameStateManager.hpp"
#include "common/network/NetworkManager.hpp"
#include "database/Options.hpp"

class Server :
        public boost::serialization::singleton<Server>,
        public EventListener {
public:
    Server();

    void Go();

    void Initialize();
    void Deinitialize();

    void RunLoop();
    void RequestShutdown();

    void UpdateNetwork();
    void HandleEvent(Event e);
    void Update(float time_delta);

    Options& GetOptions();

    static void HandleSignal(int sig);
private:
    Options mOptions;
    GameStateManager mGameStateManager;
    bool mRunning;
};

#endif
