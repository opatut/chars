#include "Logger.hpp"
#include <boost/algorithm/string.hpp>

boost::ptr_map<std::string, Logger> Logger::Loggers;

Logger::Logger() {
    SetName("default");
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
    LogStream* s = GetStream(lvl);
    s->Output(this, msg);
}

LogStream* Logger::GetStream(const std::string& streamname) {
    std::string name = boost::to_upper_copy(streamname);
    for(auto iter = mStreams.begin(); mStreams.end() != iter; ++iter) {
        std::string sname = boost::to_upper_copy(iter->GetName());
        if(name == sname) {
            return &(*iter);
        }
    }
    mStreams.push_back(new LogStream(name));
    return &mStreams.back();
}


void Logger::Debug(const std::string& msg) {
    Log("DEBUG", msg);
}

void Logger::Info(const std::string& msg) {
    Log("INFO", msg);
}

void Logger::Warning(const std::string& msg) {
    Log("WARNING", msg);
}

void Logger::Error(const std::string& msg) {
    Log("ERROR", msg);
}

void Logger::SetName(const std::string& name) {
    mName = name;
}

const std::string& Logger::GetName() const {
    return mName;
}
