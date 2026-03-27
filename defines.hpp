#ifndef COMMANDS_HPP__
#define COMMANDS_HPP__

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
namespace fs = std::filesystem;

enum Command {
    INIT_LOCAL,
    INIT_REMOTE,
    CLONE_REMOTE,
    PULL_REMOTE,
    PUSH_REMOTE,
    EXPORT_ZIP,
    IMPORT_ZIP,
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

/// config.cpp
struct Configs {
    Command command;
    bool dry_run;
    uint8_t level;
    std::optional<std::string> url;
    std::optional<std::string> file;

    const std::string program_name;
    const fs::path home_dir;
    const fs::path local_dir;

    Configs(int argc, const char **argv);
};

/// app.cpp
class Application {
    const Configs &confs;
    const Logger logger;

    void run_command(const std::string &command) const;

public:
    Application(const Configs &confs);
    void init_local() const;
    void init_remote() const;
    void print_help() const;
    void export_zip() const;
};

void print_help(const std::string &program_name);

#endif
