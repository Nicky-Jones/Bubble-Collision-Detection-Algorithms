#pragma once
#include <pcontacts.h>
#include <list>
#include <iostream>
#include <vector>
#include <gl/glut.h>

using namespace std;

class ParticleCollision : public ParticleContactGenerator
{
public:
	// holds a pointer to the particles we're checking for collisions with brute force method.
	Particle *bruteParticles;
	vector<Particle*> *particles[4]; // vector list of particles
	int blob_count; // integer value holding the number of blob particles in the scene
	bool collisionChecker = true; // bool to check if it should do the collision check or not
	ParticleCollision(); // particleCollision's constructor function
	//creates a virtual function called addContact
	virtual unsigned addContact(
		ParticleContact *contact,
		unsigned limit
	) const;
	// bool checks if collison has happened
	bool checkCollision(Particle, Particle) const;
	void bruteForceDraw(); // draws the debug lines for brute force algorithm
	void clearLists(); // clears the list
	~ParticleCollision(); // deconstructor
};

