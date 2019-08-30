#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "particle.h"
#include "simulationObject.h"
#include "configuration.h"

class Renderer {
public:

    Renderer(Configuration configuration);

    ~Renderer();

    void render(SimulationObjects &particles);

    void UpdateWindowTitle(std::size_t score, std::size_t fps, std::size_t collPerSecond);

private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    Configuration config;
};

#endif