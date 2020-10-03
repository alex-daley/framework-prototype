#include "asset_manager.h"
#include "platform.h"

namespace
{
    template <typename TKey, typename TValue>
    bool contains(const std::unordered_map<TKey, TValue>& map, const TKey& key)
    {
        return map.find(key) != map.end();
    }
}

namespace vsf
{
    ITexture& AssetManager::get_texture(const std::string& name)
    {
        if (!contains(textures, name))
        {
            textures[name] = platform::load_texture(name);
        }

        return *textures[name];
    }
}
