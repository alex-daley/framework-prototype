#pragma once
#include <string>

namespace vsf 
{
    struct VideoConfig;
    struct Config;

    struct UpdateTime;
    struct UpdateHooks;
    
    namespace application 
    {
        bool initialise(const Config& config);
        void run(UpdateHooks hooks);
        void shutdown();
    }

    struct VideoConfig 
    {
        int resolution_x;
        int resolution_y;
        bool use_vsync;
    };

    struct Config 
    {
        std::string title;
        VideoConfig video{};
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
    };
}