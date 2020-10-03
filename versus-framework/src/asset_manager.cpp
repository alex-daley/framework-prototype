#include "asset_manager.h"
#include "platform.h"
#include "debug.h"

// Requires C++17
#include <filesystem>

namespace
{
    enum class AssetType
    {
        Unknown,
        Image
    };
    
    AssetType deduce_asset_type(const std::string& extension)
    {
        if (extension == ".png")
        {
            return AssetType::Image;
        }

        return AssetType::Unknown;
    }

    AssetType deduce_asset_type(std::filesystem::path& path)
    {
        std::string extension = path.extension().string();
        return deduce_asset_type(extension);
    }

    template <typename TKey, typename TValue>
    bool contains(const std::unordered_map<TKey, TValue>& map, const TKey& key)
    {
        return map.find(key) != map.end();
    }
}

namespace vsf
{
    ITexture& AssetManager::load_texture(const std::string& name)
    {
        if (!contains(textures, name))
        {
            textures[name] = platform::load_texture(name);
        }

        return *textures[name];
    }

    void AssetManager::preload(const std::string& folder)
    {
        for (auto& entry : std::filesystem::recursive_directory_iterator(folder))
        {
            auto path = entry.path();

            switch (deduce_asset_type(path))
            {
                case AssetType::Image:
                {
                    load_texture(path.string());
                    break;
                }
            }
        }
    }

    void AssetManager::preload()
    {
        preload(".");
    }
}
