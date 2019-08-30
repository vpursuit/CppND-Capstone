//
// Created by Trebing, Peter on 2019-08-26.
//

#ifndef COLLISIONSIM_SIMULATIONOBJECT_H
#define COLLISIONSIM_SIMULATIONOBJECT_H

#include <future>
#include <thread>
#include <vector>
#include "particle.h"
#include "synchronizedList.h"

struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class SimulationObject {

public:

    SimulationObject() : part(Particle()) {};

    virtual ~SimulationObject(){};

    Particle &getParticle() { return part; };

    virtual RGBA getColor() = 0;

    virtual size_t getSize() = 0;

protected:
    Particle part;

};

typedef SynchronizedList<SimulationObject> SimulationObjects;


class Molecule : public SimulationObject {
public:
    Molecule(RGBA color, size_t size) : SimulationObject(), color(color), size(size) {};
    ~Molecule(){};

    RGBA getColor(){ return color; }
    size_t getSize(){ return size; }
private:
    RGBA color;
    size_t size;

};

class N2 : public Molecule {
public:
    N2() : Molecule({0, 102, 153, 255}, 6) {
        part.setMass(10.0 * 14.0067);
    }
};

class O2 : public Molecule {
public:
    O2() : Molecule({255, 102, 52, 255}, 6) {
        part.setMass(10.0 * 14.0067);
    }
};

class CO2 : public Molecule {
public:
    CO2() : Molecule({0, 0, 0, 255}, 6) {
        part.setMass(10.0 * 14.0067);
    }
};

#endif //COLLISIONSIM_SIMULATIONOBJECT_H
