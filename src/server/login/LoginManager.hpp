#ifndef LOGIN_LOGINMANAGER_HPP
#define LOGIN_LOGINMANAGER_HPP

#include <string>
#include <boost/serialization/singleton.hpp>
#include <boost/bind.hpp>
#include <openssl/sha.h>

#include "common/log/Logger.hpp"
#include "common/objects/Player.hpp"
#include "common/util/Utils.hpp"
#include "server/database/DatabaseHelper.hpp"

class LoginManager : public boost::serialization::singleton<LoginManager> {
public:
    LoginManager();

    Player* Login(const std::string& player, const std::string& password);
    bool Logout(Player* player);
    bool Logout(const std::string& name);

    sf::Uint32 GetPlayerID(const std::string& name);
    Player* GetPlayer(const std::string& name);
private:
    bool IsValidData(const std::string& player, const std::string& password);
    std::string GetPlayerPassHash(const std::string& player);

    boost::ptr_vector<Player> mPlayersLoggedIn;
};

#endif
