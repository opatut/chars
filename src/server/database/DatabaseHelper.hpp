#ifndef DATABASE_DATABASEHELPER_HPP
#define DATABASE_DATABASEHELPER_HPP

#include <iostream>
#include <mysql++.h>
#include <boost/serialization/singleton.hpp>

class DatabaseHelper : public boost::serialization::singleton<DatabaseHelper> {
public:
    DatabaseHelper();
    
    mysqlpp::Connection& GetConnection();
    mysqlpp::Query NewQuery();
    
    void Connect(const std::string& hostname = "", 
            const std::string& database = "", 
            const std::string& username = "", 
            const std::string& password = "");

    void SetUsername(const std::string& username);
    const std::string& GetUsername() const;

    void SetPassword(const std::string& password);
    const std::string& GetPassword() const;

    void SetDatabase(const std::string& database);
    const std::string& GetDatabase() const;

    void SetHostname(const std::string& hostname);
    const std::string& GetHostname() const;
private:
    mysqlpp::Connection mConnection; 
    std::string mUsername;
    std::string mPassword;
    std::string mDatabase;
    std::string mHostname;
};

#endif
