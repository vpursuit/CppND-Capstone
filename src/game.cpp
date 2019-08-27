#include "game.h"
#include <iostream>
#include "SDL.h"

#include "particle.h"
#import "simulationObject.h"
#include <future>
#include <thread>

using namespace sim;

Game::Game(std::size_t grid_width, std::size_t grid_height, int particleCount, double velocityRange)
        : grid_width(grid_width), grid_height(grid_height),
          particleCount(particleCount), velocityRange(velocityRange),
          engine(dev()),
          random_w(0, static_cast<int>(grid_width)),
          random_h(0, static_cast<int>(grid_height)),
          random_v(-velocityRange, velocityRange) {

}

Game::~Game() {

    // Wait for all threads to terminate
    for (auto &ftr : futures)
        ftr.wait();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration, std::size_t target_physics_interval) {

    // Create the _simobjects
    PlaceParticles(particleCount);

    // Start a physics thread
    futures.emplace_back(std::async(std::launch::async, &Game::doPhysics, this, target_physics_interval));

    // TODO Start a AI thread

    int frame_count = 0;
    bool running = true;

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

            // _update(static_cast<double>(timeSinceLastUpdate) / 1000.0f);

            // Compute physics in extra thread and  only render here
            particleMutex.lock();
            renderer.render(_particles);
            particleMutex.unlock();

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

    particleMutex.lock();
    int i = 0;

    while (i++ < count) {

        int x, y, vX, vY;
        while (true) {
            x = random_w(engine);
            y = random_h(engine);

            // TODO: initial collisiondtection
            if (x >= 0 && x <= grid_width && y >= 0 && y <= grid_height) {
                std::shared_ptr<SimulationObject> m( new N2() );
                Particle & part = m->getParticle();
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
                std::shared_ptr<SimulationObject> m( new O2() );
                Particle & part = m->getParticle();
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
                std::shared_ptr<SimulationObject> m( new CO2() );
                Particle & part = m->getParticle();
                part.setPosition(x, y, 0.0);
                part.setVelocity(random_v(engine), random_v(engine), 0.0);
                part.setDamping(0.95);
                part.setMass(1.0);
                _particles.pushBack(std::move(m));
                break;
            }
        }
    }
    particleMutex.unlock();

}

void Game::_update(double duration) {

    std::size_t width = grid_width;
    std::size_t height = grid_height;

    _particles.map([duration, width, height](std::shared_ptr<SimulationObject> obj, size_t i) mutable {

        Particle &part = obj->getParticle();

        part.integrate(duration);

        Vector3 position = part.getPosition();
        Vector3 velocity = part.getVelocity();
        bool boxCollision = false;
        if (position.x <= 0.0) {
            velocity.x *= -1.0;
            // position.x = grid_width;
            boxCollision = true;
        } else if (position.x > width) {
            velocity.x *= -1.0;
            //position.x = 0.0;
            boxCollision = true;
        }
        if (position.y <= 0.0) {
            velocity.y *= -1.0;
            //position.y = grid_height;
            boxCollision = true;
        } else if (position.y > height) {
            velocity.y *= -1.0;
            //position.y = 0.0;
            boxCollision = true;
        }
        if (boxCollision) {
            part.setVelocity(velocity);
        }

    });

}

void Game::doPhysics(std::size_t target_physics_interval) {

    bool running = true;

    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();

    while (running) {

        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now() - lastUpdate).count();

        // Its time to render a frame to met the fps spec.
        if (timeSinceLastUpdate >= target_physics_interval) {
            _update(static_cast<double>(timeSinceLastUpdate) / 1000.0f);
            lastUpdate = std::chrono::system_clock::now();
        }

    }

}

int Game::GetSize() const { return particleCount; }