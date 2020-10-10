#include "asset_manager.h"
#include "platform.h"
#include "debug.h"

#include <algorithm>
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
    AssetManager::AssetManager(const std::string& root_path) : root_path(root_path)
    {
    }
    
    AssetManager::AssetManager() : AssetManager(".")
    {
    }

    ITexture& AssetManager::load_texture(const std::string& name)
    {
        std::string key = make_asset_key(name);

        if (!contains(textures, key))
        {
            textures[key] = platform::load_texture(name);
        }

        return *textures[name];
    }

    void AssetManager::preload()
    {
        for (auto& entry : std::filesystem::recursive_directory_iterator(root_path))
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

    std::string AssetManager::make_asset_key(const std::string& path) const
    {
        // Force Unix style path separators.
        std::string key = path;
        std::replace(key.begin(), key.end(), '\\', '/');


        // Remove the root_path and its following separator.
        size_t pos = path.find(root_path);
        if (pos == 0)
        {
            key.erase(pos, root_path.length() + 1);
        }

        return key;
    }
}
