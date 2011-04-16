#include <iostream>

#include "Client.hpp"

#include "common/Strings.hpp"
#include "common/config/Configuration.hpp"
#include "common/Logger.hpp"

int main() {
    define_strings();

    /*MainGameState mgs;
    std::cout << "Name = " << mgs.GetName() << std::endl;*/

    Client& c = Client::get_mutable_instance();
    c.LoadConfig();
    c.StartupOgre();
    c.RunLoop();
    return 0;
}
