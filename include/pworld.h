/*
 *
 */
#ifndef PWORLD_H
#define PWORLD_H

#include <vector> 
#include "pcontacts.h"


    class ParticleWorld
    {
    public:
        typedef std::vector<Particle*> Particles;
        typedef std::vector<ParticleContactGenerator*> ContactGenerators;

    protected:
        /**
         * Holds the particles
         */
        Particles particles;

        /**
         * True if the world should calculate the number of iterations
         * to give the contact resolver at each frame.
         */
        bool calculateIterations;

        /**
         * Holds the resolver for contacts.
         */
        ParticleContactResolver resolver;

        /**
         * Contact generators.
         */
        ContactGenerators contactGenerators;

        /**
         * Holds the list of contacts.
         */
        ParticleContact *contacts;

        /**
         * Holds the maximum number of contacts allowed (i.e. the
         * size of the contacts array).
         */
        unsigned maxContacts;

    public:

        /**
         * Creates a new particle simulator that can handle up to the
         * given number of contacts per frame. 
         */
        ParticleWorld(unsigned maxContacts, unsigned iterations=0);

        /**
         * Deletes the simulator.
         */
        ~ParticleWorld();

        /**
         * Calls each of the registered contact generators to report
         * their contacts. Returns the number of generated contacts.
         */
        unsigned generateContacts();

        /**
         * Integrates all the particles in this world forward in time
         * by the given duration.
         */
        void integrate(float duration);

        /**
         * Processes all the physics for the particle world.
         */
        void runPhysics(float duration);
		
        /**
         *  Returns the list of particles.
         */
        Particles& getParticles();

        /**
         * Returns the list of contact generators.
         */
        ContactGenerators& getContactGenerators();

    };




#endif // PWORLD_H
