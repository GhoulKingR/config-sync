#include "defines.hpp"
#include <chrono>
#include <cstdint>
#include <format>
#include <cstdio>
#include <string>

Logger::Logger(const char *module_name) : level(0), module_name(module_name) {}
Logger::Logger(const char *module_name, uint8_t level) : level(level), module_name(module_name) {}

void Logger::setLevel(uint8_t level) {
    this->level = level;
}

void Logger::log_message(const std::string &type, const std::string &text) const {
    const auto now = std::chrono::system_clock::now();
    std::string formatted_utc = std::format("{:%Y-%m-%d %H:%M:%S UTC}", now);
    printf("[%s] [%s] [%s] %s\n", module_name, type.c_str(), formatted_utc.c_str(), text.c_str());
}

void Logger::log(const std::string &text) const {
#if DEBUG
    log_message("LOG", text);
#else
    const auto now = std::chrono::system_clock::now();
    std::string formatted_utc = std::format("{:%Y-%m-%d %H:%M:%S UTC}", now);
    printf("[%s] %s\n", formatted_utc.c_str(), text.c_str());
#endif
}

void Logger::info(const std::string &text) const {
    info(text, false);
}

void Logger::info(const std::string &text, bool dryrun) const {
    if (dryrun) {
        printf("[DRY RUN] [%s] %s\n", module_name, text.c_str());
        return;
    }

    if (level < 3) return;
    log_message("INFO", text);
}

void Logger::error(const std::string &text) const {
    log_message("ERROR", text);
}
