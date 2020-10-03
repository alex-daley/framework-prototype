#include "application.h"
#include "platform.h"

namespace vsf 
{
    bool application::initialise(const Config& config) 
    {
        return platform::initialise(config);
    }

    void application::run(UpdateHooks hooks) 
    {
        platform::run(hooks);
    }

    void application::shutdown() 
    {
        platform::shutdown();
    }
}
