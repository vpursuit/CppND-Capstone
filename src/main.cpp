#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main() {
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kMsPerPhysicsSimulation{0};
    constexpr std::size_t kScreenWidth{512};
    constexpr std::size_t kScreenHeight{512};
    constexpr std::size_t kGridWidth{512};
    constexpr std::size_t kGridHeight{512};

    Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight, 1);
    Controller controller;
    Game game(kGridWidth, kGridHeight, kMsPerPhysicsSimulation, 10, 500);
    game.Run(controller, renderer, kMsPerFrame);
    std::cout << "Game has terminated successfully!\n";
    std::cout << "Size: " << game.GetSize() << "\n";
    return 0;
}