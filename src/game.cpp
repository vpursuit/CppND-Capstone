#include <iostream>
#include <future>
#include <thread>
#include "game.h"
#include "SDL.h"
#include "particle.h"
#include "simulationObject.h"
#include "particlePhysics2D.h"

Game::Game(std::size_t grid_width,
           std::size_t grid_height,
           std::size_t kMsPerPhysicsSimulation,
           std::size_t particleCount,
           std::size_t collision_limit,
           double velocityRange)
        : grid_width(grid_width),
          grid_height(grid_height),
          particleCount(particleCount),
          velocityRange(velocityRange),
          engine(dev()),
          random_w(0, static_cast<int>(grid_width)),
          random_h(0, static_cast<int>(grid_height)),
          random_v(-velocityRange, velocityRange),
          _particles(SimulationObjects()),
          physics2D(PatrticlePhysics2D(grid_width, grid_height, kMsPerPhysicsSimulation, collision_limit, _particles)) {
}

Game::~Game() {
    physics2D.stop();

    // Wait for all threads to terminate
    for (auto &t: _threads) {
        t->join();
    }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {

    bool running = true;

    // Create the items
    PlaceParticles(particleCount);

    // Start a physics thread
    _threads.push_back(std::make_unique<std::thread>(std::thread([&]() { physics2D.run(); })));
    _threads.push_back(std::make_unique<std::thread>(std::thread([&]() { physics2D.collider(); })));

    // TODO Start an AI thread

    std::size_t frame_count = 0;

    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    std::chrono::time_point<std::chrono::system_clock> title_timestamp;
    std::chrono::time_point<std::chrono::system_clock> frame_end;

    // init stop watch
    lastUpdate = title_timestamp = std::chrono::system_clock::now();

    while (running) {

        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now() - lastUpdate).count();

        // Its time to render a frame to met the fps spec.
        if (timeSinceLastUpdate >= target_frame_duration) {

            // Input, integrate, render - the main game loop.
            controller.HandleInput(running);

            // Compute physics in extra thread and  only render here
            renderer.render(_particles);

            frame_count++;

            // After every second, update the window title.
            frame_end = std::chrono::system_clock::now();
            long timeSinceLastWindowsUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
                    frame_end - title_timestamp).count();
            if (timeSinceLastWindowsUpdate >= 1000) {
                renderer.UpdateWindowTitle(particleCount, frame_count);
                frame_count = 0;
                title_timestamp = frame_end;
            }

            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();

        }

    }

}

void Game::PlaceParticles(int const count) {

    int N2Count = (count * 78) / 100;
    int O2Count = (count * 21) / 100;
    int CO2Count = std::max((count / 100) * 1, 1);
    std::cout << "N2Count = " << N2Count << std::endl;
    std::cout << "O2Count = " << O2Count << std::endl;
    std::cout << "CO2Count = " << CO2Count << std::endl;
    int i = 0;
    while (i++ < N2Count) {
        placeMolecule(new N2());
    }
    i = 0;
    while (i++ < O2Count) {
        placeMolecule(new O2());
    }
    i = 0;
    while (i++ < CO2Count) {
        placeMolecule(new CO2());
    }
}

void Game::placeMolecule(Molecule *molecule) {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);
        if (x >= 0 && x <= grid_width && y >= 0 && y <= grid_height) {
            std::unique_ptr<SimulationObject> m(molecule);
            Particle &part = m->getParticle();
            part.setPosition(x, y, 0.0);
            part.setVelocity(random_v(engine), random_v(engine), 0.0);
            part.setDamping(0.99);
            part.setMass(1.0);
            _particles.pushBack(std::move(m));
            break;
        }
    }
}

int Game::GetSize() const { return particleCount; }