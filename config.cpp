#include "defines.hpp"
#include <optional>
#include <stdexcept>

Configs::Configs(int argc, const char **argv) :
    dry_run(false),
    level(0),
    home_dir(std::getenv("HOME")),
    url(std::nullopt),
    file(std::nullopt),
    program_name("config-sync"),
    local_dir(fs::path(home_dir) / ("." + program_name))
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
        else if (arg == "help") command = HELP;
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
        } else if (arg.substr(0, 4) == "http" || arg.substr(0, 4) == "ssh:") {
            url = arg;
        } else if (arg.substr(0, 7) == "--file=") {
            file = arg.substr(7);
        } else if (arg == "-f") {
            file = argv[i + 1];
            i++;
        } else {
            throw std::runtime_error("Invalid argument" + arg);
        }
    }
}

