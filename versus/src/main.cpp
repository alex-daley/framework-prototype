#include "vsf.h"
#include "config.h"

namespace 
{
    std::unique_ptr<vsf::Sprite> player;

    void update(const vsf::UpdateTime& time)
    {
    }

    void draw(vsf::ISpriteBatch& batch)
    {
        batch.draw(*player);
    }
}

int main() 
{
    vsf::Config config = versus::config::load();

    if (!vsf::application::initialise(config)) 
    {
        return -1;
    }

    auto asset_manager = std::make_unique<vsf::AssetManager>();
    asset_manager->preload();

    vsf::ITexture& texture = asset_manager->load_texture("images/player-placeholder.png");
    player = std::make_unique<vsf::Sprite>(texture, vsf::RectInt(0, 0, 16, 16), vsf::Vector2(16, 16));

    vsf::UpdateHooks hooks;
    hooks.update = update;
    hooks.draw = draw;
    vsf::application::run(hooks);

    asset_manager.reset();
    vsf::application::shutdown();

    return 0;
}
