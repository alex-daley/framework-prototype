#include "debug.h"
#include <cstdarg>
#include <cstdio>

namespace
{
    std::size_t calculateLength(const char* format, va_list args)
    {
        return std::vsnprintf(nullptr, 0, format, args);;
    }

    void log_internal(vsf::LogLevel level, const char* message)
    {
        switch (level)
        {
            case vsf::LogLevel::Info:
                std::printf("\u001B[32mINFO:\u001B[0m %s\n", message);
                break;

            case vsf::LogLevel::Error:
                std::printf("\u001B[31mERROR:\u001B[0m %s\n", message);
                break;
        }
    }

    void log_internal(vsf::LogLevel level, const char* format, va_list args)
    {
        std::size_t length = calculateLength(format, args) + 1;
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
