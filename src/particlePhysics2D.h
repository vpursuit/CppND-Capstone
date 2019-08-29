//
// Created by Trebing, Peter on 2019-08-27.
//

#ifndef COLLISIONSIM_PARTICLEPHYSICS2D_H
#define COLLISIONSIM_PARTICLEPHYSICS2D_H

#include "simulationObject.h"
#include "stoppable.h"

typedef std::pair<std::shared_ptr<SimulationObject>, std::shared_ptr<SimulationObject>> CollsionPair;

class PatrticlePhysics2D : public Stoppable {

public:

    PatrticlePhysics2D(std::size_t grid_width,
                       std::size_t grid_height,
                       std::size_t target_physics_interval,
                       std::size_t collision_limit,
                       SimulationObjects &particles) :
            Stoppable(),
            grid_width(grid_width),
            grid_height(grid_height),
            target_physics_interval(target_physics_interval),
            _particles(particles),
            collision_limit(collision_limit){};

    PatrticlePhysics2D(const PatrticlePhysics2D &other) :
            grid_width(other.grid_width),
            grid_height(other.grid_height),
            target_physics_interval(other.target_physics_interval),
            _particles(other._particles) {};

    ~PatrticlePhysics2D();

    void run();
    void collider();

private:
    void integrate(double duration);

    void resolveCollisions();

    void detectCollisions();

    SimulationObjects &_particles;

    std::vector<CollsionPair> collidingPairs;

    std::size_t grid_width;
    std::size_t grid_height;
    std::size_t target_physics_interval;
    std::size_t collision_limit;

    void
    resolveCollisions(std::shared_ptr<SimulationObject> &obj1, std::shared_ptr<SimulationObject> &obj2, Particle &part1,
                      Particle &part2) const;
};


#endif //COLLISIONSIM_PARTICLEPHYSICS2D_H
