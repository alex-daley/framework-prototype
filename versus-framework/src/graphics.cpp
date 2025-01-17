#include "graphics.h"

namespace
{
    vsf::RectInt full_rect(const vsf::ITexture& texture)
    {
        vsf::Resolution resolution = texture.get_resolution();
        return vsf::RectInt(0, 0, resolution.width, resolution.height);
    }
}

namespace vsf
{
    Colour::Colour(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) 
        : r(r), g(g), b(b), a(a)
    {
    }
    Colour::Colour() : r(0), g(0), b(0), a(0)
    {
    }

    Sprite::Sprite(ITexture& texture, const RectInt& texture_rect, Vector2 position) :
        texture(texture),
        texture_rect(texture_rect),
        position(position)
    {
    }

    Sprite::Sprite(ITexture& texture, const RectInt& texture_rect) : 
        Sprite(texture, texture_rect, Vector2())
    {
    }

    Sprite::Sprite(ITexture& texture, const Vector2 position) : 
        Sprite(texture, full_rect(texture), position)
    {
    }

    Sprite::Sprite(ITexture& texture) :
        Sprite(texture, full_rect(texture), Vector2())
    {
    }
}
