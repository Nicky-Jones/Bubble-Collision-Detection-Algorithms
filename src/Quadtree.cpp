#include <Quadtree.h>
//QuadTree constructor 
Quadtree::Quadtree(float x, float y, float width, float height) 
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}
//deletes the sectors
Quadtree::~Quadtree()
{
	if(NW != nullptr)
		delete NW;
	if(NE != nullptr)
		delete NE;
	if(SW != nullptr)
		delete SW;
	if(SE != nullptr)
		delete SE;
}
//recursively calls the function for each particle
bool Quadtree::insert(Particle* particles)
{
	// checks if this sector contains the particle
	if (!this->contains(particles))
	{
		return false;
	}
	//checks to see if the objects capacity is greater than the capacity of this sector
	if (this->objects.capacity() < this->capacity)
	{
		if (maxDepth <= 3)
		{
			// if it isn't, add the particle to this sectors object vector
			this->objects.push_back(particles);
			return true;
		}
	}
	// if it is
	else {
		// checks to see if the sector has been divided or not
		
		if (!this->divided && this->maxDepth <= 3) {
			//if not call the divide function and send the particle has a parameter
			divide();
			//Once a sector has been divided add objects in the parent sector to the child sectors by recursively calling this function with the object vector
			//to assign the particles to their new sectors
			for (int i = 0; i < this->capacity; i++)
			{
				this->NW->insert(objects[i]);
				this->NE->insert(objects[i]);
				this->SW->insert(objects[i]);
				this->SE->insert(objects[i]);
			}
			//once all the objects have been assigned to their child sectors, clear the parent sector's object vector
			this->objects.clear();
			//set the capacity of the parent to 0
			this->capacity = 0;
		}
		//recursively calls the parent child's sectors with the particle as a parameter to determin which sector the particle should be in
		if (this->NW->insert(particles))
		{
			NW->size++;
			return true;
		}
		if (this->NE->insert(particles))
		{
			NE->size++;
			return true;
		}
		if (this->SW->insert(particles))
		{
			SW->size++;
			return true;
		}
		if (this->SE->insert(particles))
		{
			SE->size++;
			return true;
		}
		else return false;
	}
 }
//creates the sectors 
void Quadtree::divide()
{
	// creates a new Quadtree of each four sectors and splits it up into a smaller sector
	this->NW = new Quadtree(x - (width / 4), y + (height / 4), width / 2, height / 2);
	this->NE = new Quadtree(x + (width / 4), y + (height / 4), width / 2, height / 2);
	this->SW = new Quadtree(x - (width / 4), y - (height / 4), width / 2, height / 2);
	this->SE = new Quadtree(x + (width / 4), y - (height / 4), width / 2, height / 2);
	this->NW->maxDepth = this->maxDepth + 1;
	this->NE->maxDepth = this->maxDepth + 1;
	this->SW->maxDepth = this->maxDepth + 1;
	this->SE->maxDepth = this->maxDepth + 1;
	//notifies that this sector has been divided
	this->divided = true;
}
//Returns a vector of vector particles 
std::vector<std::vector<Particle*>> Quadtree::getObjects(std::vector<std::vector<Particle*>> particleSectors)
{
	//checks if this sector has been divided
	if (this->divided == true)
	{
		//recursively calls this function to find all of the child sector objects vector which holds the particles
		particleSectors = NW->getObjects(particleSectors);
		particleSectors = NE->getObjects(particleSectors);
		particleSectors = SW->getObjects(particleSectors);
		particleSectors = SE->getObjects(particleSectors);
		//returns the vector of vector particles
		return particleSectors;
	}
	//if sector hasn't been divided
	else
	{
		// add the sector's vector of objects to the particleSectors vector
		particleSectors.push_back(this->objects);
		// return the vector of vector particles
		return particleSectors;
	}
}
std::vector<std::vector<Particle*>> Quadtree::getSectorObjects(std::vector<std::vector<Particle*>> particleSectors)
{
	particleSectors.push_back(this->objects);
	return particleSectors;
}
void Quadtree::show()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1);
	glBegin(GL_LINES); // red
	glVertex2f(this->x - (width/2), this->y);
	glVertex2f(this->x + (width / 2), this->y);
	glVertex2f(this->x, this->y - (height / 2));
	glVertex2f(this->x, this->y + (height / 2));
	glEnd();

	//recursively calls the show function for each child sector
	if (NW->NW != nullptr)
		this->NW->show();
	if (NE->NE != nullptr)
		this->NE->show();
	if (SW->SW != nullptr)
		this->SW->show();
	if (SE->SE != nullptr)
		this->SE->show();
}
//function to check which sector the particle belongs in
bool Quadtree::contains(Particle* object) {
	//checks if the particle belong in the current sector that's checking
	if (object->getPosition().x + object->getRadius() >= this->x - (this->width / 2) && object->getPosition().x - object->getRadius() <= this->x + (this->width / 2))
	{
		if (object->getPosition().y + object->getRadius() >= this->y - (this->height / 2) && object->getPosition().y - object->getRadius() <= this->y + (this->height / 2))
			return true;
	}

	return false;
}