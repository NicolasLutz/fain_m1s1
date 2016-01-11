#include "vec2f.h"

//====================================================================================================================================
//Constructeurs
Vec2f::Vec2f() : Vector<2, float>()
{}

Vec2f::Vec2f(float x, float y) : Vector<2, float>()
{
    m_tab[0]=x;
    m_tab[1]=y;
}

Vec2f::Vec2f(const Vec2f& other) : Vector<2, float>(other)
{}

Vec2f::Vec2f(const Vec2i& other) : Vector<2, float>()
{
    m_tab[0]=(float)other.X();
    m_tab[1]=(float)other.Y();
}

//====================================================================================================================================
//Opérateurs

float Vec2f::operator*(const Vec2f& other) const
{
    return X()*other.X()+Y()*other.Y();
}

Vec2f Vec2f::operator*(float fValue) const
{
    Vec2f v;
    v.setX(X()*fValue);
    v.setY(Y()*fValue);
    return v;
}

//====================================================================================================================================
//Accesseurs

const float& Vec2f::X() const
{
    return m_tab[0];
}

//====================================================================================================================================

const float& Vec2f::Y() const
{
    return m_tab[1];
}

//====================================================================================================================================
//Accesseurs

void Vec2f::setX(float x)
{
    m_tab[0]=x;
}

void Vec2f::setY(float y)
{
    m_tab[1]=y;
}
