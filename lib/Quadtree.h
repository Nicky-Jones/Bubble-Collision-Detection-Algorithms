#pragma once
#include <vector>
#include <particle.h>
#include <iostream>
using namespace std;

class Quadtree {
public:
	Quadtree(float x, float y, float width, float height, int level, int maxLevel);
	~Quadtree();
	void AddObject(Particle *object);
	void PreOrder(Quadtree *quad);
	int size;
private:
	float x;
	float y;
	float width;
	float height;
	int level;
	int maxLevel;
	vector<Particle*>objects;
	Quadtree *parent;
	Quadtree *NW;
	Quadtree *NE;
	Quadtree *SW;
	Quadtree *SE;
	bool contains(Quadtree *child, Particle *object);
};