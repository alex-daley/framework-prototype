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
        config->typography.font_path = "C:/Windows/Fonts/Arial.ttf";
        config->typography.font_size = 18;
        return config;
    }

    void update(const vsf::UpdateTime& time)
    {
    }

    void draw(vsf::ISpriteBatch& batch)
    {
    }

    void draw_gui(vsf::IGuiBatch& batch)
    {
        batch.panel(vsf::Vector2(32, 32));
        batch.label("A quick brown fox jumps over the lazy dog.");
        batch.label("A quick brown fox jumps over the lazy dog again.");
        
        if (batch.button("Click me!"))
        {
        }

        batch.panel_commit();
    }
}

int main() 
{
    const auto config = load_config();
    if (!vsf::application::initialise(*config))
    {
        return -1;
    }

    vsf::UpdateHooks hooks;
    hooks.update = update;
    hooks.draw = draw;
    hooks.draw_gui = draw_gui;
    vsf::application::run(hooks);

    vsf::application::shutdown();

    return 0;
}
