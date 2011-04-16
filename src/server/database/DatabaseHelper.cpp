#include "DatabaseHelper.hpp"

DatabaseHelper::DatabaseHelper() {
    mDatabase = "";
    mHostname = "localhost";
    mUsername = "";
    mPassword = "";
}

mysqlpp::Connection& DatabaseHelper::GetConnection() {
    return mConnection;
}

mysqlpp::Query DatabaseHelper::NewQuery() {
    return mConnection.query();
}

void DatabaseHelper::Connect(const std::string& hostname, 
        const std::string& database,  
        const std::string& username, 
        const std::string& password) {

    mConnection = mysqlpp::Connection(false);

    std::string db = (database != "" ? database : mDatabase);
    std::string hn = (hostname != "" ? hostname : mHostname);
    std::string un = (username != "" ? username : mUsername);
    std::string pw = (password != "" ? password : mPassword);

    mConnection.connect(&db[0], &hn[0], &un[0], &pw[0]);
}

void DatabaseHelper::SetUsername(const std::string& username) {
    mUsername = username;
}

const std::string& DatabaseHelper::GetUsername() const {
    return mUsername;
}

void DatabaseHelper::SetPassword(const std::string& password) {
    mPassword = password;
}

const std::string& DatabaseHelper::GetPassword() const {
    return mPassword;
}

void DatabaseHelper::SetDatabase(const std::string& database) {
    mDatabase = database;
}

const std::string& DatabaseHelper::GetDatabase() const {
    return mDatabase;
}

void DatabaseHelper::SetHostname(const std::string& hostname) {
    mHostname = hostname;
}

const std::string& DatabaseHelper::GetHostname() const {
    return mHostname;
}

