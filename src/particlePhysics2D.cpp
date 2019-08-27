//
// Created by Trebing, Peter on 2019-08-27.
//

#include <thread>
#include "particlePhysics2D.h"

void PatrticlePhysics2D::run() {

    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();

    while (stopRequested() == false) {

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

PatrticlePhysics2D::~PatrticlePhysics2D() {
}

void PatrticlePhysics2D::_update(double duration) {

    std::size_t width = grid_width;
    std::size_t height = grid_height;

    _particles.map([duration, width, height](std::unique_ptr<SimulationObject> &obj, size_t i) mutable {

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