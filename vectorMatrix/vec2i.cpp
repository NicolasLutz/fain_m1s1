#include "vec2i.h"

Vec2i::Vec2i() : Vector<2, int>()
{}

Vec2i::Vec2i(float x, float y) : Vector<2, int>()
{
    m_tab[0]=x;
    m_tab[1]=y;
}

Vec2i::Vec2i(const Vec2i& other) : Vector<2, int>(other)
{}

//Accesseurs

const int& Vec2i::X() const
{
    return m_tab[0];
}

const int& Vec2i::Y() const
{
    return m_tab[1];
}

//Not-const getters

int& Vec2i::X()
{
    return m_tab[0];
}

int& Vec2i::Y()
{
    return m_tab[1];
}

//Setters

void Vec2i::setX(int x)
{
    m_tab[0]=x;
}

void Vec2i::setY(int y)
{
    m_tab[1]=y;
}

Vec2i& Vec2i::addX(int x)
{
    m_tab[0]+=x;
    return (*this);
}

Vec2i& Vec2i::addY(int y)
{
    m_tab[1]+=y;
    return (*this);
}

//Opérateurs

int Vec2i::operator^(const Vec2i& other) const
{
    return (X()*other.Y())-(Y()*other.X());
}

Vec2i& Vec2i::operator=(const Vec2i& other)
{
    setX(other.X());
    setY(other.Y());
    return (*this);
}

//Opérations

bool Vec2i::borned(size_t width, size_t height)
{
    return ((unsigned int)X()) < width && ((unsigned int)Y()) < height;
}

Vec2i Vec2i::verticeOrientedDistance(const Vec2i& other) const
{
    return Vec2i(X()-other.X(), Y()-other.Y());
}

bool Vec2i::lowerThan(const Vec2i& v2) const
{
    if(X()>v2.X())
        return false;
    else if(X()==v2.X())
    {
        if(Y()>v2.Y())
            return false;
    }
    return true;
}

//Opérations de transformation//

void Vec2i::rotate90Left(const Vec2i& origin)
{
    Vec2i orientedDistance=verticeOrientedDistance(origin);
    //A 90 degrees rotation of a vertice with a distance of x,y from the origin is -y, x.
    int x=-orientedDistance.Y();
    int y=orientedDistance.X();

    setX(origin.X()+x);
    setY(origin.Y()+y);
}

void Vec2i::scale(const Vec2i& origin, float scaleX, float scaleY)
{
    Vec2i orientedDistance=verticeOrientedDistance(origin);
    int x=(float)(orientedDistance.X()*scaleX + 0.5f);
    int y=(float)(orientedDistance.Y()*scaleY + 0.5f);

    setX(origin.X()+x);
    setY(origin.Y()+y);
}
