#ifndef VEC2F_H
#define VEC2F_H

#include "vec2i.h"

class Vec2f : public Vector<2u, float>
{
public:

    //Constructeurs
    Vec2f();

    Vec2f(float x, float y=0);

    Vec2f(const Vec2f& other);

    Vec2f(const Vec2i& other); //is that a design flaw ? I dunno...

    //Opérateurs

    float operator*(const Vec2f& other) const;

    Vec2f operator*(float fValue) const;

    //Accesseurs

    const float &X() const;
    const float &Y() const;

    //Setters

    void setX(float x);
    void setY(float y);
};

//------------------------------------------------
//------------------------------------------------

#endif // VEC2F_H
