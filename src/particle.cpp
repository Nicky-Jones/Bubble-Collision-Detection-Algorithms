
#include "particle.h"
#include <math.h>
#include <assert.h>
#include <float.h>


void Particle::integrate(float duration)
{

	// We don't integrate things with zero mass.
   if (inverseMass <= 0.0f) return;

	assert(duration > 0.0);
	position.addScaledVector(velocity, duration);

	// Work out the acceleration from the force
    Vector2 resultingAcc = acceleration;
   resultingAcc.addScaledVector(forceAccum, inverseMass);

	// Update linear velocity from the acceleration.
    velocity.addScaledVector(resultingAcc, duration);

	// Impose drag.
	velocity *= pow(damping, duration);

	// Clear the forces.
    clearAccumulator();
}

void Particle::setMass(const float mass)
{
    assert(mass != 0);
    Particle::inverseMass = ((float)1.0)/mass;
}

float Particle::getMass() const
{
    if (inverseMass == 0) {
        return DBL_MAX;
    } else {
        return ((float)1.0)/inverseMass;
    }
}

void Particle::setInverseMass(const float inverseMass)
{
    Particle::inverseMass = inverseMass;
}

float Particle::getInverseMass() const
{
    return inverseMass;
}

bool Particle::hasFiniteMass() const
{
    return inverseMass >= 0.0f;
}


void Particle::setDamping(const float damping)
{
    Particle::damping = damping;
}

float Particle::getDamping() const
{
    return damping;
}

void Particle::setPosition(const float x, const float y)
{
    position.x = x;
    position.y = y;
}

void Particle::setPosition(const Vector2 &position)
{
	 Particle::position = position;
}


Vector2 Particle::getPosition() const
{
    return position;
}

void Particle::getPosition(Vector2 *position) const
{
    *position = Particle::position;
}

void Particle::setRadius(const float r)
{
    radius = r;
}

float Particle::getRadius() const
{
    return radius;
}


void Particle::setVelocity(const float x, const float y)
{
    velocity.x = x;
    velocity.y = y;
}

void Particle::setVelocity(const Vector2 &velocity)
{
    Particle::velocity = velocity;
}

Vector2 Particle::getVelocity() const
{
    return velocity;
}

void Particle::getVelocity(Vector2 *velocity) const
{
    *velocity = Particle::velocity;
}

void Particle::setAcceleration(const Vector2 &acceleration)
{
    Particle::acceleration = acceleration;
}


void Particle::setAcceleration(const float x, const float y)
{
    acceleration.x = x;
    acceleration.y = y;
}

Vector2 Particle::getAcceleration() const
{
    return acceleration;
}


void Particle::clearAccumulator()
{
    forceAccum.clear();
}

void Particle::addForce(const Vector2 &force)
{
    forceAccum += force;
}

