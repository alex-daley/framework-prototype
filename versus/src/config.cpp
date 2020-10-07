#include "config.h"
#include <yaml-cpp/yaml.h>
#include <fstream>

namespace
{
    constexpr const char* FILENAME = "config.yml";

    vsf::Config make_default_config()
    {
        vsf::Config config;
        config.title = "Versus";
        config.video.resolution_x = 1280;
        config.video.resolution_y = 720;
        config.video.use_vsync = true;
        return config;
    }

    vsf::Config deserialise(const YAML::Node& node)
    {
        vsf::Config config;
        config.title = node["title"].as<std::string>();
        
        YAML::Node video = node["video"];
        config.video.resolution_x = video["resolution_x"].as<std::uint32_t>();
        config.video.resolution_y = video["resolution_y"].as<std::uint32_t>();
        config.video.use_vsync = video["use_vsync"].as<bool>();
        return config;
    }

    YAML::Node serialise(const vsf::Config& config)
    {
        YAML::Node root;
        root["title"] = config.title;
        
        YAML::Node video = root["video"];
        video["resolution_x"] = config.video.resolution_x;
        video["resolution_y"] = config.video.resolution_y;
        video["use_vsync"] = config.video.use_vsync;
        return root;
    }

    void log_config(const vsf::Config& config)
    {
        LOG_INFO("Config: { title: %s, video: { resolution_x: %i, resolution_y: %i, use_vsync: %i } }",
            config.title.c_str(),
            config.video.resolution_x,
            config.video.resolution_y,
            config.video.use_vsync);
    }
}

namespace versus
{
    vsf::Config config::load()
    {
        try
        {
            YAML::Node file = YAML::LoadFile(FILENAME);
            vsf::Config config = deserialise(file);
            log_config(config);
            return config;
        }
        catch (const YAML::Exception& exception)
        {
            LOG_INFO(exception.what());
            LOG_INFO("Falling back to default config");
            
            vsf::Config config = make_default_config();
            save(config);
            return config;
        }
    }

    void config::save(const vsf::Config& config)
    {
        std::ofstream stream(FILENAME);
        stream << serialise(config);

        LOG_INFO("Saved config");
        log_config(config);
    }
}
