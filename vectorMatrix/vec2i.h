#ifndef VEC2I_H
#define VEC2I_H

#include "vector.hpp"

class Vec2i : public Vector<2u, int>
{
public:

    //Constructeurs
    Vec2i();

    Vec2i(float x, float y=0);

    Vec2i(const Vec2i& other);

    //Accesseurs

    const int &X() const;
    const int &Y() const;

    int &X();
    int &Y();

    //Setters

    void setX(int x);
    void setY(int y);

    Vec2i& addX(int x);
    Vec2i& addY(int y);

    //Opérateurs

    int operator^(const Vec2i& other) const;
    Vec2i& operator=(const Vec2i& other);

    //Opérations

    bool borned(size_t width, size_t height);
    Vec2i verticeOrientedDistance(const Vec2i& other) const;

    ///Made for the sole purpose of the project; TODO: add a setF function in vector applying a functor
    bool lowerThan(const Vec2i& v2) const;

    //Opérations de transformation

    void rotate90Left(const Vec2i& origin);
    void scale(const Vec2i& origin, float scaleX, float scaleY);
};

#endif // VEC2I_H
