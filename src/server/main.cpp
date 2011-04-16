#include <iostream>
#include <mysql++.h>

#include "common/Strings.hpp"
#include "common/Logger.hpp"
#include "Server.hpp"
#include "database/DatabaseHelper.hpp"

using namespace mysqlpp;

int main() {
    define_strings();

    std::string pw = "2tADxYjhUL5FNh5f";
    std::string user = "chars";
    std::string database = "chars";
    std::string host = "localhost";

    DatabaseHelper& h = DatabaseHelper::get_mutable_instance();
    h.Connect(host, database, user, pw);

    Server server;
    server.Load();

    Options& o = server.GetOptions();

    std::string motd = o.GetString("motd", "No motd set.");

    Logger::GetLogger("default").Info("MOTD: " + motd);
    Logger::GetLogger().InfoStream() << "MOTD: " << motd << std::endl;

    //o.SetString("motd", "Hello World I am sooo awesome!");

    server.Save();
    


    return 0;
}
