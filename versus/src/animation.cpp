#include "animation.h"

namespace versus
{
    Animator::Animator(const AnimationMap& animations) : animations(animations)
    {
        play(animations.begin()->first, {});
    }

    void Animator::update(const vsf::UpdateTime& time)
    {
        if (should_draw_next_frame(time.time))
        {
            static auto t = time.time;
            step_frame();
            next_frame_time = time.time + current_animation->seconds_per_frame;
        }
    }

    void Animator::draw(vsf::ISpriteBatch& batch)
    {
        vsf::RectInt frame = current_animation->frames[frame_index];
        vsf::Sprite sprite(*current_animation->texture, frame, vsf::Vector2(64, 64));
        batch.draw(sprite);
    }

    void Animator::play(const std::string& animation_name, const vsf::UpdateTime& time)
    {
        current_animation = &animations.at(animation_name);
        next_frame_time += time.time + current_animation->seconds_per_frame;
    }

    bool Animator::should_draw_next_frame(float time) const
    {
        return time >= next_frame_time;
    }

    void Animator::step_frame()
    {
        frame_index++;

        if (frame_index == current_animation->frames.size())
        {
            frame_index = 0;
        }
    }
}
