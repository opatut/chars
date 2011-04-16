#include "Server.hpp"

Server::Server() {}

void Server::Load() {
    mOptions.Load();
}

void Server::Save() {
    mOptions.Save();
}

Options& Server::GetOptions() {
    return mOptions;
}
