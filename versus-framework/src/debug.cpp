#include "debug.h"
#include <cstdarg>
#include <cstdio>

namespace
{
    std::size_t calculate_length(const char* format, va_list args)
    {
        return std::vsnprintf(nullptr, 0, format, args);;
    }

    void log_internal(vsf::LogLevel level, const char* message)
    {
        switch (level)
        {
            case vsf::LogLevel::Info:
                std::printf("INFO: %s\n", message);
                break;

            case vsf::LogLevel::Error:
                std::printf("ERROR: %s\n", message);
                break;
        }
    }

    void log_internal(vsf::LogLevel level, const char* format, va_list args)
    {
        std::size_t length = calculate_length(format, args) + 1;
        char* message = new char[length];
        std::vsnprintf(message, length, format, args);
        log_internal(level, message);
        delete[] message;
    }
}

namespace vsf
{
    void log(LogLevel level, const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        log_internal(level, format, args);
        va_end(args);
    }
}
