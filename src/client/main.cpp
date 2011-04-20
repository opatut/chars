#include <iostream>

#include "Client.hpp"

#include "common/Strings.hpp"
#include "common/config/Configuration.hpp"
#include "common/Logger.hpp"

int main() {
    define_strings();
    Client::get_mutable_instance().Go();
    return 0;
}
