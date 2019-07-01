
#include <float.h>
#include <pcontacts.h>


// Contact implementation
void ParticleContact::resolve(float duration)
{
    resolveVelocity(duration);
}

float ParticleContact::calculateSeparatingVelocity() const
{
    Vector2 relativeVelocity = particle[0]->getVelocity();
    if (particle[1]) relativeVelocity -= particle[1]->getVelocity();
    return relativeVelocity * contactNormal;
}

void ParticleContact::resolveVelocity(float duration)
{
    // Find the velocity in the direction of the contact
    float separatingVelocity = calculateSeparatingVelocity();

    // Check if it needs to be resolved
    if (separatingVelocity > 0)
    {
        // The contact is either separating, or stationary - there's
        // no impulse required.
        return;
    }

    // Calculate the new separating velocity
    float newSepVelocity = -separatingVelocity * restitution;
    float deltaVelocity = newSepVelocity - separatingVelocity;

    // We apply the change in velocity to each object in proportion to
    // their inverse mass (i.e. those with lower inverse mass [higher
    // actual mass] get less change in velocity)..
    float totalInverseMass = particle[0]->getInverseMass();
    if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

    // If all particles have infinite mass, then impulses have no effect
    if (totalInverseMass <= 0) return;

    // Calculate the impulse to apply
    float impulse = deltaVelocity / totalInverseMass;

    // Find the amount of impulse per unit of inverse mass
    Vector2 impulsePerIMass = contactNormal * impulse;

    // Apply impulses: they are applied in the direction of the contact,
    // and are proportional to the inverse mass.
    particle[0]->setVelocity(particle[0]->getVelocity() +
        impulsePerIMass * particle[0]->getInverseMass()
        );
    if (particle[1])
    {
        // Particle 1 goes in the opposite direction
        particle[1]->setVelocity(particle[1]->getVelocity() +
            impulsePerIMass * -particle[1]->getInverseMass()
            );
    }
}

ParticleContactResolver::ParticleContactResolver(unsigned iterations)
:
iterations(iterations)
{
}

void ParticleContactResolver::setIterations(unsigned iterations)
{
    ParticleContactResolver::iterations = iterations;
}

void ParticleContactResolver::resolveContacts(ParticleContact *contactArray,
                                              unsigned numContacts,
                                              float duration)
{
    unsigned i;

    iterationsUsed = 0;
	while (iterationsUsed < iterations)
	{
		// Find the contact with the largest closing velocity;
		float max = DBL_MAX;
		unsigned maxIndex = numContacts;
		for (i = 0; i < numContacts; i++)
		{
			float sepVel = contactArray[i].calculateSeparatingVelocity();
			if (sepVel < max &&
				(sepVel < 0 || contactArray[i].penetration > 0))
			{
				max = sepVel;
				maxIndex = i;
			}
		}
		//Do we have anything worth resolving?
		if (maxIndex == numContacts) break;

		// Resolve this contact
		contactArray[maxIndex].resolve(duration);


		iterationsUsed++;
	}

}

void ParticleContact::resolveInterpenetration(float duration)
{
	// If we don;t have any penetration, skip this step
	if (penetration <= 0) return;
	// The movement of each object is based on their inverse mass, so total that
	float totalInverseMass = particle[0]->getInverseMass();
	if (particle[1]) totalInverseMass += particle[1]->getInverseMass();
	// If all particles have infinite mass, then we do nothing
	if (totalInverseMass <= 0) return;
	// Find the amount of penetration resolution per unit of unverse mass
	Vector2 movePerIMass = contactNormal * (penetration / totalInverseMass);
	// Calculate the movement amounts
	particleMovement[0] = movePerIMass * particle[0]->getInverseMass();
	if (particle[1])
		particleMovement[1] = movePerIMass * -particle[1]->getInverseMass();
	else
		particleMovement[1].clear();

	// Apply the penetraion resolution
	particle[0]->setPosition(particle[0]->getPosition() + particleMovement[0]);
	if (particle[1])
		particle[1]->setPosition(particle[1]->getPosition() + particleMovement[1]);
}