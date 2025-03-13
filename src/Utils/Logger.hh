#pragma once
#include <spdlog/spdlog.h>

class Logger
{
public:
    Logger();

    std::unique_ptr<spdlog::logger> &GetInner() { return m_Logger; }

private:
    std::unique_ptr<spdlog::logger> m_Logger;
};

inline Logger *GetLogger()
{
    static Logger instance;
    return &instance;
}

#define SPDLOG_LOG(type, ...) SPDLOG_LOGGER_##type(GetLogger()->GetInner(), __VA_ARGS__)

#define LogInfo(...) SPDLOG_LOG(INFO, __VA_ARGS__)
#define LogWarn(...) SPDLOG_LOG(WARN, __VA_ARGS__)
#define LogError(...) SPDLOG_LOG(ERROR, __VA_ARGS__)
#define LogCritical(...) SPDLOG_LOG(CRITICAL, __VA_ARGS__)
#define LogDebug(...) SPDLOG_LOG(DEBUG, __VA_ARGS__)
#define LogTrace(...) SPDLOG_LOG(TRACE, __VA_ARGS__)
