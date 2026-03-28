#include <cstdlib>
#include <exception>
#include <string>

#include "defines.hpp"

int main(int argc, const char **argv) {
    Shell shell;
    Configs opts(argc, argv, shell);

    // Load options into shell then load config files
    // The order matters here
    shell.load_config(opts);
    opts.load_config_file();
    
    const Logger logger("MAIN", opts.level);
    const Application app(opts, shell);

    try {
        switch (opts.command) {
            case INIT_LOCAL:
                app.init_local();
                break;
            case INIT_REMOTE:
                app.init_remote();
                break;
            case PUSH_REMOTE:
                app.push_remote();
                break;
            case PULL_REMOTE:
                app.pull_remote();
                break;
            case EXPORT_ZIP:
                app.export_zip();
                break;
            case IMPORT_ZIP:
                app.import_zip();
                break;
            case CLONE_REMOTE:
                app.clone_remote();
                break;
            case STATUS:
                app.print_status();
                break;
            case HELP:
            default:
                app.print_help();
                break;
        }
    } catch (const std::exception &e) {
        logger.error(e.what());
    }

    return 0;
}




