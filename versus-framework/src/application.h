#pragma once
#include "graphics.h"
#include <string>

namespace vsf 
{
    struct VideoConfig 
    {
        int resolution_x;
        int resolution_y;
        bool use_vsync;
    };

    struct TypographyConfig
    {
        std::string font_path;
        int font_size;
    };

    struct Config 
    {
        std::string title;
        VideoConfig video{};
        TypographyConfig typography{};
    };

    struct UpdateTime
    {
        int update_count;
        float time;
        float delta_time;
    };

    struct UpdateHooks 
    {
        void (*update)(const UpdateTime&);
        void (*draw)(ISpriteBatch&);
        void (*draw_gui)(IGuiBatch&);
    };

    namespace application
    {
        bool initialise(const Config& config);
        void run(UpdateHooks hooks);
        void shutdown();
    }
}
