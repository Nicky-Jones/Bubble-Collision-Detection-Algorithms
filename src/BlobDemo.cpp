/*
 * The Blob demo.
 *
 */
#include <gl/glut.h>
#include "app.h"
#include "coreMath.h"
#include "pcontacts.h"
#include "pworld.h"
#include <stdio.h>
#include <vector>
#include <cassert>
#include <math.h>
#include <iostream>
#include <time.h>
#include <list>
#include <ParticleCollision.h>
#include <quadAlgorithm.h>
#include <ctime>
#include <chrono>
#include <Quadtree.h>
#include <NodeTree.h>

#define BLOB_COUNT 10 // global variable for the number of blob particles to add to scene
#define PLATFORM_COUNT 6 // global variable for the number of platforms to add to the scene


bool enableDebugging = false; // global bool to track if the debugging is enabled or not

using namespace std;
using namespace std::chrono;


int initial_time = time(NULL), final_time, frame_count = 0;

const Vector2 Vector2::GRAVITY = Vector2(0, -9.81);

/**
 * Platforms are two dimensional: lines on which the
 * particles can rest. Platforms are also contact generators for the physics.
 */

class Platform : public ParticleContactGenerator
{
public:
	Vector2 start;
	Vector2 end;
	/**
	 * Holds a pointer to the particles we're checking for collisions with.
	 */
	Particle *particles;
	float restitution;
	virtual unsigned addContact(
		ParticleContact *contact,
		unsigned limit
	) const;
};

unsigned Platform::addContact(ParticleContact *contact,
	unsigned limit) const
{
	unsigned used = 0;

	for (unsigned i = 0; i < BLOB_COUNT; i++) {

		if (used >= limit) return used;

		// Check for penetration
		Vector2 toParticle = particles[i].getPosition() - start;
		Vector2 lineDirection = end - start;

		float projected = toParticle * lineDirection;
		float platformSqLength = lineDirection.squareMagnitude();
		float squareRadius = particles[i].getRadius()*particles[i].getRadius();;

		if (projected <= 0)
		{

			// The blob is nearest to the start point
			if (toParticle.squareMagnitude() < squareRadius)
			{
				// We have a collision
				contact->contactNormal = toParticle.unit();
				contact->restitution = restitution;
				contact->particle[0] = particles + i;
				contact->particle[1] = 0;
				contact->penetration = particles[i].getRadius() - toParticle.magnitude();
				used++;
				contact++;
			}

		}
		else if (projected >= platformSqLength)
		{
			// The blob is nearest to the end point
			toParticle = particles[i].getPosition() - end;
			if (toParticle.squareMagnitude() < squareRadius)
			{
				// We have a collision
				contact->contactNormal = toParticle.unit();
				contact->restitution = restitution;
				contact->particle[0] = particles + i;
				contact->particle[1] = 0;
				contact->penetration = particles[i].getRadius() - toParticle.magnitude();
				used++;
				contact++;
			}
		}
		else
		{
			// the blob is nearest to the middle.
			float distanceToPlatform = toParticle.squareMagnitude() - projected * projected / platformSqLength;
			if (distanceToPlatform < squareRadius)
			{
				// We have a collision
				Vector2 closestPoint = start + lineDirection * (projected / platformSqLength);
				contact->contactNormal = (particles[i].getPosition() - closestPoint).unit();
				contact->restitution = restitution;
				contact->particle[0] = particles + i;
				contact->particle[1] = 0;
				contact->penetration = particles[i].getRadius() - sqrt(distanceToPlatform);
				used++;
				contact++;
			}
		}
	}
	return used;
}

class BlobDemo : public Application
{
	Particle* blobs;
	Platform* platforms;
	ParticleCollision *blobCollision;
	ParticleWorld world;
	quadAlgorithm *quadA;
	Quadtree *qTree;
	NodeTree *QuadTree;

	enum collisionAlgorithmSwitcher { BRUTE_FORCE, QUAD, QUAD_TREE}; // an enumerator of each algorithm state the program can be switched too
	collisionAlgorithmSwitcher currentAlgorithm = collisionAlgorithmSwitcher::BRUTE_FORCE; // creates an enumerator and sets the default algorithm to load as BRUTE_FORCE
	void switchCurrentAlgorithm(); // function to switch the algorithms
	void startingCurrentAlgorithm(collisionAlgorithmSwitcher algorithm); // function to set the starting algorithm

public:
	/** Creates a new demo object. */
	BlobDemo();
	virtual ~BlobDemo();
	void collisionQuadDetection(); // function to determin which quad section each particle should be in
	void collisionBruteForceDetection(); // function to start the brute force algorithm
	/** Returns the window title for the demo. */
	virtual const char* getTitle();
	/** Display the particles. */
	virtual void display();
	/** Update the particle positions. */
	virtual void update();
	int collisionSwitch = 0;
	void resetAlgorthm();
	virtual void keyboard(unsigned char key, int x, int y); // virtual function to receive user keyboard input
};
//keyboard function to detect inputs from the user
void BlobDemo::keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		// if the user pressed the key "f" execute the following lines of code
	case ('f'):
		switchCurrentAlgorithm();
		break;
		// if user pressed the key "d" execute the following lines of code
	case ('d'):
		if (enableDebugging != true)
		{
			enableDebugging = true;
		}
		else
			enableDebugging = false;
	}
}

// Method definitions
BlobDemo::BlobDemo() :world(PLATFORM_COUNT + BLOB_COUNT, BLOB_COUNT)
{
	width = 400; height = 400;
	nRange = 100.0;
	float margin = 0.95;

	blobs = new Particle[BLOB_COUNT];

	platforms = new Platform[PLATFORM_COUNT];

	blobCollision = new ParticleCollision;

	quadA = new quadAlgorithm;

	// creates the platforms & walls
	platforms[0].start = Vector2(-50.0, 30.0);
	platforms[0].end = Vector2(0.0, 0.0);
	platforms[0].restitution = 1.0f;
				
	platforms[1].start = Vector2(-nRange * margin, -nRange * margin);
	platforms[1].end = Vector2(nRange * margin, -nRange * margin);
	platforms[1].restitution = 1;
				
	platforms[2].start = Vector2(-nRange * margin, nRange * margin);
	platforms[2].end = Vector2(nRange * margin, nRange * margin);
	platforms[2].restitution = 1;
				
	platforms[3].start = Vector2(-nRange * margin, -nRange * margin);
	platforms[3].end = Vector2(-nRange * margin, nRange * margin);
	platforms[3].restitution = 1;
				
	platforms[4].start = Vector2(nRange * margin, -nRange * margin);
	platforms[4].end = Vector2(nRange * margin, nRange * margin);
	platforms[4].restitution = 1;
				
	platforms[5].start = Vector2(50.0, 30.0);
	platforms[5].end = Vector2(0.0, 0.0);
	platforms[5].restitution = 1.0f;

	srand(time(NULL));
	// creates the blob particles
	for (unsigned i = 0; i < BLOB_COUNT; i++)
	{
		blobs[i].setPosition((float)(rand() %180) - 90.0f, (float)(rand() % 180) - 90.0f);
		blobs[i].setRadius(5);
		blobs[i].setVelocity(0, 0);
		blobs[i].setDamping(1);
		blobs[i].setAcceleration(Vector2::GRAVITY * 20.0f);
		blobs[i].setMass(30.0f);
		blobs[i].clearAccumulator();

		world.getParticles().push_back(blobs + i);
	}
	
	blobCollision->blob_count = BLOB_COUNT;
	for (int i = 0; i < PLATFORM_COUNT; i++)
	{
		platforms[i].particles = blobs;
		world.getContactGenerators().push_back(platforms + i);
	}
	// starts the initial algoirthm has brute force
	startingCurrentAlgorithm(BRUTE_FORCE);
}


BlobDemo::~BlobDemo()
{
	delete blobs;
}
// A function to make a request to switch the algorithm
void BlobDemo::switchCurrentAlgorithm() {

	if (currentAlgorithm == BRUTE_FORCE)
		currentAlgorithm = collisionAlgorithmSwitcher::QUAD;
	else if (currentAlgorithm == QUAD)
		currentAlgorithm = collisionAlgorithmSwitcher::QUAD_TREE;
	else if(currentAlgorithm == QUAD_TREE)
		currentAlgorithm = collisionAlgorithmSwitcher::BRUTE_FORCE;

	startingCurrentAlgorithm(currentAlgorithm);
}
// a function to make the actual algoirthms change 
void BlobDemo::startingCurrentAlgorithm(collisionAlgorithmSwitcher algorithm) {
	//switch algorithm to brute force
	if (algorithm == BRUTE_FORCE)
	{
		cout << "Switching to " << "brute force algorithm" << "\n";
		resetAlgorthm();
		collisionBruteForceDetection();
	}
	// switch algorithm to QUAD
	else if (algorithm == QUAD)
	{
		cout << "Switching to " << "Quad algorithm" << "\n";
		resetAlgorthm();
		world.getContactGenerators().push_back(quadA);
		collisionQuadDetection();
	}
	// switch algorithm to dynamic quad tree's
	else if (algorithm == QUAD_TREE)
	{
		cout << "Switching to " << "Node Tree Algorithm" << "\n";
		resetAlgorthm();
		QuadTree = new NodeTree(blobs, Vector2(nRange *2.0, nRange * 2.0), BLOB_COUNT);
		world.getContactGenerators().push_back(QuadTree);
		QuadTree->update();
	}
	else
	{
		cout << "error";
	}
}
// a function for the Quad algorithm
void BlobDemo::collisionQuadDetection()
{
	// clears the quad list
	quadA->clearLists();
	// runs a loop of all the blob's to determin which quad sector they are in
	for (unsigned i = 0; i < BLOB_COUNT; i++)
	{
		float radius = blobs[i].getRadius();
		// depending on the sector the blobs are in, put the blobs in an array of that particular array 
		// & if the blob is big enough to fit in to two sectors at once, add it to both
		if (blobs[i].getPosition().x + radius > 0)
		{
			if (blobs[i].getPosition().y + radius > 0)
			{
				quadA->particles[0]->push_back(&blobs[i]);
			}
			if (blobs[i].getPosition().y - radius < 0)
			{
				quadA->particles[1]->push_back(&blobs[i]);
			}
		}
		if (blobs[i].getPosition().x - radius < 0)
		{
			if (blobs[i].getPosition().y + radius > 0)
			{
				quadA->particles[3]->push_back(&blobs[i]);
			}
			if (blobs[i].getPosition().y - radius < 0)
			{
				quadA->particles[2]->push_back(&blobs[i]);
			}
		}
	}
}
// a function to set up the brute force algorithm
void BlobDemo::collisionBruteForceDetection()
{
	blobCollision->collisionChecker = false;
	// add the blob particle array to the bobCollision bruteParticles vector
	blobCollision->bruteParticles = blobs;
	// add blobColision to the contract generators so addContact can be called in that class
	world.getContactGenerators().push_back(blobCollision);
}

void BlobDemo::display()
{
	Application::display();
	glBegin(GL_LINES);
	glColor3f(0, 1, 1);
	for (int i = 0; i < PLATFORM_COUNT; i++)
	{
		const Vector2 &p0 = platforms[i].start;
		const Vector2 &p1 = platforms[i].end;
		glVertex2f(p0.x, p0.y);
		glVertex2f(p1.x, p1.y);
	}
	glEnd();
	for (int i = 0; i < BLOB_COUNT; i++)
	{
		glColor3f((i % 2) ? 0 : 1, (i % 3) ? 0 : 1, (i % 4) ? 1 : 0);
		const Vector2 &p = blobs[i].getPosition();
		glPushMatrix();
		glTranslatef(p.x, p.y, 0);
		glutSolidSphere(blobs[i].getRadius(), 12, 12);
		glPopMatrix();
	}
	// displays the debug for the current active algorithm
	if (enableDebugging == true) {
		if (currentAlgorithm == BRUTE_FORCE)
		{
			blobCollision->bruteForceDraw();
		}
		else if (currentAlgorithm == QUAD)
		{
			quadA->quadDraw();
		}
		else if (currentAlgorithm == QUAD_TREE)
		{
			QuadTree->drawTree();
		}
		else
		{
			cout << "Error";
		}
	}

	glutSwapBuffers();

}
// a function to reset the contact generator 
void BlobDemo::resetAlgorthm()
{
	// clears the contactgenertator
	world.getContactGenerators().clear();
	// Once the contact generator is cleared for the algorithm switch, add the platforms back for collision purposes
	for (int i = 0; i < PLATFORM_COUNT; i++)
	{
		world.getContactGenerators().push_back(platforms + i);
	}
}
void BlobDemo::update()
{
	// Recenter the axes
	float duration = timeinterval / 1000;
	// Run the simulation
	world.runPhysics(duration);
	// if the current algorithm is Quad execute the following code
	if (currentAlgorithm == QUAD)
	{
		// calls the quad detection function to update where each particle is
		collisionQuadDetection();
	}
	if (currentAlgorithm == QUAD_TREE)
	{
		// calls the quad tree function to update where each particle is
		QuadTree->update();
	}
	Application::update();
}

const char* BlobDemo::getTitle()
{
	return "Blob Demo";
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
	return new BlobDemo();
}
