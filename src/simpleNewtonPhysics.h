//
// Created by Trebing, Peter on 2019-08-22.
//

#ifndef COLLISIONSIM_SIMPLENEWTONPHYSICS_H
#define COLLISIONSIM_SIMPLENEWTONPHYSICS_H

#include "particle.h"

class Physics {
public:
    void update(std::vector<Particle> particles);

private:
    unsigned box_width, box_height;
    std::vector<std::thread> threads; // holds all threads that have been launched within this object
    static std::mutex _mtx;           // mutex shared by all traffic objects for protecting cout

};

#endif //COLLISIONSIM_SIMPLENEWTONPHYSICS_H
