//
// Created by Trebing, Peter on 2019-08-27.
//

#include <thread>
#include <utility>
#include "particlePhysics2D.h"

struct Point {
    double x, y;
};

bool hasIntersection(Point l1, Point r1, Point l2, Point r2) {

    // If one rectangle is on left side of other
    if (l1.x > r2.x || l2.x > r1.x)
        return false;

    // If one rectangle is above other
    if (l1.y > r2.y || l2.y > r1.y)
        return false;

    return true;
}

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
        if (timeSinceLastUpdate >= target_physics_interval) {
            integrate(static_cast<double>(timeSinceLastUpdate) / 1000.0f);
            lastUpdate = std::chrono::system_clock::now();
        }

    }

}

void PatrticlePhysics2D::collider() {

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
        if (timeSinceLastUpdate >= target_physics_interval) {
            detectCollisions();
            lastUpdate = std::chrono::system_clock::now();
        }

    }

}

PatrticlePhysics2D::~PatrticlePhysics2D() {
}

void PatrticlePhysics2D::integrate(double duration) {

    std::size_t width = grid_width;
    std::size_t height = grid_height;

    _particles.map([duration, width, height](std::shared_ptr<SimulationObject> &obj, size_t i) -> bool {

        Particle &part = obj->getParticle();

        part.integrate(duration);

        Vector3 position = part.getPosition();
        Vector3 velocity = part.getVelocity();
        bool boxCollision = false;
        if (position.x <= 0.0) {
            velocity.x *= -1.0;
            // position.x = grid_width;
            boxCollision = true;
        } else if (position.x > width) {
            velocity.x *= -1.0;
            //position.x = 0.0;
            boxCollision = true;
        }
        if (position.y <= 0.0) {
            velocity.y *= -1.0;
            //position.y = grid_height;
            boxCollision = true;
        } else if (position.y > height) {
            velocity.y *= -1.0;
            //position.y = 0.0;
            boxCollision = true;
        }
        if (boxCollision) {
            part.setVelocity(velocity);
        }

        return false;
    });

}

void PatrticlePhysics2D::detectCollisions() {

    // Count helps to fulfill the maximum collision limit
    std::size_t count = 0;

    _particles.map([&](std::shared_ptr<SimulationObject> &obj1, size_t i) -> bool {

        _particles.map([&](std::shared_ptr<SimulationObject> &obj2, size_t j) -> bool {

            if (++count >= collision_limit) return true; //stop map
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


                //collidingPairs.push_back(std::make_pair(obj1, obj2));

                resolveCollisions(obj1, obj2, part1, part2);

            }
            return false;
        });
        return false;
    });
}

void
PatrticlePhysics2D::resolveCollisions(std::shared_ptr<SimulationObject> &obj1,
                                      std::shared_ptr<SimulationObject> &obj2,
                                      Particle &part1, Particle &part2) const {
    Vector3 p1 = part1.getPosition();
    double size1 = static_cast<double>(obj1->getSize());

    Vector3 p2 = part2.getPosition();
    double size2 = static_cast<double>(obj2->getSize());

    Point l1 = {p1.x, p1.y};
    Point r1 = {p1.x + size1, p1.y + size1};

    Point l2 = {p2.x, p2.y};
    Point r2 = {p2.x + size2, p2.y + size2};


    // Collision has occurred
    Vector3 displacement = getDisplacement(l1, r1, l2, r2) * 0.501f;

    Vector3 v1 = part1.getVelocity();
    double m1 = part1.getMass();
    Vector3 v2 = part2.getVelocity();
    double m2 = part2.getMass();

    // FInd math in the following paper'
// https://imada.sdu.dk/~rolf/Edu/DM815/E10/2dcollisions.pdf
    Vector3 normal = (p2 - p1);
    Vector3 unitNormal = normal * (1.0f / sqrt(normal.squareMagnitude()));
    Vector3 unitTangent = Vector3(-unitNormal.y, unitNormal.x, 0.0f);

    // Dot Product Tangent
    double v1t = v1.scalarProduct(unitTangent);
    double v2t = v2.scalarProduct(unitTangent);

    // Dot Product Normal
    double v1n = v1.scalarProduct(unitNormal);
    double v2n = v2.scalarProduct(unitNormal);

    double m1addm2 = m1 + m2;
    double v1nNext = (v1n * (m1 - m2) + 2.0f * m2 * v2n) / m1addm2;
    double v2nNext = (v2n * (m2 - m1) + 2.0f * m1 * v1n) / m1addm2;

    // Update ball velocities
    Vector3 v1normalNext = unitNormal * v1nNext;
    Vector3 v2normalNext = unitNormal * v2nNext;

    Vector3 v1TangentialNext = unitTangent * v1t;
    Vector3 v2TangentialNext = unitTangent * v2t;

    Vector3 v1Next = v1normalNext + v1TangentialNext;
    Vector3 v2Next = v2normalNext + v2TangentialNext;

    part1.setVelocity(v1Next);
    part2.setVelocity(v2Next);

    part1.setPosition(p1 + displacement);
    part2.setPosition(p2 - displacement);
}

void PatrticlePhysics2D::resolveCollisions() {
    // Now work out dynamic collisions
    float fEfficiency = 1.00f;
    /*
    for (auto c : collidingPairs) {
        sBall *b1 = c.first, *b2 = c.second;

        // Distance between balls
        float fDistance = sqrtf((b1->px - b2->px) * (b1->px - b2->px) + (b1->py - b2->py) * (b1->py - b2->py));

        // Normal
        float nx = (b2->px - b1->px) / fDistance;
        float ny = (b2->py - b1->py) / fDistance;

        // Tangent
        float tx = -ny;
        float ty = nx;

        // Dot Product Tangent
        float dpTan1 = b1->vx * tx + b1->vy * ty;
        float dpTan2 = b2->vx * tx + b2->vy * ty;

        // Dot Product Normal
        float dpNorm1 = b1->vx * nx + b1->vy * ny;
        float dpNorm2 = b2->vx * nx + b2->vy * ny;

        // Conservation of momentum in 1D
        float m1 = fEfficiency * (dpNorm1 * (b1->mass - b2->mass) + 2.0f * b2->mass * dpNorm2) / (b1->mass + b2->mass);
        float m2 = fEfficiency * (dpNorm2 * (b2->mass - b1->mass) + 2.0f * b1->mass * dpNorm1) / (b1->mass + b2->mass);

        // Update ball velocities
        b1->vx = tx * dpTan1 + nx * m1;
        b1->vy = ty * dpTan1 + ny * m1;
        b2->vx = tx * dpTan2 + nx * m2;
        b2->vy = ty * dpTan2 + ny * m2;
    }

    // Remove collisions
    collidingPairs.clear();
*/
}

