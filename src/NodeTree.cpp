#include <NodeTree.h>

NodeTree::NodeTree(Particle* particle, Vector2 size, int numOfP)
{
	this->sizeOfScreen = size; // adds the size of the screen from blobdemo
	this->particles = particle; // adds the particle array from blobdemo
	this->numOfParticles = numOfP; // adds the number of particles that exist from blobdemo
	// calls the createTree function 
	createTree();

}
NodeTree::~NodeTree()
{
	delete this;
}
// creates tree function
void NodeTree::createTree()
{
	// check if the rootNode does not equal null to delete it
	if (rootNode != nullptr)
		delete rootNode;

	// creates a new root node of quad tree 
	rootNode = new Quadtree(0, 0, sizeOfScreen.x, sizeOfScreen.y);

	// loops through all the particles in the scene
	for (int i = 0; i < numOfParticles; i++)
	{
		// sends each particle to the quad tree to determine which sector it should be in
		rootNode->insert(&particles[i]);
	}
}
// update function
void NodeTree::update() 
{
	// calls the create tree function each update
	createTree();
}

unsigned NodeTree::addContact(ParticleContact * contact, unsigned limit) const
{
	unsigned used = 0;

	std::vector<std::vector<Particle*>> particleSectors;
	particleSectors = rootNode->getObjects(particleSectors);

	for (std::vector<std::vector<Particle*>>::iterator i = particleSectors.begin(); i != std::prev(particleSectors.end()); i++)
	{
		if ((*i).size() >= 2)
		{
			//runs a for loop of count "BLOB_COUNT" - 1
			for (std::vector<Particle*>::const_iterator j = (*i).begin(); j != std::prev((*i).end()); j++)
			{
				if (used >= limit) return used;
				// runs a second for loop from the first loop + 1 as this will check the next circles in the array  
				for (vector<Particle*>::const_iterator k = std::next(j); k != (*i).end(); k++)
				{
					//sends the particles into a function called checkCollision
					if (checkCollision((*j), (*k)))
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
				}
			}
		}
	}
	return used;
}
// function to call the Quad tree debug function
void NodeTree::drawTree()
{
	//calls the quad tree's debug function show
	rootNode->show();
}

bool NodeTree::checkCollision(Particle* circle1, Particle* circle2) const
{
	//takes the checkCollision parameters and gets their position.magnitude and send it to a float called Distance
	float distance = (circle1->getPosition() - circle2->getPosition()).magnitude();
	// after it gets the distance between both particles, it checks to see if the distance is greater than both of their radius's to check if they are touching
	if (distance < circle1->getRadius() + circle2->getRadius())
		// if so, they are touching return true that they are colliding
		return true;
	else
		//if false meaning they aren't colliding
		return false;
}