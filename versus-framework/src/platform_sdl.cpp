#include "platform.h"
#include <SDL.h>

namespace
{
    constexpr Uint32 INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER;
    SDL_Window* window;
    SDL_Renderer* renderer;

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


    SDL_Renderer* create_renderer(SDL_Window* window, bool use_vsync)
    {
        constexpr int FIRST_VIABLE_DRIVER = -1;
        
        Uint32 flags = SDL_RENDERER_ACCELERATED;
        if (use_vsync)
        {
            flags |= SDL_RENDERER_PRESENTVSYNC;
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, FIRST_VIABLE_DRIVER, flags);
        if (!renderer)
        {
            SDL_Log("Failed to create renderer: %s", SDL_GetError());
            return nullptr;
        }

        SDL_Log("Created renderer %p with window %p", renderer, window);
        return renderer;
    }

    void free_renderer(SDL_Renderer* renderer)
    {
        SDL_DestroyRenderer(renderer);
        SDL_Log("Destroyed renderer %p and associated textures", renderer);
    }

    void render_begin(SDL_Renderer* renderer)
    {
        // Cornflower blue.
        SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);
        SDL_RenderClear(renderer);
    }

    void render_present(SDL_Renderer* renderer)
    {
        SDL_RenderPresent(renderer);
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

        renderer = create_renderer(window, config.video.use_vsync);
        if (!renderer)
        {
            free_window(window);
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
                render_begin(renderer);
                hooks.update();
                render_present(renderer);
            }
        }
    }

    void platform::shutdown() 
    {
        free_renderer(renderer);
        free_window(window);
        shutdown_sdl();
    }
}
