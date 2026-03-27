#include <cstdlib>
#include <exception>
#include <string>

#include "defines.hpp"

int main(int argc, const char **argv) {
    Logger logger("MAIN");
    Configs opts(argc, argv);
    Application app(opts);

    try {
        switch (opts.command) {
            case INIT_LOCAL:
                app.init_local();
                break;

            default:
                app.print_help();
                break;
        }
    } catch (const std::exception &e) {
        logger.error(e.what());
    }

    return 0;
}




