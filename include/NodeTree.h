#pragma once
#include <particle.h>
#include <list>
#include <vector>
#include <pcontacts.h>
#include <Quadtree.h>

class NodeTree : public ParticleContactGenerator
{
private:
	Vector2 sizeOfScreen; // Size of the screen
	int numOfParticles; // integer to hold the number of particles in a scene
	Quadtree* rootNode = nullptr; // Root node of quad tree
	bool checkCollision(Particle*, Particle*) const; // checks if particles are touching


public:
	NodeTree(Particle* particle, Vector2 size, int numOfP); // constructor
	~NodeTree(); // deconstructor
	void createTree(); // function to create a tree
	Particle* particles; // particle array
	void update(); // update function to update the quadtree
	void drawTree(); // function to call the quad tree's debug function
	virtual unsigned addContact(
		ParticleContact *contact,
		unsigned limit
	) const;
};