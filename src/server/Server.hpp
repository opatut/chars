#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/serialization/singleton.hpp>

#include "database/Options.hpp"

class Server : public boost::serialization::singleton<Server> {
public:
    Server();

    void Load();
    void Save();

    Options& GetOptions();
private:
    Options mOptions;
};

#endif
