#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "configuration.h"

int main() {

    // Read the configuration file
    Configuration config;

    std::size_t kFramesPerSecond = config.getIntParameter("fps");
    std::size_t kScreenWidth = config.getIntParameter("window_width");
    std::size_t kScreenHeight = config.getIntParameter("window_height");
    std::size_t kMsPerPhysicsSimulation = config.getIntParameter("physic_interval_ms");
    std::size_t particle_count = config.getIntParameter("particle_count");
    std::size_t particle_render_limit = config.getIntParameter("particle_render_limit");
    std::size_t particle_velocity_range = config.getIntParameter("particle_velocity_range");
    std::size_t collision_limit = config.getIntParameter("collision_limit");

    std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    std::size_t kGridWidth = kScreenWidth;
    std::size_t kGridHeight = kScreenHeight;

    // Initialize a render object
    Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight, particle_render_limit);
    Controller controller;
    Game game(kGridWidth, kGridHeight, kMsPerPhysicsSimulation, particle_count, collision_limit,
              particle_velocity_range);
    game.Run(controller, renderer, kMsPerFrame);

    std::cout << "Game has terminated successfully!\n";
    std::cout << "Size: " << game.GetSize() << "\n";
    return 0;
}