#include <iostream>
#include <mysql++.h>
#include <signal.h>

#include "common/Strings.hpp"
#include "common/log/Logger.hpp"
#include "Server.hpp"
#include "database/DatabaseHelper.hpp"

using namespace mysqlpp;

int main() {
    Logger::GetLogger().GetStream("debug")->SetFormat(  LogStream::COLOR_CYAN   + "%2$8s: " + LogStream::COLOR_NONE + "%3$s");
    Logger::GetLogger().GetStream("info")->SetFormat(   LogStream::COLOR_BLUE   + "%2$8s: " + LogStream::COLOR_NONE + "%3$s");
    Logger::GetLogger().GetStream("warning")->SetFormat(LogStream::COLOR_YELLOW + "%2$8s: " + LogStream::COLOR_NONE + "%3$s");
    Logger::GetLogger().GetStream("error")->SetFormat(  LogStream::COLOR_RED    + "%2$8s: " + LogStream::COLOR_NONE + "%3$s");

    define_strings();

	signal(SIGINT, Server::HandleSignal);
	signal(SIGABRT, Server::HandleSignal);
	signal(SIGTERM, Server::HandleSignal);


    std::string pw = "2tADxYjhUL5FNh5f";
    std::string user = "chars";
    std::string database = "chars";
    std::string host = "localhost";

    DatabaseHelper& h = DatabaseHelper::get_mutable_instance();
    h.Connect(host, database, user, pw);

    Server::get_mutable_instance().Go();
    return 0;
}
