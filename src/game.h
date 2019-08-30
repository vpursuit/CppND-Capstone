#ifndef GAME_H
#define GAME_H

#include <random>
#include <future>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "particle.h"
#include "particlePhysics2D.h"
#include "configuration.h"

class Game {

public:
    Game(Configuration configuration);

    ~Game();

    void Run(Controller const &controller,
             Renderer &renderer,
             std::size_t target_frame_duration);

private:

    Configuration config;

    PatrticlePhysics2D physics2D;
    SimulationObjects _particles;

    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
    std::uniform_real_distribution<double> random_v;

    std::vector<std::unique_ptr<std::thread>> _threads;

    void PlaceParticles(int const count);

    void placeMolecule(Molecule *molecule);
};

#endif