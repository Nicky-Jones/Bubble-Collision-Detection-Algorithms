#pragma once
#include <pcontacts.h>
#include <list>
#include <vector>
#include <gl/glut.h>
#include <iostream>
using namespace std;
class quadAlgorithm : public ParticleContactGenerator
{
private:
	bool checkCollision(Particle, Particle) const;

public:
	vector<Particle*> *particles[4]; // vector of type particles 
	quadAlgorithm(); // constructor
	virtual unsigned addContact(
		ParticleContact *contact,
		unsigned limit
	) const;
	unsigned quadCollision(ParticleContact *contact,
		unsigned limit, unsigned used, unsigned particlesChecked
	) const;
	int pChecked = 0;
	void clearLists(); // function to clear the list
	void quadDraw(); // function to draw the debug lines
	~quadAlgorithm(); // deconstuctor
};
