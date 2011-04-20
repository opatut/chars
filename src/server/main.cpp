#include <iostream>
#include <mysql++.h>
#include <signal.h>

#include "common/Strings.hpp"
#include "common/Logger.hpp"
#include "Server.hpp"
#include "database/DatabaseHelper.hpp"

using namespace mysqlpp;

int main() {
    define_strings();

	signal(SIGINT, Server::HandleSignal);
	signal(SIGABRT, Server::HandleSignal);
	signal(SIGTERM, Server::HandleSignal);


    /* std::string pw = "2tADxYjhUL5FNh5f";
    std::string user = "chars";
    std::string database = "chars";
    std::string host = "localhost";

    DatabaseHelper& h = DatabaseHelper::get_mutable_instance();
    h.Connect(host, database, user, pw); */

    Server::get_mutable_instance().Go();
    return 0;
}
