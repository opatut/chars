#include <iostream>

#include "Client.hpp"

#include "common/Strings.hpp"
#include "common/config/Configuration.hpp"
#include "common/log/Logger.hpp"

int main() {
    Logger::GetLogger().GetStream("debug")->SetFormat(  LogStream::COLOR_CYAN   + "%2$8s: " + LogStream::COLOR_NONE + "%3$s");
    Logger::GetLogger().GetStream("info")->SetFormat(   LogStream::COLOR_BLUE   + "%2$8s: " + LogStream::COLOR_NONE + "%3$s");
    Logger::GetLogger().GetStream("warning")->SetFormat(LogStream::COLOR_YELLOW + "%2$8s: " + LogStream::COLOR_NONE + "%3$s");
    Logger::GetLogger().GetStream("error")->SetFormat(  LogStream::COLOR_RED    + "%2$8s: " + LogStream::COLOR_NONE + "%3$s");

    define_strings();
    Client::get_mutable_instance().Go();
    return 0;
}
