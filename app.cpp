#include "defines.hpp"
#include <filesystem>
#include <format>

Application::Application(const Configs &confs)
: confs(confs), logger("APPLICATION") {}

void Application::run_command(const std::string &command) {
    if (confs.dry_run) {
        logger.info(command, true);
    } else {
        std::system(command.c_str());
    }
}

void Application::init_local() {
    // init local folder
    if (!fs::is_directory(confs.local_dir)) {
        run_command(std::format("mkdir \"{}\"", confs.local_dir.c_str()));
    }

    run_command(std::format("cd \"{}\"", confs.local_dir.c_str()));

    logger.log("Copying tmux configs");
    const fs::path tmux_to = confs.local_dir / "tmux";
    const fs::path tmux_path = confs.home_dir / ".tmux.conf"; 

    // check if tmux has already been copied and delete
    if (fs::is_directory(tmux_to)) {
        run_command(std::format("rm -rf \"{}\"", tmux_to.c_str()));
        logger.log("Deleted previous saved tmux config");
    }

    // check for tmux config file and copy
    if (fs::exists(tmux_path)) {
        run_command(std::format("mkdir \"{}\"", tmux_to.c_str()));
        run_command(std::format("cp \"{}\" \"{}\"/", tmux_path.c_str(), tmux_to.c_str()));
    }
    logger.log("Done copying tmux configs");

    logger.log("Copying neovim configs");
    const fs::path nvim_to = confs.local_dir / "neovim";
    const fs::path nvim_path = confs.home_dir / ".config/nvim";

    // check if neovim has been copied before and delete
    if (fs::is_directory(nvim_to)) {
        run_command(std::format("rm -rf \"{}\"", nvim_to.c_str()));
        logger.log("Deleted previous saved neovim config");
    }

    // check for neovim config files
    if (fs::is_directory(nvim_path)) {
        run_command(std::format("mkdir \"{}\"", nvim_to.c_str()));
        run_command(std::format("cp -r \"{}\" \"{}\"/", nvim_path.c_str(), nvim_to.c_str()));
    }

    logger.log("Done copying neovim configs");
}

void Application::print_help() const {
    const char *msg = "\n"
        "Usage: %s [command] [flags]\n"
        "\n\nCommands:\n"
        "\tinit\n"
        "\tremote\n"
        "\tclone\n"
        "\tpull\n"
        "\tpush\n"
        "\texport\n"
        "\timport\n"
        "\n";
    printf(msg, confs.program_name.c_str());
}

