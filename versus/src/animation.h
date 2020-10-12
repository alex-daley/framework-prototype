#pragma once
#include <vsf.h>
#include <string>
#include <vector>
#include <unordered_map>

namespace versus
{
    struct Animation
    {
        vsf::ITexture* texture;
        std::vector<vsf::RectInt> frames;
        float seconds_per_frame;
    };

    typedef std::unordered_map<std::string, Animation> AnimationMap;

    class Animator final
    {
    public:
        Animator(const AnimationMap& animations);

        void update(const vsf::UpdateTime& time);
        void draw(vsf::ISpriteBatch& batch);
        void play(const std::string& animation_name, const vsf::UpdateTime& time);

    private:
        AnimationMap animations;
        Animation* current_animation = nullptr;
        float next_frame_time = 0.0f;
        int frame_index = 0;

        bool should_draw_next_frame(float time) const;
        void step_frame();
    };
}
