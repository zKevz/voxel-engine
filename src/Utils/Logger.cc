#include "Logger.hh"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

Logger::Logger()
{
    std::vector<spdlog::sink_ptr> sinks {
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::always),
        std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(10),
    };

#ifndef NDEBUG
    spdlog::level::level_enum level = spdlog::level::trace;
#else
    spdlog::level::level_enum level = spdlog::level::info;
#endif

    for (auto &sink : sinks) { sink->set_level(level); }

    m_Logger = std::make_unique<spdlog::logger>("Voxel", sinks.begin(), sinks.end());
    m_Logger->set_level(level);
}
