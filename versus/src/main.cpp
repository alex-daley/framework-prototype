#include "vsf.h"
#include "config.h"
#include "animation.h"

namespace 
{
    using namespace versus;

    std::unique_ptr<Animator> player;

    void update(const vsf::UpdateTime& time)
    {
        player->update(time);
    }

    void draw(vsf::ISpriteBatch& batch)
    {
        player->draw(batch);
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
    
    Animation idle;
    idle.frames = { vsf::RectInt(0, 0, 16, 16), vsf::RectInt(16, 0, 32, 16), vsf::RectInt(32, 0, 48, 16), vsf::RectInt(48, 0, 64, 16) };
    idle.seconds_per_frame = 0.25f;
    idle.texture = &texture;
    
    AnimationMap map;
    map["Idle"] = idle;
    player = std::make_unique<Animator>(map);

    vsf::UpdateHooks hooks;
    hooks.update = update;
    hooks.draw = draw;
    vsf::application::run(hooks);

    asset_manager.reset();
    vsf::application::shutdown();

    return 0;
}
