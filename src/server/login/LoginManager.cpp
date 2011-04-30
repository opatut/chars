#include "LoginManager.hpp"

LoginManager::LoginManager() {}

LoginResultRequest::Result LoginManager::Login(const std::string& player, const std::string& password) {
    if(IsValidData(player, password)) {
        if(GetPlayer(player) == NULL) {
            mPlayersLoggedIn.push_back(new Player(player, GetPlayerID(player)));
            Logger::GetLogger().Info("Player " + player + " logged in.");
            return LoginResultRequest::SUCCESS;
        } else {
            Logger::GetLogger().Info("Player " + player + " tried to log in but is already logged in.");
            return LoginResultRequest::FAIL_ALREADY_LOGGED_IN;
        }
    } else {
        Logger::GetLogger().Info("Player " + player + " tried to log in with invalid data.");
        return LoginResultRequest::FAIL_BAD_LOGIN;
    }
}


sf::Uint32 LoginManager::GetPlayerID(const std::string& name) {
    mysqlpp::Query q = DatabaseHelper::get_mutable_instance().NewQuery();
    q << "SELECT id FROM users WHERE name = " << mysqlpp::quote << name;
    mysqlpp::StoreQueryResult res = q.store();
    if(res.num_rows() == 1) {
        int id = res[0]["id"];
        return id;
    }
    return 0;
}


Player* LoginManager::GetPlayer(const std::string& name) {
    for(auto iter = mPlayersLoggedIn.begin(); iter != mPlayersLoggedIn.end(); ++iter) {
        if(iter->GetName() == name)
            return &(*iter);
    }
}

bool LoginManager::Logout(Player* player) {
    if(player == NULL)
        return false;
    int c = mPlayersLoggedIn.size();
    std::string name = player->GetName();
    mPlayersLoggedIn.erase_if(boost::bind(&Player::GetID, _1) == player->GetID());
    if(c != mPlayersLoggedIn.size()) {
        Logger::GetLogger().Info("Player " + name + " logged out.");
        return true;
    }
    return false;
}

bool LoginManager::Logout(const std::string& name) {
    return Logout(GetPlayer(name));
}

bool LoginManager::IsValidData(const std::string& player, const std::string& password) {
    if(player == "" || password == "")
        return false;
    return password == GetPlayerPassHash(player);
}

std::string LoginManager::GetPlayerPassHash(const std::string& player) {
    mysqlpp::Query q = DatabaseHelper::get_mutable_instance().NewQuery();
    q << "SELECT password FROM users WHERE name = " << mysqlpp::quote << player;
    mysqlpp::StoreQueryResult res = q.store();
    if(res.num_rows() == 1) {
        return res[0]["password"].c_str();
    }
    Logger::GetLogger().Error("Cannot find user " + player + ".");
    return "";
}

std::vector<Player*> LoginManager::GetPlayersWithPingGreaterThan(int min) {
    std::vector<Player*> result;
    for(auto iter = mPlayersLoggedIn.begin(); iter != mPlayersLoggedIn.end(); ++iter) {
        if(iter->GetPing() > min)
            result.push_back(&(*iter));
    }
    return result;
}

void LoginManager::KickPlayer(const std::string& name, const std::string& reason) {
    Player* p = GetPlayer(name);
    if(p == NULL) {
        Logger::GetLogger().Error("Cannot kick player: " + name + ". This player is not logged in.");
    } else {
        // inform the players about the kick
        // NetworkManager::get_mutable_instance().QueueRequest(new PlayerKickRequest(name, reason));

        // logout player
        Logout(p);
    }
}
