#include <TreeStructure.h>

//TreeNode Constructor
TreeStructure::TreeStructure(Vector2 position, Vector2 size, TreeStructure* parent)
{
	this->position = position;
	this->size = size;
	this->parent = parent;
}
// Creates and positions the 4 child nodes and places them into an array of childNodes
TreeStructure* TreeStructure::createChildNodes()
{
	// creates a vector2 array called childNodePosition to create the new sizes and positions of the child nodes
	Vector2 childNodeP[4];
	// assigns each child node with a position and a size based of parameters sent from BlobDemo.cpp of the Scene
	childNodeP[0] = Vector2(position.x - (size.x / 4), position.y - (size.y / 4));
	childNodeP[1] = Vector2(position.x + (size.x / 4), position.y - (size.y / 4));
	childNodeP[2] = Vector2(position.x - (size.x / 4), position.y + (size.y / 4));
	childNodeP[3] = Vector2(position.x + (size.x / 4), position.y + (size.y / 4));

	for (int i = 0; i < 4; i++)
	{
		childNodes[i] = new TreeStructure(childNodeP[i], size, this);
	}

	hasChildnodes = true;

	return *childNodes;
}
// Checks the particles to find out which Node they should be in.
void TreeStructure::checkParticle(Particle* particle, int numOfParticles)
{
	this->particles.clear();

	if (hasChildnodes == true)
	{
		for (int i = 0; i < 4; i++)
		{
			childNodes[i]->moveParticles(particle, numOfParticles);
		}
	}
}
// moves the particle to the node depending on location of particle
std::list<Particle*> TreeStructure::moveParticles(Particle* particle, int numOfParticles) 
{
	std::list<Particle*> numOfParticle;

	for (int i = 0; i < numOfParticles; i++)
	{
		
	}
	return numOfParticle;
}
bool TreeStructure::barrierChecking(Particle* particle) 
{
	Vector2 position = particle->getPosition();
	float radius = particle->getRadius();
	return true;
}