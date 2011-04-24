#include "Server.hpp"

#include "common/log/Logger.hpp"
#include "common/util/Utils.hpp"
#include "server/gamestate/MainState.hpp"

Server::Server() {}

void Server::Go() {
    mRunning = true;

    Logger::GetLogger().Debug("--- Server initialization ---");
    Initialize();

    Logger::GetLogger().Debug("--- Server starting main loop ---");
    RunLoop();

    Logger::GetLogger().Debug("--- Server shutdown ---");
    Deinitialize();
}

void Server::Initialize() {
    mOptions.Load();

    NetworkManager::get_mutable_instance().SetMode(NetworkManager::MODE_SERVER);
    NetworkManager::get_mutable_instance().SetListener(this);
    NetworkManager::get_mutable_instance().SetPort(25567);
    if(!NetworkManager::get_mutable_instance().Connect())
        mRunning = false;
}

void Server::Deinitialize() {
    mOptions.Save();
}

Options& Server::GetOptions() {
    return mOptions;
}

void Server::RunLoop() {
    mGameStateManager.SetNewState(new MainState());

    Ogre::Timer timer;
    while(mRunning) {
        float time_delta = timer.getMicroseconds() * 1000000.f; // seconds
        timer.reset();

        mGameStateManager.PushState();
        UpdateNetwork();
        Update(time_delta);
    }
}

void Server::RequestShutdown() {
    Logger::GetLogger().Info("Server shutdown requested");
    mRunning = false;
}

void Server::UpdateNetwork() {
    NetworkManager::get_mutable_instance().SendQueuedRequests();
    NetworkManager::get_mutable_instance().HandleIncomingRequests();
}

void Server::HandleEvent(Event e) {
    mGameStateManager.HandleEvent(e);
}

void Server::Update(float time_delta) {
    Input in(NULL, NULL);
    mGameStateManager.Update(time_delta, in);
}


void Server::HandleSignal(int sig) {
    std::cout << "\r"; // hide "^C" etc.
    if(sig == SIGINT || sig == SIGABRT || sig == SIGTERM) {
        Server::get_mutable_instance().RequestShutdown();
    } else {
        Logger::GetLogger().Warning("Raised signal: " + tostr(sig));
    }
}
