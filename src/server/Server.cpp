#include "Server.hpp"

#include "common/log/Logger.hpp"
#include "common/util/Utils.hpp"

Server::Server() {}

void Server::Go() {
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
}

void Server::Deinitialize() {
    mOptions.Save();
}

Options& Server::GetOptions() {
    return mOptions;
}

void Server::RunLoop() {
    mRunning = true;
    while(mRunning) {
        Update(0.f);
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
