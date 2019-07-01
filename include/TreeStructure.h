#pragma once
#include <particle.h>
#include <list>
// 
// A Class for the Tree architect
//
class TreeStructure
{
private:
	Vector2 size; // size of the node in the scene
	Vector2 position; // the position in the scene the node is current at

	TreeStructure* parent = nullptr; // if node has a parent

	bool hasChildnodes = false; // Checks for children in node


public:
	TreeStructure *childNodes[4]; // creates child nodes
	std::list<Particle*> particles; // list of particles that are inside each node tree

	TreeStructure(Vector2 poisition, Vector2 size, TreeStructure* parent); // Tree node constructor

	//checks which node the particle should be in
	void checkParticle(Particle* particle, int numOfParticle);
	// moves the particle to the node depending on location of particle
	std::list<Particle*> moveParticles(Particle* particle, int numOfParticles);
	bool barrierChecking(Particle* particle);

	TreeStructure* createChildNodes(); // creates 4 child nodes
};