#pragma once
#include "graphics.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace vsf
{
    class ITexture;
    typedef std::unordered_map<
        std::string, 
        std::unique_ptr<ITexture>> 
        TextureMap;

    // Loads assets and manages their lifetimes (via RAII).
    class AssetManager
    {
    public:
        // Lazy loads a texture by name.
        ITexture& get_texture(const std::string& name);

    private:
        TextureMap textures;
    };
}
