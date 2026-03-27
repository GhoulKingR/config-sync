#include "defines.hpp"
#include <filesystem>
#include <format>
#include <chrono>
#include <stdexcept>
#include <string>

Application::Application(const Configs &confs)
: confs(confs), logger("APPLICATION", confs.level) {}

void Application::run_command(const std::string &command) const {
    if (confs.dry_run) {
        logger.info(command, true);
    } else {
        std::system(command.c_str());
    }
}

void Application::init_remote() const {
    if (!confs.url.has_value()) {
        throw std::runtime_error("You must provide a url");
    }

    // init local folder
    logger.info("Checking for local directory");
    if (!fs::is_directory(confs.local_dir)) {
        throw std::runtime_error("No local directory found, make sure you run the `init` command first");
    }

    logger.info("Initializing local directory for git remote");
    const auto now = std::chrono::system_clock::now();

    run_command(std::format("cd \"{}\"; git init -b main", confs.local_dir.c_str()));
    run_command(std::format("cd \"{}\"; git add .", confs.local_dir.c_str()));
    run_command(std::format("cd \"{}\"; git commit -m \"{:%Y-%m-%d %H:%M:%S UTC}\"", confs.local_dir.c_str(), now));
    run_command(std::format("cd \"{}\"; git remote add origin \"{}\"", confs.local_dir.c_str(), *confs.url));
    run_command(std::format("cd \"{}\"; git branch -u origin/main", confs.local_dir.c_str()));
    logger.info("Done initializing local directory for git remote");
}

void Application::export_zip() const {
    const std::string output_path = confs.file.value_or("export.zip");
    const char *export_folder = ".config-sync-export";
    run_command(std::format("mkdir {}", export_folder));

    const std::string tmux_to = std::format("{}/tmux", export_folder);
    const fs::path tmux_path = confs.home_dir / ".tmux.conf"; 

    // check for tmux config file and copy
    if (fs::exists(tmux_path)) {
        run_command(std::format("mkdir \"{}\"", tmux_to));
        run_command(std::format("cp \"{}\" \"{}\"", tmux_path.c_str(), tmux_to));
    }

    const std::string nvim_to = std::format("{}/nvim", export_folder);
    const fs::path nvim_path = confs.home_dir / ".config/nvim";

    // check for neovim config files
    if (fs::is_directory(nvim_path)) {
        run_command(std::format("mkdir \"{}\"", nvim_to));
        run_command(std::format("cp -r \"{}\" \"{}\"", nvim_path.c_str(), nvim_to));
    }

    run_command(std::format("cd {}; zip -q -r ../{} .", export_folder, output_path));
    run_command(std::format("rm -rf {}", export_folder));
}

void Application::init_local() const {
    // init local folder
    if (!fs::is_directory(confs.local_dir)) {
        run_command(std::format("mkdir \"{}\"", confs.local_dir.c_str()));
    }

    logger.info("Copying tmux configs");
    const fs::path tmux_to = confs.local_dir / "tmux";
    const fs::path tmux_path = confs.home_dir / ".tmux.conf"; 

    // check if tmux has already been copied and delete
    if (fs::is_directory(tmux_to)) {
        run_command(std::format("rm -rf \"{}\"", tmux_to.c_str()));
        logger.info("Deleted previous saved tmux config");
    }

    // check for tmux config file and copy
    if (fs::exists(tmux_path)) {
        run_command(std::format("mkdir \"{}\"", tmux_to.c_str()));
        run_command(std::format("cp \"{}\" \"{}\"/", tmux_path.c_str(), tmux_to.c_str()));
    }
    logger.info("Done copying tmux configs");

    logger.info("Copying neovim configs");
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

    logger.info("Done copying neovim configs");
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
        "\n\nFlags:\n"
        "\t--dry-run\n"
        "\t-v[v[v]]\n"
        "\n";
    printf(msg, confs.program_name.c_str());
}

