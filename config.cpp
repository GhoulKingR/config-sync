#include "defines.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>
#include <utility>

Configs::Configs(int argc, const char **argv, const Shell &shell) :
    dry_run(false),
    level(0),
    home_dir(std::getenv("HOME")),
    url(std::nullopt),
    file(std::nullopt),
    program_name("config-sync"),
    local_dir(fs::path(home_dir) / ("." + program_name)),
    shell(shell)
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
        else if (arg == "status") command = STATUS;
        else command = NONE;
    }

    for (int i = 2; i < argc; i++) {
        const std::string_view arg = argv[i];

        if (arg == "--dry-run") {
            dry_run = true;
        } else if (arg == "-v") {
            level = 1;
        } else if (arg == "-vv") {
            level = 2;
        } else if (arg == "-vvv") {
            level = 3;
        } else if (arg.substr(0, 6) == "--url=") {
            url = arg.substr(6);
        } else if (arg == "-u") {
            url = argv[i + 1];
            i++;
        } else if (arg.substr(0, 7) == "--file=") {
            file = arg.substr(7);
        } else if (arg == "-f") {
            file = argv[i + 1];
            i++;
        } else {
            throw std::runtime_error(std::format("Invalid argument: {}", arg));
        }
    }
}

void Configs::load_config_file() {
    const fs::path config_path = local_dir / "config.toml";

    if (!fs::is_directory(local_dir)) {
        shell.mkdir(local_dir.string());
    }

    // write basic toml to config file if it doesn't exist
    if (!fs::exists(config_path)) {
        std::ofstream outfile(config_path);
        outfile << "[targets]\n";
        outfile.close();
    }

    targets = std::move(toml::parse_file(config_path.string()));
}
