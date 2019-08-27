#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "particle.h"
#include "simulationObject.h"

using namespace sim;

class Renderer {
public:
    Renderer(const std::size_t screen_width, const std::size_t screen_height,
             const std::size_t grid_width, const std::size_t grid_height,
             size_t particle_limit);

    ~Renderer();

    void render(SimulationObjects &particles);

    void UpdateWindowTitle(int score, int fps);

private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;

    const std::size_t screen_width;
    const std::size_t screen_height;
    const std::size_t grid_width;
    const std::size_t grid_height;
    const std::size_t particle_limit;
};

#endif