#include "Options.hpp"

Options::Options() {

}

void Options::Load() {
    mysqlpp::Query q = DatabaseHelper::get_mutable_instance().NewQuery();
    q << "SELECT * FROM options";
    mysqlpp::StoreQueryResult res = q.store();
    for(size_t i = 0; i < res.num_rows(); ++i) {
        std::string name, value;
        res[i]["name"].to_string(name);
        res[i]["value"].to_string(value);
        SetString(name, value);
    }
}

void Options::Save() {
    mysqlpp::Query q = DatabaseHelper::get_mutable_instance().NewQuery();
    q << "INSERT INTO options (name, value) VALUES (%0q, %1q) ON DUPLICATE KEY UPDATE value = %1q";
    q.parse();
    for(auto iter = mMap.begin(); iter != mMap.end(); ++iter) {
        q.execute(iter->first, iter->second);
    }
}

std::string Options::GetString(const std::string& key, const std::string& def) {
    if(mMap.find(key) != mMap.end())
        return mMap[key];
    return def;
}

int Options::GetInt(const std::string& key, int def) {
    std::string v = GetString(key, boost::lexical_cast<std::string>(def));
    return boost::lexical_cast<int>(v);
}

float Options::GetFloat(const std::string& key, float def) {
    std::string v = GetString(key, boost::lexical_cast<std::string>(def));
    return boost::lexical_cast<float>(v);
}

bool Options::GetBool(const std::string& key, bool def) {
    return (GetInt(key, (def ? 1 : 0) ) != 0);
}

void Options::SetString(const std::string& key, const std::string& value) {
    mMap[key] = value;
}

void Options::SetInt(const std::string& key, int value) {
    SetString(key, boost::lexical_cast<std::string>(value));
}

void Options::SetFloat(const std::string& key, float value) {
    SetString(key, boost::lexical_cast<std::string>(value));
}

void Options::SetBool(const std::string& key, bool value) {
    SetString(key, boost::lexical_cast<std::string>(value));
}

