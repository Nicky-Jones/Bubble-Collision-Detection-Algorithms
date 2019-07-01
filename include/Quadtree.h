#pragma once
#include <vector>
#include <particle.h>
#include <iostream>
#include <gl/glut.h>
using namespace std;

class Quadtree {
public:
	Quadtree(float x, float y, float width, float height); // Quadtree's constructor
	~Quadtree(); // Quadtree's deconstructor
	int size = 0;// size variable to update per particle object
	bool insert(Particle* particles); // function to insert the particles into the quad tree
	std::vector<std::vector<Particle*>> getObjects(std::vector<std::vector<Particle*>>); // function to return a vector of vector particles
	std::vector<std::vector<Particle*>> getSectorObjects(std::vector<std::vector<Particle*>>);
	void show(); // function to draw the debug lines
	
private:
	float x;
	float y;
	float width;
	float height;
	int minDepth;
	int maxDepth = 0;
	int capacity = 3;
	bool divided = false; // check to see if this Quadtree has been divided or not
	void divide(); // function to divide the Quadtree
	vector<Particle*>objects; // vector of particles
	Quadtree *NW = nullptr; // Northwest sector of a Quadtree
	Quadtree *NE = nullptr; // Northeast sector of a Quadtree
	Quadtree *SW = nullptr; // Southwest sector of a Quadtree
	Quadtree *SE = nullptr; // Southeast sector of a Quadtree
	bool contains(Particle* object); // function to check if a particle belongs in this current sector
};
