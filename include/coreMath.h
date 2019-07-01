#include <math.h>

/**
 * @file
 *
 * The core contains utility functions, helpers and a basic set of
 * mathematical types.
 */
#ifndef CORE_MATH
#define CORE_MATH


class Vector2
    {
    public:
         /** Holds the value along the x axis. */
        float x;

        /** Holds the value along the y axis. */
        float y;


    public:
        /** The default constructor creates a zero vector. */
        Vector2() : x(0), y(0) {}

        /**
         * The explicit constructor creates a vector with the given
         * components.
         */
        Vector2(const float x, const float y)
            : x(x), y(y) {}


		const static Vector2 GRAVITY;
		const static Vector2 UP;

        float operator[](unsigned i) const
        {
            if (i == 0) return x;
			return y;
        }

        float& operator[](unsigned i)
        {
            if (i == 0) return x;
            return y;
        }

        /** Adds the given vector to this. */
        void operator+=(const Vector2& v)
        {
            x += v.x;
            y += v.y;
        }

        /**
         * Returns the value of the given vector added to this.
         */
        Vector2 operator+(const Vector2& v) const
        {
            return Vector2(x+v.x, y+v.y);
        }

        /** Subtracts the given vector from this. */
        void operator-=(const Vector2& v)
        {
            x -= v.x;
            y -= v.y;
        }

        /**
         * Returns the value of the given vector subtracted from this.
         */
        Vector2 operator-(const Vector2& v) const
        {
            return Vector2(x-v.x, y-v.y);
        }

        /** Multiplies this vector by the given scalar. */
        void operator*=(const float value)
        {
            x *= value;
            y *= value;
        }

        /** Returns a copy of this vector scaled the given value. */
        Vector2 operator*(const float value) const
        {
            return Vector2(x*value, y*value);
        }

        /**
         * Calculates and returns a component-wise product of this
         * vector with the given vector.
         */
        Vector2 componentProduct(const Vector2 &vector) const
        {
            return Vector2(x * vector.x, y * vector.y);
        }

        /**
         * Performs a component-wise product with the given vector and
         * sets this vector to its result.
         */
        void componentProductUpdate(const Vector2 &vector)
        {
            x *= vector.x;
            y *= vector.y;
        }


        /**
         * Calculates and returns the scalar product of this vector
         * with the given vector.
         */
        float scalarProduct(const Vector2 &vector) const
        {
            return x*vector.x + y*vector.y;
        }

        /**
         * Calculates and returns the scalar product of this vector
         * with the given vector.
         */
        float operator *(const Vector2 &vector) const
        {
            return x*vector.x + y*vector.y;
        }

        /**
         * Adds the given vector to this, scaled by the given amount.
         */
        void addScaledVector(const Vector2& vector, float scale)
        {
            x += vector.x * scale;
            y += vector.y * scale;
        }

        /** Gets the magnitude of this vector. */
        float magnitude() const
        {
            return sqrt(x*x+y*y);
        }

        /** Gets the squared magnitude of this vector. */
        float squareMagnitude() const
        {
            return x*x+y*y;
        }

        /** Limits the size of the vector to the given maximum. */
        void trim(float size)
        {
            if (squareMagnitude() > size*size)
            {
                normalise();
                x *= size;
                y *= size;
            }
        }

        /** Turns a non-zero vector into a vector of unit length. */
        void normalise()
        {
            float l = magnitude();
            if (l > 0)
            {
                (*this) *= ((float)1)/l;
            }
        }

        /** Returns the normalised version of a vector. */
        Vector2 unit() const
        {
            Vector2 result = *this;
            result.normalise();
            return result;
        }

        /** Checks if the two vectors have identical components. */
        bool operator==(const Vector2& other) const
        {
            return x == other.x &&
                y == other.y;
        }

        /** Checks if the two vectors have non-identical components. */
        bool operator!=(const Vector2& other) const
        {
            return !(*this == other);
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a < b) does not imply (b >= a).
         */
        bool operator<(const Vector2& other) const
        {
            return x < other.x && y < other.y;
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a < b) does not imply (b >= a).
         */
        bool operator>(const Vector2& other) const
        {
            return x > other.x && y > other.y;
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a <= b) does not imply (b > a).
         */
        bool operator<=(const Vector2& other) const
        {
            return x <= other.x && y <= other.y;
        }

        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a <= b) does not imply (b > a).
         */
        bool operator>=(const Vector2& other) const
        {
            return x >= other.x && y >= other.y;
        }

        /** Zero all the components of the vector. */
        void clear()
        {
            x = y = 0;
        }

        /** Flips all the components of the vector. */
        void invert()
        {
            x = -x;
            y = -y;
        }

    };

    
 

   



	#endif // CORE_H