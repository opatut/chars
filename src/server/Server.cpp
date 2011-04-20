#include "Server.hpp"

#include "common/Logger.hpp"

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

    mNetworkManager.SetMode(NetworkManager::MODE_SERVER);
    mNetworkManager.SetListener(this);
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
    mNetworkManager.SendQueuedRequests();
    mNetworkManager.HandleIncomingRequests();
}

void Server::HandleEvent(Event e) {
    mGameStateManager.HandleEvent(e);
}

void Server::Update(float time_delta) {
    Input in(NULL, NULL);
    mGameStateManager.Update(time_delta, in);
}


void Server::HandleSignal(int sig) {
    Logger::GetLogger().WarningStream() << "Raised signal: " << sig << std::endl;
    if(sig == SIGINT || sig == SIGABRT || sig == SIGTERM) {
        Server::get_mutable_instance().RequestShutdown();
    }
}
