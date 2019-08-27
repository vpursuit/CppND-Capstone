//
// Created by Trebing, Peter on 2019-08-27.
//

#ifndef COLLISIONSIM_PARTICLEPHYSICS2D_H
#define COLLISIONSIM_PARTICLEPHYSICS2D_H

#include "simulationObject.h"
#include "stoppable.h"

class PatrticlePhysics2D : public Stoppable {

public:

    PatrticlePhysics2D(std::size_t grid_width,
                       std::size_t grid_height,
                       std::size_t target_physics_interval,
                       SimulationObjects &particles) :
            Stoppable(),
            grid_width(grid_width),
            grid_height(grid_height),
            target_physics_interval(target_physics_interval),
            _particles(particles) {};

    PatrticlePhysics2D(const PatrticlePhysics2D &other) :
            grid_width(other.grid_width),
            grid_height(other.grid_height),
            target_physics_interval(other.target_physics_interval),
            _particles(other._particles) {};

    ~PatrticlePhysics2D();

    void run();

private:
    void _update(double duration);

    SimulationObjects &_particles;
    std::size_t grid_width;
    std::size_t grid_height;
    std::size_t target_physics_interval;

};


#endif //COLLISIONSIM_PARTICLEPHYSICS2D_H
