#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>

#include <boost/ptr_container/ptr_map.hpp>

class Logger {
public:
    Logger(); // for boost::ptr_map
    Logger(const std::string& name);

    static Logger& GetLogger(const std::string& name = "default");

    // levels are usually (debug|info|warning|error) or self-defined levels
    void Log(const std::string& level, const std::string& msg);
    std::ostream& GetStream(const std::string& level);

    void Debug(const std::string& msg);
    std::ostream& DebugStream();

    void Info(const std::string& msg);
    std::ostream& InfoStream();

    void Warning(const std::string& msg);
    std::ostream& WarningStream();

    void Error(const std::string& msg);
    std::ostream& ErrorStream();

    void SetName(const std::string& name);
    const std::string& GetName() const;
private:
    static boost::ptr_map<std::string, Logger> Loggers;

    std::string mName;
};

#endif
