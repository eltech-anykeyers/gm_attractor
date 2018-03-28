#ifndef VEC3_HPP
#define VEC3_HPP

template<typename T> class Vec3
{
private:
    T x;
    T y;
    T z;

public:
    Vec3() { x = y = z = 0; }

    Vec3(T xValue, T yValue, T zValue)
    {
        x = xValue;
        y = yValue;
        z = zValue;
    }

    void set(const T& xValue, const T& yValue, const T& zValue)
    {
        x = xValue;
        y = yValue;
        z = zValue;
    }

    T getX() const { return x; }
    T getY() const { return y; }
    T getZ() const { return z; }

    void setX(const T& xValue) { x = xValue; }
    void setY(const T& yValue) { y = yValue; }
    void setZ(const T& zValue) { z = zValue; }

    void zero() { x = y = z = 0; }

    void normalise()
    {
        // Calculate the magnitude of our vector.
        T magnitude = sqrt((x * x) + (y * y) + (z * z));

        /* As long as the magnitude isn't zero, divide each element
         * by the magnitude to get the normalised value between -1 and +1.
         */
        if (magnitude != 0)
        {
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
        }
    }

    /* Method to calculate and return the scalar dot product of two vectors
     * Note: The dot product of two vectors tell us things
     * about the angle between the vectors. That is, it tells us
     * if they are pointing in the same direction (i.e. are they parallel?
     * If so, the dot product will be 1), or if they're perpendicular
     * i.e. at 90 degrees to each other) the dot product will be 0,
     * or if they're pointing in opposite directions then
     * the dot product will be -1.
     */
    T dotProduct(const Vec3& vec) const
    {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    static T dotProduct(const Vec3& vec1, const Vec3& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }

    /* Method to calculate and return a vector which is the
     * cross product of two vectors.
     *
     * Note: The cross product is simply a vector which is perpendicular
     * to the plane formed by the first two vectors. Think of a desk like
     * the one your laptop or keyboard is sitting on. If you put one pencil
     * pointing directly away from you, and then another pencil pointing to the
     * right so they form a "L" shape, the vector perpendicular
     * to the plane made by these two pencils points directly upwards.
     *
     * Whether the vector is perpendicularly pointing "up" or "down" depends
     * on the "handedness" of the coordinate system that you're using.
     *
     * Further reading: http://en.wikipedia.org/wiki/Cross_product
     */
    static Vec3 crossProduct(const Vec3& vec1, const Vec3& vec2)
    {
        return Vec3(vec1.y * vec2.z - vec1.z * vec2.y,
                    vec1.z * vec2.x - vec1.x * vec2.z,
                    vec1.x * vec2.y - vec1.y * vec2.x);
    }

    void addX(T value) { x += value; }
    void addY(T value) { y += value; }
    void addZ(T value) { z += value; }

    static T getDistance(const Vec3& v1, const Vec3& v2)
    {
        T dx = v2.x - v1.x;
        T dy = v2.y - v1.y;
        T dz = v2.z - v1.z;

        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    Vec3 operator +(const Vec3& vector) const
    {
        return Vec3<T>(x + vector.x, y + vector.y, z + vector.z);
    }

    void operator +=(const Vec3& vector)
    {
        x += vector.x;
        y += vector.y;
        z += vector.z;
    }

    Vec3 operator -(const Vec3& vector) const
    {
        return Vec3<T>(x - vector.x, y - vector.y, z - vector.z);
    }

    void operator -=(const Vec3& vector)
    {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
    }

    Vec3 operator *(const Vec3& vector) const
    {
        return Vec3<T>(x * vector.x, y * vector.y, z * vector.z);
    }

    Vec3 operator *(const T& value) const
    {
        return Vec3<T>(x * value, y * value, z * value);
    }

    void operator *=(const T& value)
    {
        x *= value;
        y *= value;
        z *= value;
    }

    Vec3 operator /(const T& value) const
    {
        return Vec3<T>(x / value, y / value, z / value);
    }

    void operator /=(const T& value)
    {
        x /= value;
        y /= value;
        z /= value;
    }
};

#endif // VEC3_HPP
