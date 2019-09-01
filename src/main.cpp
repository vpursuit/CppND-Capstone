#include <iostream>
#include "controller.h"
#include "simulation.h"
#include "renderer.h"
#include "configuration.h"

std::string Configuration::DEFAULT_CONFIGFILE = "simulation_config.txt";

int main() {

    // Read the configuration file
    Configuration config;

    std::size_t kMsPerFrame{1000 / config.getFPS()};
    std::size_t kGridWidth = config.getWindowWidth();
    std::size_t kGridHeight = config.getWindowHeight();

    // Initialize a render object
    Renderer renderer(config);
    Controller controller;
    Simulation game(config);
    game.Run(controller, renderer, kMsPerFrame);

    std::cout << "Simulation has terminated successfully!\n";
    return 0;
}