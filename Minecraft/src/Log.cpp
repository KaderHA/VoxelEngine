#include "Log.hpp"

std::shared_ptr<spdlog::logger> Log::m_logger;

void Log::init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_logger = spdlog::stdout_color_mt("Minecraft");
    m_logger->set_level(spdlog::level::trace);
}
