#include <quadAlgorithm.h>

quadAlgorithm::quadAlgorithm()
{
	// instantiates a vector of type particles 
	for (int i = 0; i < 4; i++)
	{
		particles[i] = new vector<Particle*>;
	}
}
// function to check for collisions
unsigned quadAlgorithm::quadCollision(ParticleContact *contact, unsigned limit, unsigned used, unsigned particlesChecked) const
{
	int a = 0;
	for (int i = 0; i < 4; i++)
	{
		if (particles[i]->size() >= 2)
		{
			//runs a for loop of count "BLOB_COUNT" - 1
			for (vector<Particle*>::iterator j = particles[i]->begin(); j != std::prev(particles[i]->end()); j++)
			{
				if (used >= limit) return used;
				// runs a second for loop from the first loop + 1 as this will check the next circles in the array  
				for (vector<Particle*>::iterator k = std::next(j); k != particles[i]->end(); k++)
				{
					//sends the particles into a function called checkCollision
					if (checkCollision(*(*j), *(*k)))
					{
						//cout << "there was a collision between " << i <<  " & " << j << " " << endl;
						//gets the contactNormal which is the first particles position - the second particles position and normalises the results
						contact->contactNormal = ((*j)->getPosition() - (*k)->getPosition()).unit();
						//resitution is the bounce back after colliding which will be a default 0.8f 
						contact->restitution = 1.0f;
						//holds what is in particles particles i
						contact->particle[0] = (*j);
						// holds what is in particles j to check between the particles[0] & particles[1]
						contact->particle[1] = (*k);
						// checks the penetration of the particles colliding by getting the radius of particle[i] - the magnitude of both particles.positions
						contact->penetration = (*j)->getRadius() - ((*j)->getPosition() - (*k)->getPosition()).magnitude();
						used++;
						contact++;
					}
					a++;
				}
				particlesChecked++;
			}
			//std::cout << "Number of overall collision checks were: " << a << endl;
			//std::cout << "Number of potential collision in quad " << i << " = " << particlesChecked << endl;
			particlesChecked = 0;
			a = 0;
		}
	}

	return used;
}
// function to call for collision detection to check the speed of completion
unsigned quadAlgorithm::addContact(ParticleContact *contact, unsigned limit) const
{
	unsigned used = 0;
	//calls the quadCollision function to check for collisions
	used = quadCollision(contact, limit, used, pChecked);
	return used;
}
//deconstructor
quadAlgorithm::~quadAlgorithm()
{
	delete particles;
}
// function to check if two particles are touching each other
bool quadAlgorithm::checkCollision(Particle circle1, Particle circle2) const
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
// function to draw the debug lines
void quadAlgorithm::quadDraw()
{
	// draws the four quads on screen
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f); // red
	glVertex2f(0.0f, -100.0f);
	glVertex2f(0.0f, 100.0f);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0f, 0.0f, 0.0f); // red
	glVertex2f(-100.0f, 0.0f);
	glVertex2f(100.0f, 0.0f);
	glEnd();
	// loops through the particle vector
	for (int i = 0; i < 4; i++)
	{
		vector<Particle*>::iterator last = particles[i]->end();
		for (vector<Particle*>::iterator j = particles[i]->begin(); j != last; j++)
		{
			for (vector<Particle*>::iterator k = std::next(particles[i]->begin()); k != particles[i]->end(); k++)
			{
				// creates a line to each particle inside its own quad as a potential collision 
				glBegin(GL_LINE_LOOP);
				glColor3f(0.0f, 1.0f, 0.0f);

				Vector2 start = (*j)->getPosition();
				Vector2 end = (*k)->getPosition();

				glVertex2f(start.x, start.y);
				glVertex2f(end.x, end.y);
				glEnd();
			}
		}
	}
}
// function to clear the list of particles
void quadAlgorithm::clearLists()
{
	// clears the vector of particles
	for (int i = 0; i < 4; i++)
	{
		particles[i]->clear();
	}
}