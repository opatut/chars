#ifndef DATABASE_OPTIONS_HPP
#define DATABASE_OPTIONS_HPP

#include <map>

#include <boost/lexical_cast.hpp>

#include <mysql++.h>

#include "DatabaseHelper.hpp"

class Options {
public:
    Options();

    void Load();
    void Save();

    std::string GetString(const std::string& key, const std::string& def = "");
    int GetInt(const std::string& key, int def = 0);
    float GetFloat(const std::string& key, float def = 0.f);
    bool GetBool(const std::string& key, bool def = false);

    void SetString(const std::string& key, const std::string& value);
    void SetInt(const std::string& key, int value);
    void SetFloat(const std::string& key, float value);
    void SetBool(const std::string& key, bool value);
private:
    std::map<std::string, std::string> mMap;
};

#endif
