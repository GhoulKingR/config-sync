#ifndef COMMANDS_HPP__
#define COMMANDS_HPP__

#include <cstdint>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

#define PROGRAM_NAME "config-sync"

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
    std::string program_name;
    fs::path home_dir;
    fs::path local_dir;
    uint8_t level;

    static Configs *init(int argc, const char **argv);
    Configs(int argc, const char **argv);

private:
    Logger logger;
};

/// app.cpp
class Application {
    const Configs &confs;
    const Logger logger;

    void run_command(const std::string &command);

public:
    Application(const Configs &confs);
    void init_local();
    void print_help() const;
};

void print_help(const std::string &program_name);

#endif
