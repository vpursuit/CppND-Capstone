//
// Created by Trebing, Peter on 2019-08-22.
//

/*
 * Implementation file for the particle class.
 * 
 * Part of the sim physics system.
 * 
 * Copyright (c) Icosagon 2003. All Rights Reserved.
 *
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */


#include <assert.h>
#include "particle.h"

using namespace sim;

void Particle::integrate(double duration) {

    // We don't integrate things with zero mass.
    if (inverseMass <= 0.0f) return;

    // Integrate only if some time passed
    if (duration <= 0.0f) return;
    // assert(duration > 0.0);

    // _update linear position.
    position.addScaledVector(velocity, duration);

    // Work out the acceleration from the force
    Vector3 resultingAcc = acceleration;
    resultingAcc.addScaledVector(forceAccum, inverseMass);

    // _update linear velocity from the acceleration.
    velocity.addScaledVector(resultingAcc, duration);

    // Impose drag.
    velocity *= pow(damping, duration);

    // Clear the forces.
    clearAccumulator();

}

void Particle::setMass(const double mass) {
    assert(mass != 0);
    Particle::inverseMass = ((double) 1.0) / mass;
}

double Particle::getMass() const {
    if (inverseMass == 0) {
        return std::numeric_limits<double>::max();
    } else {
        return ((double) 1.0) / inverseMass;
    }
}

void Particle::setInverseMass(const double inverseMass) {
    Particle::inverseMass = inverseMass;
}

double Particle::getInverseMass() const {
    return inverseMass;
}

bool Particle::hasFiniteMass() const {
    return inverseMass >= 0.0f;
}

void Particle::setDamping(const double damping) {
    Particle::damping = damping;
}

double Particle::getDamping() const {
    return damping;
}

void Particle::setPosition(const Vector3 &position) {
    Particle::position = position;
}

void Particle::setPosition(const double x, const double y, const double z) {
    position.x = x;
    position.y = y;
    position.z = z;
}

void Particle::getPosition(Vector3 *position) const {
    *position = Particle::position;
}

Vector3 Particle::getPosition() const {
    return position;
}

void Particle::setVelocity(const Vector3 &velocity) {
    Particle::velocity = velocity;
}

void Particle::setVelocity(const double x, const double y, const double z) {
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void Particle::getVelocity(Vector3 *velocity) const {
    *velocity = Particle::velocity;
}

Vector3 Particle::getVelocity() const {
    return velocity;
}

void Particle::setAcceleration(const Vector3 &acceleration) {
    Particle::acceleration = acceleration;
}

void Particle::setAcceleration(const double x, const double y, const double z) {
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;
}

void Particle::getAcceleration(Vector3 *acceleration) const {
    *acceleration = Particle::acceleration;
}

Vector3 Particle::getAcceleration() const {
    return acceleration;
}

void Particle::clearAccumulator() {
    forceAccum.clear();
}

void Particle::addForce(const Vector3 &force) {
    forceAccum += force;
}



