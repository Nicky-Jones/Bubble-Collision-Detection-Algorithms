/*
 * Interface file for the contact resolution system for particles.
 *
 */

#ifndef PCONTACTS_H
#define PCONTACTS_H

#include "particle.h"


    class ParticleContactResolver;

    /**
     * A Contact represents two objects in contact (in this case
     * ParticleContact representing two Particles). 
     */
    class ParticleContact
    {
        /**
         * The contact resolver object needs access into the contacts to
         * set and effect the contact.
         */
        friend ParticleContactResolver;

    public:
        /**
         * Holds the particles that are involved in the contact. The
         * second of these can be NULL, for contacts with the scenery.
         */
        Particle* particle[2];

        /**
         * Holds the normal restitution coefficient at the contact.
         */
        float restitution;

        /**
         * Holds the direction of the contact in world coordinates.
         */
        Vector2 contactNormal;

        /**
         * Holds the depth of penetration at the contact.
         */
        float penetration;

		/**
		 * Holds the amount the particels are moved by
		*/
		Vector2 particleMovement[2];


    protected:
        /**
         * Resolves this contact, for both velocity and interpenetration.
         */
        void resolve(float duration);
		void resolveInterpenetration(float duration);
        /**
         * Calculates the separating velocity at this contact.
         */
        float calculateSeparatingVelocity() const;

    private:
        /**
         * Handles the impulse calculations for this collision.
         */
        void resolveVelocity(float duration);

    };

    /**
     * The contact resolution routine for particle contacts. One
     * resolver instance can be shared for the whole simulation.
     */
    class ParticleContactResolver
    {
    protected:
        /**
         * Holds the number of iterations allowed.
         */
        unsigned iterations;

        /**
         * This is a performance tracking value - we keep a record
         * of the actual number of iterations used.
         */
        unsigned iterationsUsed;

    public:
        /**
         * Creates a new contact resolver.
         */
        ParticleContactResolver(unsigned iterations);

        /**
         * Sets the number of iterations that can be used.
         */
        void setIterations(unsigned iterations);

        /**
         * Resolves a set of particle contacts for both penetration
         * and velocity.
         *
        */
        void resolveContacts(ParticleContact *contactArray,
            unsigned numContacts,
            float duration);
    };

    /**
     * This is the basic polymorphic interface for contact generators
     * applying to particles.
     */
    class ParticleContactGenerator
    {
    public:
        /**
         * Fills the given contact structure with the generated
         * contact. 
         */
        virtual unsigned addContact(ParticleContact *contact,
                                    unsigned limit) const = 0;
    };

	

#endif // CONTACTS_H