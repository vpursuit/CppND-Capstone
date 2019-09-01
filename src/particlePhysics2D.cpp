//
// Created by Trebing, Peter on 2019-08-27.
//

#include <thread>
#include <utility>
#include "particlePhysics2D.h"

// Representation of a point in 2D
struct Point {
    double x, y;
};

/** Checks if two rectangles do intersect - used to check for collision
 * @param l1 left upper point of rechtangle 1
 * @param r1 right lower point of rechtangle 1
 * @param l2 left upper point of rechtangle 2
 * @param r2 right lower point of rechtangle 2
 */
bool hasIntersection(Point l1, Point r1, Point l2, Point r2) {

    // If one rectangle is on left side of other
    if (l1.x > r2.x || l2.x > r1.x)
        return false;

    // If one rectangle is above other
    if (l1.y > r2.y || l2.y > r1.y)
        return false;

    return true;
}

/**
 * Computes the necessary displacement of two rectangles after collsion - to prevent
 * detecting a collision over and over again
 * @param l1 left upper point of rechtangle 1
 * @param r1 right lower point of rechtangle 1
 * @param l2 left upper point of rechtangle 3
 * @param r2 right lower point of rechtangle 2
 * @return a vector instance representing  the displacement in component x and y (z will always be 0)
 */
Vector3 getDisplacement(Point l1, Point r1, Point l2, Point r2) {

    Vector3 result;

    result.x = (std::min(r1.x, r2.x) -
                std::max(l1.x, l2.x));

    result.y = (std::min(r1.y, r2.y) -
                std::max(l1.y, l2.y));

    result.z = 0;
    return result;
}

void PatrticlePhysics2D::run() {

    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();

    while (stopRequested() == false) {

        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now() - lastUpdate).count();

        // Its time to render a frame to met the fps spec.
        if (timeSinceLastUpdate >= config.getPhysicIntervalMs()) {
            integrate(static_cast<double>(timeSinceLastUpdate) / 1000.0f);
            lastUpdate = std::chrono::system_clock::now();
        }

    }

}

void PatrticlePhysics2D::collider() {

    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    std::chrono::time_point<std::chrono::system_clock> collisionDetectionStart;

    // init stop watch
    lastUpdate = std::chrono::system_clock::now();

    while (stopRequested() == false) {

        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now() - lastUpdate).count();

        if (timeSinceLastUpdate >= config.getPhysicIntervalMs()) {
            std::size_t collisionsDetected = detectCollisions();
            lastUpdate = std::chrono::system_clock::now();
            collisions += collisionsDetected;
        }

    }

}

PatrticlePhysics2D::~PatrticlePhysics2D() {
}

void PatrticlePhysics2D::integrate(double duration) {

    std::size_t width = config.getWindowWidth();
    std::size_t height = config.getWindowHeight();

    _particles.map([duration, width, height](std::shared_ptr<SimulationObject> &obj, size_t i) -> bool {

        Particle &part = obj->getParticle();

        part.integrate(duration);

        Vector3 position = part.getPosition();
        Vector3 velocity = part.getVelocity();
        bool boxCollision = false;
        if (position.x <= 0.0) {
            velocity.x *= -1.0;
            position.x = 0.0f;
            // position.x = grid_width;
            boxCollision = true;
        } else if (position.x > width) {
            velocity.x *= -1.0;
            position.x = width;
            boxCollision = true;
        }
        if (position.y <= 0.0) {
            velocity.y *= -1.0;
            position.y = 0.0f;
            boxCollision = true;
        } else if (position.y > height) {
            velocity.y *= -1.0;
            position.y = height;
            boxCollision = true;
        }
        if (boxCollision) {
            part.setVelocity(velocity);
            part.setPosition(position);
        }

        return false;
    });

}

std::size_t PatrticlePhysics2D::detectCollisions() {

    // Count helps to fulfill the maximum collision limit
    std::size_t checkCount = 0;
    std::size_t collisionCount = 0;

    _particles.map([&](std::shared_ptr<SimulationObject> &obj1, size_t i) -> bool {

        _particles.map([&](std::shared_ptr<SimulationObject> &obj2, size_t j) -> bool {

            if (++checkCount >= config.getCollisionLimit()) return true; //stop map
            if (i == j) return false; // do not collide particle with itself, but continue

            Particle &part1 = obj1->getParticle();
            Particle &part2 = obj2->getParticle();

            Vector3 p1 = part1.getPosition();
            double size1 = static_cast<double>(obj1->getSize());

            Vector3 p2 = part2.getPosition();
            double size2 = static_cast<double>(obj2->getSize());

            Point l1 = {p1.x, p1.y};
            Point r1 = {p1.x + size1, p1.y + size1};

            Point l2 = {p2.x, p2.y};
            Point r2 = {p2.x + size2, p2.y + size2};

            if (hasIntersection(l1, r1, l2, r2)) {
                ++collisionCount;
                resolveCollisions(obj1, obj2);
            }
            return false; // go on mapping list
        });
        return false; // go on mapping list
    });
    return collisionCount;
}

void
PatrticlePhysics2D::resolveCollisions(std::shared_ptr<SimulationObject> &obj1,
                                      std::shared_ptr<SimulationObject> &obj2) {

    Particle &part1 = obj1->getParticle();
    Particle &part2 = obj2->getParticle();

    Vector3 p1 = part1.getPosition();
    double size1 = static_cast<double>(obj1->getSize());

    Vector3 p2 = part2.getPosition();
    double size2 = static_cast<double>(obj2->getSize());

    Point l1 = {p1.x, p1.y};
    Point r1 = {p1.x + size1, p1.y + size1};

    Point l2 = {p2.x, p2.y};
    Point r2 = {p2.x + size2, p2.y + size2};

    // Collision has occurred
    Vector3 displacement = getDisplacement(l1, r1, l2, r2);
    part1.setPosition(p1 + displacement);
    part2.setPosition(p2 - displacement);

    Vector3 v1 = part1.getVelocity();
    double m1 = part1.getMass();
    Vector3 v2 = part2.getVelocity();
    double m2 = part2.getMass();

    // FInd math in the following paper'
    // https://imada.sdu.dk/~rolf/Edu/DM815/E10/2dcollisions.pdf
    Vector3 normal = (p2 - p1);
    double length = sqrt(normal.squareMagnitude());
    Vector3 unitNormal = normal * (1.0f / length);
    Vector3 unitTangent = Vector3(-unitNormal.y, unitNormal.x, 0.0f);

    // Dot Product Tangent
    double v1t = v1.scalarProduct(unitTangent);
    if (isnan(v1t)) return;
    double v2t = v2.scalarProduct(unitTangent);
    if (isnan(v2t)) return;

    // Dot Product Normal
    double v1n = v1.scalarProduct(unitNormal);
    if (isnan(v1n)) return;
    double v2n = v2.scalarProduct(unitNormal);
    if (isnan(v2n)) return;

    double velocityRange = (double) config.getParticleVelocityRange();
    double m1addm2 = m1 + m2;
    double v1nNext = (v1n * (m1 - m2) + 2.0f * m2 * v2n) / m1addm2;
    if (isnan(v1nNext)) return;
    double v2nNext = (v2n * (m2 - m1) + 2.0f * m1 * v1n) / m1addm2;
    if (isnan(v2nNext)) return;

    // Update molecule velocities
    Vector3 v1normalNext = unitNormal * v1nNext;
    Vector3 v2normalNext = unitNormal * v2nNext;

    Vector3 v1TangentialNext = unitTangent * v1t;
    Vector3 v2TangentialNext = unitTangent * v2t;

    Vector3 v1Next = v1normalNext + v1TangentialNext;
    Vector3 v2Next = v2normalNext + v2TangentialNext;

    part1.setVelocity(v1Next);
    part2.setVelocity(v2Next);

}

void PatrticlePhysics2D::changeEnergy(double factor) {
    _particles.map([factor](std::shared_ptr<SimulationObject> &obj, size_t i) -> bool {
        if (obj->getSensitivity() == Sensitivity::sensitive) {
            Particle &part = obj->getParticle();
            Vector3 velocity = part.getVelocity();
            part.setVelocity(velocity * factor);
        }
        return false;
    });
}

