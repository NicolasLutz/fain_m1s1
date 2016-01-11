#include "vec3f.h"

//====================================================================================================================================
//Constructeurs
Vec3f::Vec3f() : Vector<3, float>()
{}

Vec3f::Vec3f(float x, float y, float z) : Vector<3, float>()
{
    m_tab[0]=x;
    m_tab[1]=y;
    m_tab[2]=z;
}

Vec3f::Vec3f(const Vec3f &other) : Vector<3, float>(other)
{}

Vec3f::Vec3f(const Vec2f &vector2, float z) : Vector<3, float>()
{
    m_tab[0]=vector2.X();
    m_tab[1]=vector2.Y();
    m_tab[2]=z;
}

//====================================================================================================================================
//Opérateurs

float Vec3f::operator*(const Vec3f& other) const
{
    return X()*other.X()+Y()*other.Y()+Z()*other.Z();
}

Vec3f Vec3f::operator^(const Vec3f& other) const
{
    Vec3f res(  Y()*other.Z()-Z()*other.Y()
               ,Z()*other.X()-X()*other.Z()
               ,X()*other.Y()-Y()*other.X());
    return res;
}

//====================================================================================================================================

Vec3f Vec3f::operator*(float fValue) const
{
    Vec3f v;
    v.setX(X()*fValue);
    v.setY(Y()*fValue);
    v.setZ(Z()*fValue);
    return v;
}


//Accesseurs

const float& Vec3f::X() const
{
    return m_tab[0];
}

const float& Vec3f::Y() const
{
    return m_tab[1];
}

const float& Vec3f::Z() const
{
    return m_tab[2];
}

Vec2f Vec3f::XY() const
{
    return Vec2f(X(),Y());
}

//====================================================================================================================================
//Setteurs

void Vec3f::setX(float x)
{
    m_tab[0]=x;
}

void Vec3f::setY(float y)
{
    m_tab[1]=y;
}

void Vec3f::setZ(float z)
{
    m_tab[2]=z;
}

//====================================================================================================================================
//Outside

Vec3f operator^(const Vec2f& v_this, const Vec2f& other)
{
    Vec3f res(0,0,v_this.X()*other.Y()-v_this.Y()*other.X());
    return res;
}
