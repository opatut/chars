#include "MainState.hpp"

#include "server/Server.hpp"

MainState::MainState() {
    mTimeToNextPing = 0;
}

std::string MainState::GetName() const  {
    return "MainState";
}

void MainState::OnEnable() {}

void MainState::OnDisable() {}

void MainState::OnLoadResources() {}

void MainState::OnUnloadResources() {}

void MainState::OnInitializeGUI() {}

void MainState::OnDeinitializeGUI() {}

void MainState::OnEvent(Event e) {
    if(e.GetIdString() == "network:request:received") {
        Request* raw = NetworkManager::get_mutable_instance().GetLastReceivedRequest();
        if(raw->GetType() == "request:login") {
            LoginRequest* r = (LoginRequest*)raw;
            // Logger::GetLogger().Info("User logging in with username: " +  r->GetUsername() + " and pw hash: " + r->GetPasswordHash());

            LoginResultRequest::Result res = mLoginManager.Login(r->GetUsername(), r->GetPasswordHash());
            LoginResultRequest* reply = new LoginResultRequest(r->GetUsername(), res, "");
            if(res == LoginResultRequest::SUCCESS) {
                // rename in clientmananger
                NetworkManager::get_mutable_instance().GetClientManager().Rename(r->GetReceivedFrom(), r->GetUsername());
                // send success
                reply->SetMessage(Server::get_mutable_instance().GetOptions().GetString("motd"));
                reply->AddRecipient(r->GetUsername());
                NetworkManager::get_mutable_instance().QueueRequest(reply);
            } else {
                // send result ( something bad :/ )
                reply->AddRecipient(r->GetReceivedFrom());
                NetworkManager::get_mutable_instance().QueueRequest(reply);
                // remove client
                // NetworkManager::get_mutable_instance().GetClientManager().Remove(r->GetReceivedFrom());
            }
        } else if(raw->GetType() == "request:ping") {
            PingRequest* r = (PingRequest*)raw;
            if(r->IsOriginServer()) {
                // it returned
                float ping = mLifetime - r->GetStartTime();
                std::string user = r->GetReceivedFrom();
                mLoginManager.GetPlayer(user)->SetPing((int) round(ping * 1000));
            } else {
                // we gotta return
                NetworkManager::get_mutable_instance().QueueRequest(new PingRequest(r->GetStartTime(), false));
            }
        } else {
            Logger::GetLogger().Info("Received a generic request!");
        }
    }
}

void MainState::OnUpdate(float time_delta, Input& input) {
    mTimeToNextPing -= time_delta;
    if(mTimeToNextPing < 0) {
        mTimeToNextPing = 5.f;
        NetworkManager::get_mutable_instance().QueueRequest(new PingRequest(mLifetime, true));
    }
    std::vector<Player*> ping_too_high = mLoginManager.GetPlayersWithPingGreaterThan(10000);
    for(auto iter = ping_too_high.begin(); iter != ping_too_high.end(); ++iter) {
        Logger::GetLogger().Warning("Player " + (*iter)->GetName() + " has too high ping: " + tostr((*iter)->GetPing()));
        // iter->Kick("Ping timeout (" + (*iter)->GetPing() + ")");
    }
}
