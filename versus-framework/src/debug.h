#pragma once

namespace vsf
{
    enum class LogLevel
    {
        Info,
        Error
    };

    void log(LogLevel level, const char* format, ...);
}

#define LOG_ERROR(format, ...) vsf::log(vsf::LogLevel::Error, format, __VA_ARGS__)

#if _DEBUG
    #define LOG_INFO(format, ...) vsf::log(vsf::LogLevel::Info, format, __VA_ARGS__)
#else
    #define LOG_INFO
#endif
