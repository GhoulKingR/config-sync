#ifndef COMMANDS_HPP__
#define COMMANDS_HPP__

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>
namespace fs = std::filesystem;

enum Command {
    INIT_LOCAL,
    INIT_REMOTE,
    CLONE_REMOTE,
    PULL_REMOTE,
    PUSH_REMOTE,
    EXPORT_ZIP,
    IMPORT_ZIP,
    HELP,
    NONE,
};

/// logger.cpp
class Logger {
    uint8_t level;
    const char *module_name;
    void log_message(const std::string &type, const std::string &text) const;

public:
    Logger(const char *module_name);
    Logger(const char *module_name, uint8_t log_level);
    void setLevel(uint8_t level);
    void info(const std::string &) const;
    void info(const std::string &, bool) const;
    void error(const std::string &) const;
    void log(const std::string &) const;
};


/// shell.cpp
struct Configs;
class Shell {
    bool dry_run;
    Logger logger;

    void run_command(const std::string &, const std::string &) const;

public:
    Shell();
    Shell(bool dry_run, uint8_t log_level);
    void load_config(const Configs &);

    void mkdir(std::string_view) const;
    void del(std::string_view) const;
    void copy(std::string_view, std::string_view) const;
    void zip(std::string_view) const;
    void unzip(std::string_view) const;
    void set_cwd(const fs::path &) const;
    const fs::path get_cwd() const;

    // git commands
    void git_init() const;
    void git_add() const;
    void git_commit() const;
    void git_add_remote(std::string_view) const;
    void git_clone(std::string_view, std::string_view) const;
    void git_push() const;
    void git_pull() const;
};

/// config.cpp
struct Configs {
    Command command;
    bool dry_run;
    uint8_t level;
    std::optional<std::string> url;
    std::optional<std::string> file;
    toml::table targets;

    const std::string program_name;
    const fs::path home_dir;
    const fs::path local_dir;
    const Shell &shell;

    Configs(int argc, const char **argv, const Shell &shell);
    void load_config_file();
};

/// app.cpp
class Application {
    Configs &confs;
    const Shell &shell;
    const Logger logger;

    void update_dir(const fs::path &) const;
    void load_dir(const fs::path &) const;
public:
    Application(Configs &confs, const Shell &shell);
    void init_local() const;
    void init_remote() const;
    void push_remote() const;
    void pull_remote() const;
    void clone_remote() const;
    void print_help() const;
    void export_zip() const;
    void import_zip() const;
};

#endif
