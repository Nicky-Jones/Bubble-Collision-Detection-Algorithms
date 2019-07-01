#include <particle.h>
#include <ParticleCollision.h>


ParticleCollision::ParticleCollision()
{
	for (int i = 0; i < 4; i++)
	{
		particles[i] = new vector<Particle*>;
	}
}

unsigned ParticleCollision::addContact(ParticleContact *contact, unsigned limit) const
{
	int a = 0;
	unsigned used = 0;
	//std::cout << "Current Algorithm: Brute Force Collision" << endl;
	for (int i = 0; i < blob_count - 1; i++)
	{
		if (used >= limit) return used;
		// runs a second for loop from the first loop + 1 as this will check the next circles in the array  
		for (int k = i + 1; k < blob_count; k++)
		{
			Particle particle1 = bruteParticles[i];
			Particle particle2 = bruteParticles[k];
			//sends the particles into a function called checkCollision
			if (checkCollision(particle1, particle2))
			{
				//cout << "there was a collision between " << i <<  " & " << j << " " << endl;
				//gets the contactNormal which is the first particles position - the second particles position and normalises the results
				contact->contactNormal = (particle1.getPosition() - particle2.getPosition()).unit();
				//resitution is the bounce back after colliding which will be a default 0.8f 
				contact->restitution = 1.0f;
				//holds what is in particles particles i
				contact->particle[0] = bruteParticles + i;
				// holds what is in particles j to check between the particles[0] & particles[1]
				contact->particle[1] = bruteParticles + k;
				// checks the penetration of the particles colliding by getting the radius of particle[i] - the magnitude of both particles.positions
				contact->penetration = particle1.getRadius() - (particle1.getPosition() - (particle2.getPosition())).magnitude();
				used++;
				contact++;
			}
			a++;
		}

	}
	//std::cout << "Number of overall collision checks were: " << a << endl;
	//std::cout << "Number of potential collision are: " << blob_count << endl;
	return used;
}
void ParticleCollision::bruteForceDraw()
{
	for (int i = 0; i < blob_count; i++)
	{
		for (int j = 0; j < blob_count; j++)
		{
			glBegin(GL_LINE_LOOP);
			glColor3f(0.0f, 1.0f, 0.0f); // hopefully green

			Vector2 start = bruteParticles[i].getPosition();
			Vector2 end = bruteParticles[j].getPosition();

			glVertex2f(start.x, start.y);
			glVertex2f(end.x, end.y);
			glEnd();
		}
	}
}
bool ParticleCollision::checkCollision(Particle circle1, Particle circle2) const
{
	//takes the checkCollision parameters and gets their position.magnitude and send it to a float called Distance
	float distance = (circle1.getPosition() - circle2.getPosition()).magnitude();
	// after it gets the distance between both particles, it checks to see if the distance is greater than both of their radius's to check if they are touching
	if (distance < circle1.getRadius() + circle2.getRadius())
		// if so, they are touching return true that they are colliding
		return true;
	else
		//if false meaning they aren't colliding
		return false;
}

ParticleCollision::~ParticleCollision()
{
	delete bruteParticles;
}
void ParticleCollision::clearLists()
{
	for (int i = 0; i < 4; i++)
	{
		particles[i]->clear();
	}
}
