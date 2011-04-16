#include "Logger.hpp"
#include <boost/algorithm/string.hpp>

boost::ptr_map<std::string, Logger> Logger::Loggers;

Logger::Logger() {
    SetName("Unnamed Logger");
}

Logger::Logger(const std::string& name) {
    SetName(name);
}

Logger& Logger::GetLogger(const std::string& name) {
    // create logger with name if not exists
    if(Loggers.find(name) == Loggers.end()) {
        Loggers[name] = Logger(name);
    }
    return Loggers[name];
}

void Logger::Log(const std::string& level, const std::string& msg) {
    std::string lvl = boost::to_upper_copy(level);
    GetStream(level) << "[" << GetName() << "] " << lvl << " - " << msg << std::endl;
}

std::ostream& Logger::GetStream(const std::string& level) {
    std::string lvl = boost::to_upper_copy(level);
    if(lvl == "ERROR" || lvl == "CRITICAL") {
        return std::cerr;
    } else {
        return std::cout;
    }
}

void Logger::Debug(const std::string& msg) {
    Log("DEBUG", msg);
}
std::ostream& Logger::DebugStream() {
    return GetStream("DEBUG");
}

void Logger::Info(const std::string& msg) {
    Log("INFO", msg);
}
std::ostream& Logger::InfoStream() {
    return GetStream("INFO");
}

void Logger::Warning(const std::string& msg) {
    Log("WARNING", msg);
}
std::ostream& Logger::WarningStream() {
    return GetStream("WARNING");
}

void Logger::Error(const std::string& msg) {
    Log("ERROR", msg);
}
std::ostream& Logger::ErrorStream() {
    return GetStream("ERROR");
}

void Logger::SetName(const std::string& name) {
    mName = name;
}

const std::string& Logger::GetName() const {
    return mName;
}
