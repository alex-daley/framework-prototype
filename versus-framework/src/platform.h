#pragma once
#include "application.h"
#include "graphics.h"
#include <memory>
#include <string>

namespace vsf 
{
    // Wraps platform specific code implemented in a platform_{platform-name}.cpp file.
    // This header should not be exposed to consumer programs.
    namespace platform 
    {
        bool initialise(const Config& config);
        void run(UpdateHooks hooks);
        void shutdown();

        std::unique_ptr<ITexture> load_texture(const std::string& path);
    }
}
