//
// Created by Trebing, Peter on 2019-08-27.
//
#include <iostream>
#include <future>
#include <thread>
#include "simulation.h"
#include "SDL.h"
#include "particle.h"
#include "simulationObject.h"
#include "particlePhysics2D.h"
#include "molecules.h"

Simulation::Simulation(Configuration configuration) :
        config(configuration),
        engine(dev()),
        random_w(0, static_cast<int>(configuration.getWindowWidth())),
        random_h(0, static_cast<int>(configuration.getWindowHeight())),
        random_v(-configuration.getParticleVelocityRange(), configuration.getParticleVelocityRange()),
        _particles(SimulationObjects()),
        physics2D(PatrticlePhysics2D(configuration, _particles)) {

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);

}

Simulation::~Simulation() {
    physics2D.stop();

    // Wait for all threads to terminate
    for (auto &t: _threads) {
        t->join();
    }
}

void Simulation::Run(Controller &controller, Renderer &renderer,
                     std::size_t target_frame_duration) {

    bool running = true;
    KeyState keys = KeyState{false, false, false};

    // Create the items
    PlaceParticles(config.getParticleCount());

    // Start a physics thread
    _threads.push_back(std::make_unique<std::thread>(std::thread([&]() { physics2D.run(); })));
    _threads.push_back(std::make_unique<std::thread>(std::thread([&]() { physics2D.collider(); })));

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
            controller.HandleInput(running, keys);
            if (keys.heat) physics2D.changeEnergy(2.0f);
            if (keys.cool) physics2D.changeEnergy(0.5f);
            if (keys.plus) {
                placeMolecule(new N2(), Vector3());
                placeMolecule(new O2(), Vector3());
                config.setParticleCount(config.getParticleCount() + 2);
            }
            if (keys.minus && _particles.size() > 2) {
                _particles.popBack();
                _particles.popBack();
                config.setParticleCount(config.getParticleCount() - 2);
            }
            //if (keys.minus) physics2D.removeParticle();
            keys = KeyState{false, false, false};

            // Compute physics in extra thread and only render here
            renderer.render(_particles);

            frame_count++;

            // After every second, update the window title.
            frame_end = std::chrono::system_clock::now();
            long timeSinceLastWindowsUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
                    frame_end - title_timestamp).count();
            if (timeSinceLastWindowsUpdate >= 1000) {
                renderer.UpdateWindowTitle(config.getParticleCount(), frame_count,
                                           physics2D.getCollisionsSincelastCall());
                frame_count = 0;
                title_timestamp = frame_end;
            }

            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();

        }

    }

}

void Simulation::PlaceParticles(int const count) {

    int N2Count = (count * 78) / 100;
    int O2Count = (count * 21) / 100;
    int CO2Count = std::max((count / 100) * 1, 1);
    std::cout << "N2Count = " << N2Count << std::endl;
    std::cout << "O2Count = " << O2Count << std::endl;
    std::cout << "CO2Count = " << CO2Count << std::endl;
    int i = 0;
    while (i++ < N2Count) {
        placeMolecule(new N2(), Vector3(random_v(engine), random_v(engine), 0.0));
    }
    i = 0;
    while (i++ < O2Count) {
        placeMolecule(new O2(), Vector3(random_v(engine), random_v(engine), 0.0));
    }
    i = 0;
    while (i++ < CO2Count) {
        placeMolecule(new CO2(), Vector3(random_v(engine), random_v(engine), 0.0));
    }
}

void Simulation::placeMolecule(Molecule *molecule, Vector3 velocity) {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);
        if (x >= 0 && x <= config.getWindowWidth() && y >= 0 && y <= config.getWindowHeight()) {
            std::unique_ptr<SimulationObject> m(molecule);
            Particle &part = m->getParticle();
            part.setPosition(x, y, 0.0);
            part.setVelocity(velocity);
            part.setAcceleration(Vector3(Vector3::GRAVITY) * -config.getGravityFactor());
            part.setDamping(config.getDamping());
            _particles.pushBack(std::move(m));
            break;
        }
    }
}
