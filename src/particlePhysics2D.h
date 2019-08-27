//
// Created by Trebing, Peter on 2019-08-27.
//

#ifndef COLLISIONSIM_PARTICLEPHYSICS2D_H
#define COLLISIONSIM_PARTICLEPHYSICS2D_H

#include <stddef.h>
#include "simulationObject.h"

class PatrticlePhysics2D {

public:

    PatrticlePhysics2D(size_t grid_width, size_t grid_height, SimulationObjects &particles) :
            grid_width(grid_width),
            grid_height(grid_height),
            _particles(particles) {};

    PatrticlePhysics2D(const PatrticlePhysics2D &other) :
            grid_width(other.grid_width),
            grid_height(other.grid_height),
            _particles(other._particles) {};

    ~PatrticlePhysics2D();

    void run(std::size_t target_physics_interval, std::future<void> futureObj);

private:
    void _update(double duration);

    SimulationObjects &_particles;
    size_t grid_width;
    size_t grid_height;

};


#endif //COLLISIONSIM_PARTICLEPHYSICS2D_H
