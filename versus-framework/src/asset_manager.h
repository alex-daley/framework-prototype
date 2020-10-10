#pragma once
#include "graphics.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace vsf
{
    typedef std::unordered_map<
        std::string, 
        std::unique_ptr<ITexture>> 
        TextureMap;

    // Loads assets and manages their lifetimes (via RAII).
    class AssetManager
    {
    public:
        AssetManager(const std::string& root_path);
        AssetManager();

        // Lazy loads a texture by name.
        ITexture& load_texture(const std::string& name);

        // Preload assets.
        void preload();


    private:
        std::string make_asset_key(const std::string& path) const;

        std::string root_path;
        TextureMap textures;
    };
}
