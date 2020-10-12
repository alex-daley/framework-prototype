#include <vsf.h>
#include <memory>

namespace
{
    std::unique_ptr<vsf::Config> load_config()
    {
        auto config = std::make_unique<vsf::Config>();
        config->title = "Versus Editor";
        config->video.resolution_x = 1280;
        config->video.resolution_y = 720;
        config->video.use_vsync = true;
        return config;
    }

    void update(const vsf::UpdateTime& time)
    {
    }

    void draw(vsf::ISpriteBatch& batch)
    {
    }
}

int main() 
{
    auto config = load_config();

    if (!vsf::application::initialise(*config))
    {
        return -1;
    }

    vsf::UpdateHooks hooks;
    hooks.update = update;
    hooks.draw = draw;

    vsf::application::run(hooks);

    return 0;
}
