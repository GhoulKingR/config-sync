#include "defines.hpp"
#include <chrono>
#include <cstdint>
#include <format>
#include <iostream>
#include <string>
#include <string_view>

Logger::Logger(std::string_view module_name) : level(0), module_name(module_name) {}
Logger::Logger(std::string_view module_name, uint8_t level) : level(level), module_name(module_name) {}

void Logger::setLevel(uint8_t level) {
    this->level = level;
}

void Logger::log_message(std::string_view type, std::string_view text) const {
    const auto now = std::chrono::system_clock::now();
    std::string formatted_utc = std::format("{:%Y-%m-%d %H:%M:%S UTC}", now);
    std::cout << std::format("[{}] [{}] [{}] {}\n", module_name, type, formatted_utc.c_str(), text);
}

void Logger::log(std::string_view text) const {
#if DEBUG
    log_message("LOG", text);
#else
    std::cout << text << '\n';
#endif
}

void Logger::info(std::string_view text) const {
    info(text, false);
}

void Logger::info(std::string_view text, bool dryrun) const {
    if (dryrun) {
        std::cout << std::format("[DRY RUN] [{}] {}\n", module_name, text);
        return;
    }

    if (level < 3) return;
    log_message("INFO", text);
}

void Logger::error(std::string_view text) const {
    log_message("ERROR", text);
}
