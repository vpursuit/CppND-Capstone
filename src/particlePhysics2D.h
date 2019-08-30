//
// Created by Trebing, Peter on 2019-08-27.
//

#ifndef COLLISIONSIM_PARTICLEPHYSICS2D_H
#define COLLISIONSIM_PARTICLEPHYSICS2D_H

#include <thread>
#include "simulationObject.h"
#include "stoppable.h"
#include "configuration.h"

class PatrticlePhysics2D : public Stoppable {

public:

    PatrticlePhysics2D(Configuration configuration,
                       SimulationObjects &particles) :
            Stoppable(),
            config(configuration),
            collisions(0),
            _particles(particles) {};

    ~PatrticlePhysics2D();

    void run();

    void collider();

    std::size_t getCollisionsSincelastCall() {
        std::lock_guard<std::mutex> uLock(_mutex);
        std::size_t result = collisions;
        return result;
    }

private:

    std::mutex _mutex;
    std::size_t collisions;

    void integrate(double duration);

    std::size_t detectCollisions();

    void resolveCollisions(std::shared_ptr<SimulationObject> &obj1,
                           std::shared_ptr<SimulationObject> &obj2);

    SimulationObjects &_particles;

    Configuration config;

};


#endif //COLLISIONSIM_PARTICLEPHYSICS2D_H
