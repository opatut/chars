#include "LoginManager.hpp"

LoginManager::LoginManager() {}

Player* LoginManager::Login(const std::string& player, const std::string& password) {
    if(IsValidData(player, password)) {
        if(GetPlayer(player) == NULL) {
            mPlayersLoggedIn.push_back(new Player(player, GetPlayerID(player)));
            Logger::GetLogger().Info("Player " + player + " logged in successfully.");
            return GetPlayer(player);
        } else {
            Logger::GetLogger().Info("Player " + player + " tried to log in but is already logged in.");
            return NULL;
        }
    } else {
        Logger::GetLogger().Info("Player " + player + " tried to log in with invalid data.");
        return NULL;
    }
}


sf::Uint32 LoginManager::GetPlayerID(const std::string& name) {
    mysqlpp::Query q = DatabaseHelper::get_mutable_instance().NewQuery();
    q << "SELECT id FROM players WHERE name = " << mysqlpp::quote << name << " LIMIT 1";
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
    if(player == "")
        return false;
    std::string hash = GetPlayerPassHash(player);
    return sha(password) == hash;
}

std::string LoginManager::GetPlayerPassHash(const std::string& player) {
    mysqlpp::Query q = DatabaseHelper::get_mutable_instance().NewQuery();
    q << "SELECT password FROM player WHERE name = " << mysqlpp::quote << player << " LIMIT 1";
    mysqlpp::StoreQueryResult res = q.store();
    if(res.num_rows() == 1) {
        std::string r = res[0]["password"].c_str();
        return r;
    }
    return "";
}
