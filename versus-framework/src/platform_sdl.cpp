#include "platform.h"
#include <SDL.h>

namespace
{
    constexpr Uint32 INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER;
    SDL_Window* window;

    void log_sdl_version()
    {
        SDL_version version;
        SDL_GetVersion(&version);
        SDL_Log("SDL version = %i.%i.%i", version.major, version.minor, version.patch);
    }


    bool initialise_sdl()
    {
        if (SDL_Init(INIT_FLAGS) != 0)
        {
            SDL_Log("Failed to initialise SDL: %s", SDL_GetError());
            return false;
        }

        SDL_Log("Initialised SDL");
        log_sdl_version();

        return true;
    }

    void shutdown_sdl()
    {
        SDL_Quit();
        SDL_Log("Shutdown SDL");
    }

    SDL_Window* create_window(const char* title, int width, int height)
    {
        constexpr auto CENTRE = SDL_WINDOWPOS_CENTERED;
        constexpr Uint32 WINDOW_FLAGS = 0;

        SDL_Window* window = SDL_CreateWindow(title, CENTRE, CENTRE, width, height, WINDOW_FLAGS);
        if (!window)
        {
            SDL_Log("Failed to create window: %s", SDL_GetError());
            return nullptr;
        }

        SDL_Log("Created window %p", window);
        return window;
    }

    void free_window(SDL_Window* window)
    {
        SDL_DestroyWindow(window);
        SDL_Log("Destoryed window %p", window);
    }

    void poll_and_handle_events(bool& is_running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                SDL_Log("SDL quit event received");
                is_running = false;
            }
        }
    }
}

namespace vsf 
{
    bool platform::initialise(const Config& config) 
    {
        if (!initialise_sdl())
        {
            return false;
        }

        window = create_window(config.title.c_str(), config.video.resolution_x, config.video.resolution_y);
        if (!window)
        {
            shutdown_sdl();
            return false;
        }

        return true;
    }

    void platform::run(UpdateHooks hooks) 
    {
        auto is_running = true;
        while (is_running)
        {
            poll_and_handle_events(is_running);
            if (is_running)
            {
                hooks.update();
            }
        }
    }

    void platform::shutdown() 
    {
        free_window(window);
        shutdown_sdl();
    }
}
