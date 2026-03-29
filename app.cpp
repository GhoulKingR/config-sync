#include "defines.hpp"
#include <cstdio>
#include <filesystem>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

Application::Application(Configs &confs, const Shell &shell)
: confs(confs), shell(shell), logger("APPLICATION", confs.level) {}

void Application::init_remote() const {
    if (!confs.url.has_value()) {
        throw std::runtime_error("You must provide a url");
    }

    if (!confs.name.has_value()) {
        throw std::runtime_error("You must provide a name to label remote URL");
    }

    // init local folder
    logger.info("Checking for local directory");
    if (!fs::is_directory(confs.local_dir)) {
        throw std::runtime_error("No local directory found, make sure you run the `init` command first");
    }

    const fs::path former = shell.get_cwd();
    shell.set_cwd(confs.local_dir);

    logger.info("Initializing local directory for git remote");

    shell.git_init();
    shell.git_add();
    shell.git_commit();
    shell.git_add_remote(confs.name.value(), confs.url.value());
    shell.set_cwd(former);
    logger.info("Done initializing local directory for git remote");
}

void Application::export_zip() const {
    const std::string output_path = confs.file.value_or("export.zip");
    const std::string_view export_folder = ".config-sync-export";
    shell.mkdir(export_folder);

    update_dir(confs.local_dir);
    update_dir(export_folder);

    const fs::path current_path = shell.get_cwd();
    shell.set_cwd(export_folder);
    shell.zip("../" + output_path);
    shell.del(export_folder);
    shell.set_cwd(current_path);
    shell.del(export_folder);
}

void Application::update_dir(const fs::path &dest_dir) const {
    // Read [targets] sub-table from toml file
    confs.targets["targets"].as_table()->for_each(
        [this, &dest_dir](const toml::key &key, const toml::node &val) {
            // get key and value for the current entry
            const std::string_view dest_name = key.str();
            const std::string &src  = val.as_string()->get();

            logger.info(std::format("Copying {} configs", dest_name));
            const fs::path dest = dest_dir / dest_name;

            // check if file has already been copied and delete
            if (fs::is_directory(dest)) {
                shell.del(dest.c_str());
            }

            // check if config file/folder already exists and copy to
            // local_dir
            if (fs::exists(src)) {
                shell.mkdir(dest.c_str());
                shell.copy(src, dest.c_str());
            }
            logger.info(std::format("Done copying {} configs", dest_name));
        }
    );
}


void Application::load_dir(const fs::path &src_dir) const {
    confs.targets["targets"].as_table()->for_each(
        [this, &src_dir](const toml::key &key, const toml::node &val) {
            const std::string_view src_name = key.str();
            const fs::path &dest  = val.as_string()->get();

            logger.info(std::format("Copying {} configs", src_name));
            const fs::path src = src_dir / src_name;

            // check if source config folder exists
            if (!fs::is_directory(src)) {
                return;
            }

            shell.del(dest.c_str());
            shell.copy((src / dest.filename()).c_str(), dest.c_str());
            logger.info(std::format("Done copying {} configs", src_name));
        }
    );
}

void Application::init_local() const {
    // Make sure local_dir exists
    if (!fs::is_directory(confs.local_dir)) {
        throw std::runtime_error("Config file/directory missing");
    }

    update_dir(confs.local_dir);
}

void Application::pull_remote() const {
    // check if local dir exists
    if (!fs::is_directory(confs.local_dir)) {
        throw std::runtime_error("Config directory missing");
    }

    if (!confs.name.has_value()) {
        throw std::runtime_error("You must provide a name for the remote URL to pull from");
    }
    
    // check if it's a git repository
    if (!fs::is_directory(confs.local_dir / ".git")) {
        throw std::runtime_error("Config directory hasn't been initialized yet");
    }

    const fs::path former = shell.get_cwd();
    shell.set_cwd(confs.local_dir);
    shell.git_pull(confs.name.value());
    shell.set_cwd(former);

    confs.load_config_file();
    load_dir(confs.local_dir);
}

void Application::import_zip() const {
    if (!confs.file.has_value()) {
        throw std::runtime_error("import command requires a file");
    }

    const fs::path file = confs.file.value();
    if (!fs::exists(file)) {
        throw std::runtime_error(std::format("File '{}' not found", file.c_str()));
    }

    printf("This action is destructive and will wipe your existing configurations. Do you want to continue? (yN) ");
    if (confs.dry_run) {
        printf("y\n");
    } else {
        char c = std::getchar();
        if (c != 'y' && c != 'Y') {
            return;
        }
    }

    shell.del(confs.local_dir.c_str());
    shell.mkdir(confs.local_dir.c_str());
    shell.copy(file.c_str(), confs.local_dir.c_str());

    const fs::path former = shell.get_cwd();
    shell.set_cwd(confs.local_dir);
    shell.unzip(file.c_str());
    shell.del(file.c_str());

    shell.set_cwd(former);
    confs.load_config_file();
    load_dir(confs.local_dir);
}

void Application::push_remote() const {
    // check if local dir exists
    if (!fs::is_directory(confs.local_dir)) {
        throw std::runtime_error("Config directory missing");
    }

    if (!confs.name.has_value()) {
        throw std::runtime_error("You must provide a name for the remote URL label");
    }
    
    // check if it's a git repository
    if (!fs::is_directory(confs.local_dir / ".git")) {
        throw std::runtime_error("Config directory hasn't been initialized yet");
    }

    const fs::path former = shell.get_cwd();
    shell.set_cwd(confs.local_dir);
    shell.git_push(confs.name.value());
    shell.set_cwd(former);
}

void Application::clone_remote() const {
    if (!confs.url.has_value()) {
        throw std::runtime_error("You must provide a url");
    }

    // delete local dir if it exists
    if (fs::is_directory(confs.local_dir)) {
        shell.del(confs.local_dir.c_str());
    }

    printf("This action is destructive and will wipe your existing configurations. Do you want to continue? (yN) ");
    if (confs.dry_run) {
        printf("y\n");
    } else {
        char c = std::getchar();
        if (c != 'y' && c != 'Y') {
            return;
        }
    }

    // clone repo to local_dir
    shell.git_clone(confs.url.value().c_str(), confs.local_dir.c_str());
    confs.load_config_file();
    load_dir(confs.local_dir);
}


void Application::print_status() const {
    std::cout << "Local setup initialized: ";
    
    if (!fs::is_directory(confs.local_dir)) {
        std::cout << "No\n";
        return;
    }

    std::cout << "Yes\n";

    const fs::path former = shell.get_cwd();
    shell.set_cwd(confs.local_dir);

    std::cout << "\nRemote list:\n";
    shell.git_remote();

    std::cout << "\nCurrent files: \n";
    shell.list_files();
}


void Application::print_help() const {
    const char *msg = "\n"
        "Usage: %s [command] [flags]\n"
        "\n\nCommands:\n"
        "\tinit\t\tInitialize config-sync on your system\n"
        "\tremote\t\tSet a remote git repository for this project\n"
        "\tclone\t\tClone an existing config repository\n"
        "\tpull\t\tPull updates from remote repository\n"
        "\tpush\t\tPush recent updates to remote repository\n"
        "\texport\t\tExport neovim and tmux configs to zip file\n"
        "\timport\t\tImport neovim and tmux configs from zip file\n"
        "\tstatus\t\tShow the status of local setup\n"
        "\thelp\t\tShow this help text\n"
        "\n\nFlags:\n"
        "\t--dry-run\n"
        "\t-v[v[v]]\n"
        "\t-f FILE, --file=FILE\n"
        "\t-u URL, --url=URL\n"
        "\n";
    printf(msg, confs.program_name.c_str());
}

