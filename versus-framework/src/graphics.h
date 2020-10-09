#pragma once
#include "maths.h"

namespace vsf
{
    struct Resolution
    {
        int width;
        int height;
    };

    class ITexture
    {
    public:
        virtual ~ITexture() {}
        virtual Resolution get_resolution() const = 0;
    };

    struct Sprite
    {
        Sprite(ITexture& texture, const RectInt& texture_rect, Vector2 position);
        Sprite(ITexture& texture, const RectInt& texture_rect);
        Sprite(ITexture& texture, const Vector2 position);
        Sprite(ITexture& texture);

        ITexture& texture;
        RectInt texture_rect;
        Vector2 position;
    };

    class ISpriteBatch
    {
    public:
        virtual ~ISpriteBatch() {}
        virtual void draw(const Sprite& sprite) = 0;
    };
}
