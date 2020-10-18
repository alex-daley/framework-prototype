#pragma once
#include "maths.h"
#include <string>
#include <cstdint>

namespace vsf
{
    struct Resolution
    {
        int width;
        int height;
    };

    struct Colour
    {
        Colour(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
        Colour();

        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;
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

    class IGuiBatch
    {
    public:
        virtual ~IGuiBatch() {}
        virtual void label(const std::string& text) = 0;
    };
}
