#pragma once
#include <vsf.h>

namespace versus
{
    namespace config
    {
        vsf::Config load();
        void save(const vsf::Config& config);
    }
}
