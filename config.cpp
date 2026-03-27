#include "defines.hpp"
#include <stdexcept>

Configs::Configs(int argc, const char **argv) :
    logger("CONFIGS"),
    dry_run(false),
    level(0),
    home_dir(std::getenv("HOME"))
{
    if (argc < 2) command = NONE;
    else {
        const std::string arg = argv[1];
        if (arg == "init") command = INIT_LOCAL;
        else if (arg == "remote") command = INIT_REMOTE;
        else if (arg == "clone") command = CLONE_REMOTE;
        else if (arg == "pull") command = PULL_REMOTE;
        else if (arg == "push") command = PUSH_REMOTE;
        else if (arg == "export") command = EXPORT_ZIP;
        else if (arg == "import") command = IMPORT_ZIP;
        else command = NONE;
    }

    for (int i = 2; i < argc; i++) {
        const std::string arg = argv[i];

        if (arg == "--dry-run") {
            dry_run = true;
        } else if (arg == "-v") {
            level = 1;
        } else if (arg == "-vv") {
            level = 2;
        } else if (arg == "-vvv") {
            level = 3;
        } else {
            throw std::runtime_error("Invalid argument" + arg);
        }
    }

    logger.setLevel(level);
    this->program_name = PROGRAM_NAME;
    local_dir = fs::path(home_dir) / ("." + program_name);
}

