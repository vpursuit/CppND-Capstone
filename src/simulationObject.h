//
// Created by Trebing, Peter on 2019-08-26.
//

#ifndef COLLISIONSIM_SIMULATIONOBJECT_H
#define COLLISIONSIM_SIMULATIONOBJECT_H

#include <future>
#include <thread>
#include <vector>
#include "particle.h"

using namespace sim;

struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class SimulationObject {

public:

    SimulationObject() : part(Particle()) {};

    Particle &getParticle() { return part; };

    virtual RGBA getColor() = 0;

    virtual size_t getSize() = 0;

protected:
    Particle part;

};

class SimulationObjects {
public:
    SimulationObjects() : _numSimulationObjects(0) {}

    int getNumSimulationObjects() {
        std::lock_guard<std::mutex> uLock(_mutex);
        return _numSimulationObjects;
    }

    bool isParticleAvailable() {
        std::lock_guard<std::mutex> myLock(_mutex);
        return !_simobjects.empty();
    }

    std::shared_ptr<SimulationObject> popBack() {
        // perform vector modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);

        // remove last vector element from queue
        std::shared_ptr<SimulationObject> v = std::move(_simobjects.back());
        _simobjects.pop_back();
        --_numSimulationObjects;

        return v; // will not be copied due to return value optimization (RVO) in C++
    }

    void map(const std::function<void(std::shared_ptr<SimulationObject> part, size_t)> &f) {
        // perform closure modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);
        size_t i = 0;
        for (std::shared_ptr<SimulationObject> part: _simobjects) {
            ++i;
            f(part, i);
        }
    }

    void pushBack(std::shared_ptr<SimulationObject> &&v) {
        // perform vector modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);
        _simobjects.emplace_back(std::move(v));
        ++_numSimulationObjects;
    }

private:
    std::vector<std::shared_ptr<SimulationObject>> _simobjects; // list of all _simobjects in the simulationn
    std::mutex _mutex;
    size_t _numSimulationObjects;
};

class Molecule : public SimulationObject {
public:
    Molecule(RGBA color, size_t size) : SimulationObject(), color(color), size(size) {};
    RGBA getColor(){ return color; }
    size_t getSize(){ return size; }
private:
    RGBA color;
    size_t size;

};

class N2 : public Molecule {
public:
    N2() : Molecule({100, 100, 100, 255}, 10) {
        part.setMass(2.0 * 14.0067);
    }
};

class O2 : public Molecule {
public:
    O2() : Molecule({0, 0, 255, 255}, 8) {
        part.setMass(2.0 * 14.0067);
    }
};

class CO2 : public Molecule {
public:
    CO2() : Molecule({255, 0, 0, 255}, 5) {
        part.setMass(2.0 * 14.0067);
    }
};

#endif //COLLISIONSIM_SIMULATIONOBJECT_H
