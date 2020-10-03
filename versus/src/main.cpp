#include "vsf.h"
#include <memory>

namespace 
{
    void update(const vsf::UpdateTime& time)
    {
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

    auto asset_manager = std::make_unique<vsf::AssetManager>();
    asset_manager->preload();
    
    vsf::UpdateHooks hooks;
    hooks.update = update;
    vsf::application::run(hooks);

    asset_manager.reset();
    vsf::application::shutdown();

    return 0;
}
