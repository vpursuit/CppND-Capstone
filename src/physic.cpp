//
// Created by Trebing, Peter on 2019-08-22.
//
#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include "physic.h"

void Physics::update(std::vector<Particle> particles) {

    // print id of the current thread
    //std::unique_lock<std::mutex> lck(_mtx);
    //lck.unlock();

    // initalize variables

    double cycleDuration = 1; // duration of a single simulation cycle in ms
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();
    while (true) {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now() - lastUpdate).count();

        if (timeSinceLastUpdate >= cycleDuration) {

            for (auto &part: particles) {

                part.integrate(static_cast<double>(timeSinceLastUpdate) / 1000.0f);

                // Check box collisons
                Vector3 position = part.getPosition();
                Vector3 velocity = part.getVelocity();
                bool boxCollision = false;
                if (position.x <= 0.0) {
                    velocity.x *= -1.0;
                    boxCollision = true;
                } else if (position.x > box_width) {
                    velocity.x *= -1.0;
                    boxCollision = true;
                }
                if (position.y <= 0.0) {
                    velocity.y *= -1.0;
                    boxCollision = true;
                } else if (position.y > box_height) {
                    velocity.y *= -1.0;
                    boxCollision = true;
                }
                if (boxCollision) {
                    part.setVelocity(velocity);
                }
            }

            // reset stop watch for next cycle
            lastUpdate = std::chrono::system_clock::now();
        }
    }
}