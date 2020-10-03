#include "vsf.h"

namespace 
{
    void update(const vsf::UpdateTime& time)
    {
        LOG_INFO("Frame count: %i, Seconds: %f, Delta Time: %f", time.update_count, time.time, time.delta_time);
    }
}

int main() 
{
    vsf::Config config;
    config.video.resolution_x = 1280;
    config.video.resolution_y = 720;
    config.video.use_vsync = true;

    if (!vsf::application::initialise(config)) 
    {
        return -1;
    }

    vsf::UpdateHooks hooks;
    hooks.update = update;
    vsf::application::run(hooks);

    vsf::application::shutdown();

    return 0;
}
