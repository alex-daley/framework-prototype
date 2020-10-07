#pragma once

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
}
