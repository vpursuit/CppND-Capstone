//
// Created by Trebing, Peter on 01.09.19.
//
#ifndef COLLISIONSIM_MOLECULES_H
#define COLLISIONSIM_MOLECULES_H

#include "simulationObject.h"

class N2 : public Molecule {
public:
    N2() : Molecule({0, 102, 153, 255}, 6) {
        part.setMass(50.0 * 14.0067);
    }
};

class O2 : public Molecule {
public:
    O2() : Molecule({255, 102, 52, 255}, 4) {
        part.setMass(10.0 * 14.0067);
    }
};

class CO2 : public Molecule {
public:
    CO2() : Molecule({0, 0, 0, 255}, 10) {
        sensitivity = sensitive;
        part.setMass(100.0 * 14.0067);
    }
};

#endif //COLLISIONSIM_MOLECULES_H
