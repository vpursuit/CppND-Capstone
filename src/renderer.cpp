#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width,
                   const std::size_t grid_height,
                   const std::size_t particle_limit)
        : screen_width(screen_width),
          screen_height(screen_height),
          grid_width(grid_width),
          grid_height(grid_height),
          particle_render_limit(particle_limit) {


    int numdrivers = SDL_GetNumRenderDrivers();
    std::cout << "Render driver count: " << numdrivers << std::endl;
    for (int i = 0; i < numdrivers; i++) {
        SDL_RendererInfo drinfo;
        SDL_GetRenderDriverInfo(0, &drinfo);
        std::cout << "Driver name (" << i << "): " << drinfo.name << std::endl;
        if (drinfo.flags & SDL_RENDERER_SOFTWARE) std::cout << " the renderer is a software fallback" << std::endl;
        if (drinfo.flags & SDL_RENDERER_ACCELERATED)
            std::cout << " the renderer uses hardware acceleration" << std::endl;
        if (drinfo.flags & SDL_RENDERER_PRESENTVSYNC)
            std::cout << " present is synchronized with the refresh rate " << std::endl;
        if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE)
            std::cout << " the renderer supports rendering to texture" << std::endl;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create Window
    sdl_window = SDL_CreateWindow("Particle Simulation",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  screen_width,
                                  screen_height,
                                  SDL_WINDOW_SHOWN);

    if (nullptr == sdl_window) {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create renderer
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == sdl_renderer) {
        std::cerr << "Accelerated renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        std::cerr << "Trying now to create software renderer.\n";
        sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_SOFTWARE);
        if (nullptr == sdl_renderer) {
            std::cerr << "Software renderer could not be created.\n";
            std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        }
    }
}

Renderer::~Renderer() {
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void Renderer::render(SimulationObjects &particles) {

    // Clear screen
    SDL_SetRenderDrawColor(sdl_renderer, 50, 204, 255, 0xFF);
    SDL_RenderClear(sdl_renderer);

    std:size_t fraction = particle_render_limit;

    SDL_Renderer *renderer = sdl_renderer;
    particles.map([renderer, fraction](std::shared_ptr<SimulationObject> &obj, size_t i) -> bool {
        Particle &part = obj->getParticle();
        SDL_Rect block;
        block.w = obj->getSize();
        block.h = obj->getSize();
        ++i;
        if (i % fraction == 0) {
            Vector3 position = part.getPosition();
            block.x = static_cast<int>(position.x);
            block.y = static_cast<int>(position.y);
            RGBA color = obj->getColor();
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            //SDL_SetRenderDrawColor(renderer, 0x00, 0x7A, 0xCC, 0xFF);
            SDL_RenderFillRect(renderer, &block);
        }
        return false;
    });

    // integrate Screen
    SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int particleCount, int fps) {
    std::string title{"Molecules: " + std::to_string(particleCount) + " FPS: " + std::to_string(fps)};
    SDL_SetWindowTitle(sdl_window, title.c_str());
}
