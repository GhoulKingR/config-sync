#include "defines.hpp"
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <string>
#include <string_view>
#include <chrono>

Shell::Shell(bool dry_run, uint8_t log_level)
: dry_run(dry_run), logger("SHELL", log_level) {}

Shell::Shell()
: logger("SHELL") {}

void Shell::load_config(const Configs &conf) {
    dry_run = conf.dry_run;
    logger.setLevel(conf.level);
}

void Shell::run_command(const std::string &command, const std::string &description) const {
    logger.info(description);
    if (dry_run) {
        logger.info(command, true);
    } else {
        std::system(command.c_str());
    }
}

void Shell::mkdir(std::string_view dir) const {
    run_command(
        std::format("mkdir \"{}\"", dir),
        std::format("Creating directory '{}'", dir)
    );
}

void Shell::del(std::string_view file) const {
    run_command(
        std::format("rm -rf \"{}\"", file),
        std::format("Deleting file/folder {}", file)
    );
}

void Shell::copy(std::string_view src, std::string_view dest) const {
    run_command(
        std::format("cp -r \"{}\" \"{}\"", src, dest),
        std::format("Copying file \"{}\" -> \"{}\"", src, dest)
    );
}

void Shell::set_cwd(const fs::path &dir) const {
    logger.info(std::format("Changing working directory to {}", dir.c_str()));
    fs::current_path(dir);
}

void Shell::zip(std::string_view dir) const {
    run_command(
        std::format("zip -q -r {} .", dir),
        std::format("Zipping current directory to {}", dir)
    );
}

void Shell::unzip(std::string_view dir) const {
    run_command(
        std::format("unzip -q {}", dir),
        std::format("Unzipping {} to current directory", dir)
    );
}

const fs::path Shell::get_cwd() const {
    return fs::current_path();
}


void Shell::git_init() const {
    run_command("git init -b main", "Initializing git repository");
}

void Shell::git_add() const {
    run_command("git add .", "Adding all files to git repository");
}

void Shell::git_commit() const {
    const auto now = std::chrono::system_clock::now();
    run_command(
        std::format("git commit -m \"{:%Y-%m-%d %H:%M:%S UTC}\"", now),
        "Commiting updates to repository"
    );
}

void Shell::git_add_remote(std::string_view remote_name, std::string_view url) const {
    run_command(
        std::format("git remote add {} {}", remote_name, url),
        std::format("Adding remote URL as {}", remote_name)
    );
}

void Shell::git_push(std::string_view remote_name) const {
    run_command(
        std::format("git push {} main", remote_name),
        std::format("Pushing updates to {} repository", remote_name)
    );
}

void Shell::git_pull(std::string_view remote_name) const {
    run_command(
        std::format("git pull {} main --rebase", remote_name),
        std::format("Pushing updates from {}", remote_name)
    );
    git_add();
    git_commit();
}

void Shell::git_clone(std::string_view url, std::string_view dest) const {
    run_command(
        std::format("git clone {} {}", url, dest),
        "Cloning remote repository"
    );
}

void Shell::git_remote() const  {
    run_command(
        "git remote",
        "Viewing remote list"
    );
}

void Shell::list_files() const {
    run_command(
        "ls -l",
        "Listing files in current directory"
    );
}
