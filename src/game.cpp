#include <iostream>
#include <future>
#include <thread>
#include "game.h"
#include "SDL.h"
#include "particle.h"
#include "simulationObject.h"
#include "particlePhysics2D.h"

using namespace sim;

Game::Game(std::size_t grid_width, std::size_t grid_height, int particleCount, double velocityRange)
        : grid_width(grid_width), grid_height(grid_height),
          particleCount(particleCount), velocityRange(velocityRange),
          engine(dev()),
          random_w(0, static_cast<int>(grid_width)),
          random_h(0, static_cast<int>(grid_height)),
          random_v(-velocityRange, velocityRange),
          _particles(SimulationObjects()),
          physics2D(PatrticlePhysics2D(grid_width, grid_height, _particles)){

}

Game::~Game() {
    // Wait for all threads to terminate
    for (auto &ftr : futures)
        ftr.wait();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration, std::size_t target_physics_interval) {

    bool running = true;

    // Create the items
    PlaceParticles(particleCount);

    // Start a physics thread

    std::promise<void> exitSignal;

    //Fetch std::future object associated with promise
    std::future<void> futureObj = exitSignal.get_future();

    // Starting Thread & move the future object in lambda function by reference
    std::thread th(&PatrticlePhysics2D::run, physics2D, target_physics_interval, std::move(futureObj));


    //futures.emplace_back(std::async(std::launch::async, &PatrticlePhysics2D::run, physics2D, target_physics_interval));

    // TODO Start an AI thread

    int frame_count = 0;

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

            // Input, _update, render - the main game loop.
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

    //Set the value in promise
    exitSignal.set_value();
    th.join();
    /**
    physics2D.stop();
    for (auto &ftr : futures)
        ftr.wait();
        **/
}

void Game::PlaceParticles(int const count) {

    int i = 0;

    while (i++ < count) {

        int x, y, vX, vY;
        while (true) {
            x = random_w(engine);
            y = random_h(engine);

            // TODO: initial collisiondtection
            if (x >= 0 && x <= grid_width && y >= 0 && y <= grid_height) {
                std::unique_ptr<SimulationObject> m(new N2());
                Particle &part = m->getParticle();
                part.setPosition(x, y, 0.0);
                part.setVelocity(random_v(engine), random_v(engine), 0.0);
                part.setDamping(0.95);
                part.setMass(1.0);
                _particles.pushBack(std::move(m));
                break;
            }
        }

        while (true) {
            x = random_w(engine);
            y = random_h(engine);

            // TODO: initial collisiondtection
            if (x >= 0 && x <= grid_width && y >= 0 && y <= grid_height) {
                std::unique_ptr<SimulationObject> m(new O2());
                Particle &part = m->getParticle();
                part.setPosition(x, y, 0.0);
                part.setVelocity(random_v(engine), random_v(engine), 0.0);
                part.setDamping(0.95);
                part.setMass(1.0);
                _particles.pushBack(std::move(m));
                break;
            }
        }

        while (true) {
            x = random_w(engine);
            y = random_h(engine);

            // TODO: initial collisiondtection
            if (x >= 0 && x <= grid_width && y >= 0 && y <= grid_height) {
                std::unique_ptr<SimulationObject> m(new CO2());
                Particle &part = m->getParticle();
                part.setPosition(x, y, 0.0);
                part.setVelocity(random_v(engine), random_v(engine), 0.0);
                part.setDamping(0.95);
                part.setMass(1.0);
                _particles.pushBack(std::move(m));
                break;
            }
        }
    }

}


int Game::GetSize() const { return particleCount; }