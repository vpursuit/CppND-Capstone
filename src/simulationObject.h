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

/**
 * Representation of the color attributes of an simulation object
 */
struct RGBA {
    uint8_t r; // red
    uint8_t g; // green
    uint8_t b; // blue
    uint8_t a; // transparency
};

/**
 * A attribute to mark simulation object sensitive against acceleration by user interacti0n
 */
enum Sensitivity {
    sensitive,
    insensitive
};

/**
 * SimulationObject ist an aggregation of Particle, which defines physical attributes
 * and the visual outline of the object
 */
class SimulationObject {

public:

    SimulationObject() : part(Particle()), sensitivity(insensitive) {};

    virtual ~SimulationObject() {};

    /**
     * Returns the Particle instance of the obejct
     */
    Particle &getParticle() { return part; };

    /**
     * Returns the color
     * @return RGBA instance
     */
    virtual RGBA getColor() = 0;

    /**
     * Returns size of the object to be used in rendering and collision detection
     * @return
     */
    virtual size_t getSize() = 0;

    /**
     * Returns the sensitivity marker of the siumulation object. When it is sensitive,
     * the object shall be sensitive to user interaction, otherwise it follows only the
     * particle systems internal paramters.
     */
    Sensitivity getSensitivity() { return sensitivity; };

protected:
    Particle part;
    Sensitivity sensitivity;

};

typedef SynchronizedList<SimulationObject> SimulationObjects;


class Molecule : public SimulationObject {
public:
    Molecule(RGBA color, size_t size) : SimulationObject(), color(color), size(size) {};

    ~Molecule() {};

    RGBA getColor() { return color; }

    size_t getSize() { return size; }

private:
    RGBA color;
    size_t size;

};

#endif //COLLISIONSIM_SIMULATIONOBJECT_H
