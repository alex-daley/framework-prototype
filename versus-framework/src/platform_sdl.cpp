#include "platform.h"
#include "debug.h"
#include "graphics.h"

#include "stb/stb_image.h"
#include "stb/stb_truetype.h"

#include <SDL.h>

#include <unordered_map>
#include <vector>

namespace
{
    constexpr Uint32 INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER;
    SDL_Window* window;
    SDL_Renderer* renderer;

    typedef std::unordered_map<SDL_JoystickID, SDL_GameController*> ControllerMap;
    constexpr int MAX_CONTROLERS = 8;
    ControllerMap controllers(MAX_CONTROLERS);
    
    struct Font;
    // ASCII has 127 characters but we don't care about the first 32.
    constexpr int FIRST_FONT_CHAR = 32; 
    constexpr int MAX_FONT_CHARS = 127 - FIRST_FONT_CHAR;
    Font* standard_font;

    struct Font
    {
        stbtt_fontinfo info;
        stbtt_packedchar chars[MAX_FONT_CHARS];
        SDL_Texture* atlas;
    };


    class TextureImplSDL final : public vsf::ITexture
    {
    public:
        TextureImplSDL(SDL_Texture* sdl_texture, int width, int height);
        ~TextureImplSDL();

        vsf::Resolution get_resolution() const override;

        SDL_Texture* sdl_texture;

    private:
        vsf::Resolution resolution;
    };

    class SpriteBatchImplSDL final : public vsf::ISpriteBatch
    {
    public:
        void draw(const vsf::Sprite& sprite) override;
    };

    void log_sdl_version()
    {
        SDL_version version;
        SDL_GetVersion(&version);
        LOG_INFO("SDL version = %i.%i.%i", version.major, version.minor, version.patch);
    }

    bool initialise_sdl()
    {
        if (SDL_Init(INIT_FLAGS) != 0)
        {
            LOG_ERROR("Failed to initialise SDL: %s", SDL_GetError());
            return false;
        }

        LOG_INFO("Initialised SDL");
        log_sdl_version();

        return true;
    }

    void shutdown_sdl()
    {
        SDL_Quit();
        LOG_INFO("Shutdown SDL");
    }

    SDL_Window* create_window(const char* title, int width, int height)
    {
        constexpr auto CENTRE = SDL_WINDOWPOS_CENTERED;
        constexpr Uint32 WINDOW_FLAGS = 0;

        SDL_Window* window = SDL_CreateWindow(title, CENTRE, CENTRE, width, height, WINDOW_FLAGS);
        if (!window)
        {
            LOG_ERROR("Failed to create window: %s", SDL_GetError());
            return nullptr;
        }

        LOG_INFO("Created window %p", window);
        return window;
    }

    void free_window(SDL_Window* window)
    {
        SDL_DestroyWindow(window);
        LOG_INFO("Destoryed window %p", window);
    }

    SDL_JoystickID get_controller_id(SDL_GameController* sdl_controller)
    {
        SDL_Joystick* joystick = SDL_GameControllerGetJoystick(sdl_controller);
        return SDL_JoystickInstanceID(joystick);
    }

    void add_controller(ControllerMap& controllers, int index)
    {
        SDL_GameController* controller = SDL_GameControllerOpen(index);
        if (!controller)
        {
            LOG_ERROR("Failed to open controller at index %i: %s", index, SDL_GetError());
            return;
        }

        SDL_JoystickID id = get_controller_id(controller);
        const char* name = SDL_GameControllerName(controller);
        controllers[id] = controller;
        LOG_INFO("Opened %s %p (id: %i)", name, controller, id);
    }

    void close_controller(SDL_JoystickID id)
    {
        SDL_GameController* controller = controllers[id];
        const char* name = SDL_GameControllerName(controller);
        SDL_GameControllerClose(controller);
        LOG_INFO("Closed %s %p (id: %i)", name, controller, id);
    }

    void remove_controller(ControllerMap& controllers, SDL_JoystickID id)
    {
        close_controller(id);
        controllers.erase(id);
    }

    void free_controllers(ControllerMap& controllers)
    {
        for (auto& entry : controllers)
        {
            SDL_JoystickID id = entry.first;
            close_controller(id);
        }

        controllers.clear();
    }

    void handle_event(bool& is_running, const SDL_Event& event)
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                LOG_INFO("SDL quit event received");
                is_running = false;
                break;
            }
            case SDL_CONTROLLERDEVICEADDED:
            {
                auto index = static_cast<int>(event.cdevice.which);
                add_controller(controllers, index);
                break;
            }
            case SDL_CONTROLLERDEVICEREMOVED:
            {
                // `event.cdevice.which` refers to a device index on add events,
                // but a device ID on all subsequent controller events.
                auto id = static_cast<SDL_JoystickID>(event.cdevice.which);
                remove_controller(controllers, id);
                break;
            }
        }
    }

    void poll_and_handle_events(bool& is_running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            handle_event(is_running, event);
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
            LOG_ERROR("Failed to create renderer: %s", SDL_GetError());
            return nullptr;
        }

        SDL_RenderSetLogicalSize(renderer, 320, 180);

        LOG_INFO("Created renderer %p with window %p", renderer, window);
        return renderer;
    }

    void free_renderer(SDL_Renderer* renderer)
    {
        SDL_DestroyRenderer(renderer);
        LOG_INFO("Destroyed renderer %p and associated textures", renderer);
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

    SDL_Texture* load_texture(const char* path, SDL_Renderer* renderer)
    {
        int width, height, channels;
        unsigned char* data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
        if (!data)
        {
            LOG_ERROR("Failed to load image data: %s", stbi_failure_reason());
            return nullptr;
        }

        constexpr int DEPTH = 32;
        int pitch = width * 4;
        SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom((void*)data, width, height, DEPTH, pitch, SDL_PIXELFORMAT_RGBA32);
        if (!surface)
        {
            LOG_ERROR("Failed to create surface: %s", SDL_GetError());
            stbi_image_free(data);
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture)
        {
            LOG_ERROR("Failed to create texture: %s", SDL_GetError());
            SDL_FreeSurface(surface);
            stbi_image_free(data);
            return nullptr;
        }

        SDL_FreeSurface(surface);
        stbi_image_free(data);
        LOG_INFO("Loaded texture %p from %s", texture, path);

        return texture;
    }

    void free_sdl_texture(SDL_Texture* texture)
    {
        SDL_DestroyTexture(texture);
        LOG_INFO("Destroyed texture %p", texture);
    }

    SDL_Texture* make_font_atlas(unsigned char* bitmap, size_t texture_width, size_t texture_height)
    {
        const size_t texture_size = texture_width * texture_height;
        SDL_Texture* atlas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, texture_width, texture_height);
        if (!atlas)
        {
            LOG_ERROR("Failed to create font altas: %s", SDL_GetError());
            return nullptr;
        }

        auto pixels = new Uint32[texture_size];
        static SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
        for (size_t i = 0; i < texture_size; i++)
        {
            pixels[i] = SDL_MapRGBA(format, 255, 255, 255, bitmap[i]);
        }

        SDL_UpdateTexture(atlas, nullptr, pixels, texture_width * sizeof(Uint32));
        SDL_SetTextureBlendMode(atlas, SDL_BLENDMODE_BLEND);
        delete[] pixels;

        LOG_INFO("Created font atlas %p (%ix%i)", atlas, texture_width, texture_height);
        return atlas;
    }

    unsigned char* pack_chars_and_make_bitmap(stbtt_packedchar* chars, unsigned char* file_data, size_t width, size_t height, float font_size)
    {
        const size_t size = width * height;
        auto bitmap = new unsigned char[size];
        stbtt_pack_context pack_context;
        stbtt_PackBegin(&pack_context, bitmap, width, height, 0, 1, 0);

        if (!stbtt_PackFontRange(&pack_context, file_data, 0, font_size, FIRST_FONT_CHAR, MAX_FONT_CHARS, chars))
        {
            delete[] bitmap;
            stbtt_PackEnd(&pack_context);
            return nullptr;
        }

        stbtt_PackEnd(&pack_context);
        return bitmap;
    }

    unsigned char* read_binary_file(const char* path)
    {
        constexpr auto READ_BINARY = "rb";
        SDL_RWops* stream = SDL_RWFromFile(path, READ_BINARY);
        if (!stream)
        {
            LOG_ERROR("Failed to open binary file: %s", SDL_GetError());
            return nullptr;
        }

        size_t file_size = static_cast<size_t>(SDL_RWsize(stream));
        unsigned char* file_data = new unsigned char[file_size];
        if (!SDL_RWread(stream, file_data, file_size, 1))
        {
            LOG_ERROR("Failed to read binary file: %s", SDL_GetError());
            delete[] file_data;
            SDL_RWclose(stream);
            return nullptr;
        }

        SDL_RWclose(stream);
        
        return file_data;
    }

    bool read_font_info(unsigned char* ttf_data, stbtt_fontinfo* info)
    {
        if (!stbtt_InitFont(info, ttf_data, 0))
        {
            LOG_ERROR("Failed to read font data %s");
            return false;
        }

        return true;
    }

    Font* load_font(SDL_Renderer* renderer, const char* path, int font_size)
    {
        unsigned char* ttf_data = read_binary_file(path);
        if (!ttf_data)
        {
            return nullptr;
        }

        Font* font = new Font();
        if (!read_font_info(ttf_data, &font->info))
        {
            delete font;
            return nullptr;
        }

        size_t bitmap_w = 32;
        size_t bitmap_h = 32;
        unsigned char* bitmap = nullptr;        

        while (!bitmap)
        {
            bitmap = pack_chars_and_make_bitmap(font->chars, ttf_data, bitmap_w, bitmap_h, (float)font_size);
            if (!bitmap)
            {
                bitmap_w += 32;
                bitmap_h += 32;
            }
        }

        delete[] ttf_data;

        font->atlas = make_font_atlas(bitmap, bitmap_w, bitmap_h);
        delete[] bitmap;
        
        LOG_INFO("Created font %p from %s", font, path);
        
        return font;
    }

    void free_font(Font* font)
    {
        SDL_DestroyTexture(font->atlas);
        LOG_INFO("Destroyed font atlas %p", font->atlas);

        Font* to_print = font;
        delete font;
        LOG_INFO("Destroyed font %p", to_print);
        font = nullptr;
    }

    SDL_Rect get_font_source_rect(const stbtt_packedchar& info)
    {
        SDL_Rect rect;
        rect.x = info.x0;
        rect.y = info.y0;
        rect.w = info.x1 - rect.x;
        rect.h = info.y1 - rect.y;
        return rect;
    }

    SDL_Rect get_font_destination_rect(const stbtt_packedchar& info, SDL_Point position)
    {
        SDL_Rect rect;
        rect.x = vsf::maths::round_to_int(position.x + info.xoff);
        rect.y = vsf::maths::round_to_int(position.y + info.yoff);
        rect.w = info.x1 - info.x0;
        rect.h = info.y1 - info.y0;
        return rect;
    }

    void draw_text(SDL_Renderer* renderer, Font* font, float x, float y, const char* text)
    {
        for (int i = 0; text[i]; i++)
        {
            stbtt_packedchar* info = &font->chars[text[i] - FIRST_FONT_CHAR];
            SDL_Rect source = get_font_source_rect(*info);
            SDL_Rect destination = get_font_destination_rect(*info, { (int)x, (int)y });
            SDL_RenderCopy(renderer, font->atlas, &source, &destination);
            x += info->xadvance;
        }
    }

    SDL_Point get_texture_size(SDL_Texture* texture)
    {
        int width = 0;
        int height = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        return { width, height };
    }

    SDL_Texture* get_sdl_texture(const vsf::ITexture& texture)
    {
        auto impl = reinterpret_cast<const TextureImplSDL*>(&texture);
        return impl->sdl_texture;
    }

    SDL_Rect to_sdl_rect(const vsf::RectInt& rect)
    {
        SDL_Rect sdl_rect;
        sdl_rect.x = rect.min_x;
        sdl_rect.y = rect.min_y;
        sdl_rect.w = rect.width();
        sdl_rect.h = rect.height();
        return sdl_rect;
    }

    TextureImplSDL::TextureImplSDL(SDL_Texture* sdl_texture, int width, int height) :
        sdl_texture(sdl_texture),
        resolution({ width, height })
    {
    }

    TextureImplSDL::~TextureImplSDL()
    {
        free_sdl_texture(sdl_texture);
    }

    vsf::Resolution TextureImplSDL::get_resolution() const
    {
        return resolution;
    }

    void SpriteBatchImplSDL::draw(const vsf::Sprite& sprite)
    {
        SDL_Texture* texture = get_sdl_texture(sprite.texture);
        SDL_Rect source = to_sdl_rect(sprite.texture_rect);
        
        SDL_Rect destination;
        destination.x = vsf::maths::round_to_int((sprite.position.x - source.w) / 2.0f);
        destination.y = vsf::maths::round_to_int((sprite.position.y - source.h) / 2.0f);
        destination.w = source.w;
        destination.h = source.h;

        SDL_RenderCopy(renderer, texture, &source, &destination);
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

        standard_font = load_font(renderer, config.typography.font_path.c_str(), config.typography.font_size);
        if (!standard_font)
        {
            free_renderer(renderer);
            free_window(window);
            shutdown_sdl();
            return false;
        }

        return true;
    }

    void platform::run(UpdateHooks hooks) 
    {
        static Uint64 start = SDL_GetPerformanceCounter();
        static Uint64 frequency = SDL_GetPerformanceFrequency();
        static UpdateTime time{ 0 };

        static SpriteBatchImplSDL sprite_batch;

        static bool is_running = true;
        while (is_running)
        {
            static auto frame_start = SDL_GetPerformanceCounter();
            auto now = SDL_GetPerformanceCounter();
            time.update_count++;
            time.time = ((float)(now - start)) / frequency;
            time.delta_time = ((now - frame_start) * 1000.0f) / frequency;
            frame_start = now;

            poll_and_handle_events(is_running);

            if (is_running)
            {
                hooks.update(time);

                render_begin(renderer);
                SDL_RenderSetLogicalSize(renderer, 1280, 720);
                draw_text(renderer, standard_font, 50.0f, 50.0f, "A quick brown fox jumps over the lazy dog.");
                SDL_RenderSetLogicalSize(renderer, 320, 180);
                hooks.draw(sprite_batch);
                render_present(renderer);
            }
        }
    }

    void platform::shutdown() 
    {
        free_font(standard_font);
        free_controllers(controllers);
        free_renderer(renderer);
        free_window(window);
        shutdown_sdl();
    }

    std::unique_ptr<ITexture> platform::load_texture(const std::string& path)
    {
        SDL_Texture* sdl_texture = ::load_texture(path.c_str(), renderer);
        SDL_Point size = get_texture_size(sdl_texture);
        return std::make_unique<TextureImplSDL>(sdl_texture, size.x, size.y);
    }
}
