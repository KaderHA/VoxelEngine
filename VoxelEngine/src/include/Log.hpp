#pragma once
#include <memory.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

class Log {
public:
    static void init();

    inline static std::shared_ptr<spdlog::logger> getLogger() { return m_logger; }

private:
    static std::shared_ptr<spdlog::logger> m_logger;
};