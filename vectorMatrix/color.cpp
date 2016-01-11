#include "color.h"

Color Color::Black(0.0f, 0.0f, 0.0f);
Color Color::White(1.0f, 1.0f, 1.0f);

Color Color::Red(1.0f, 0.0f, 0.0f);
Color Color::Green(0.0f, 1.0f, 0.0f);
Color Color::Blue(0.0f, 0.0f, 1.0f);

Color Color::Yellow(1.0f, 1.0f, 0.0f);
Color Color::Cyan(0.0f, 1.0f, 1.0f);
Color Color::Magenta(1.0f, 0.0f, 1.0f);

Color::Color():
    Vec3f(), m_alpha(1)
{}

Color::Color(const Color& other):
    Vec3f(other), m_alpha(other.Alpha())
{}

Color::Color(float r, float g, float b, float alpha):
    Vec3f(r,g,b), m_alpha(alpha)
{}

float Color::Rf() const
{
    return m_tab[0];
}

float Color::Gf() const
{
    return m_tab[1];
}

float Color::Bf() const
{
    return m_tab[2];
}

float Color::Alpha() const
{
    return m_alpha;
}

void Color::setAlpha(float alpha, bool forceClamp)
{
    m_alpha=!forceClamp ? alpha : alpha<0 ? 0 : alpha>1.0f ? 1.0f : alpha;
}

int Color::Ri(bool forceClamp) const
{
    return !forceClamp ? m_tab[0]*255 : m_tab[0]<=0 ? 0 : m_tab[0]>=1.0f ? 255 : m_tab[0]*255;
}

int Color::Gi(bool forceClamp) const
{
    return !forceClamp ? m_tab[1]*255 : m_tab[1]<=0 ? 0 : m_tab[1]>=1.0f ? 255 : m_tab[1]*255;
}

int Color::Bi(bool forceClamp) const
{
    return !forceClamp ? m_tab[2]*255 : m_tab[2]<=0 ? 0 : m_tab[2]>=1.0f ? 255 : m_tab[2]*255;
}

unsigned int Color::Rui(bool forceClamp) const
{
    return !forceClamp ? m_tab[0]*255 : m_tab[0]<=0 ? 0 : m_tab[0]>=1.0f ? 255 : m_tab[0]*255;
}
unsigned int Color::Gui(bool forceClamp) const
{
    return !forceClamp ? m_tab[1]*255 : m_tab[1]<=0 ? 0 : m_tab[1]>=1.0f ? 255 : m_tab[1]*255;
}
unsigned int Color::Bui(bool forceClamp) const
{
    return !forceClamp ? m_tab[2]*255 : m_tab[2]<=0 ? 0 : m_tab[2]>=1.0f ? 255 : m_tab[2]*255;
}

unsigned char Color::Ruc(bool forceClamp) const
{
    return !forceClamp ? m_tab[0]*255 : m_tab[0]<=0 ? 0 : m_tab[0]>=1.0f ? 255 : m_tab[0]*255;
}

unsigned char Color::Guc(bool forceClamp) const
{
    return !forceClamp ? m_tab[1]*255 : m_tab[1]<=0 ? 0 : m_tab[1]>=1.0f ? 255 : m_tab[1]*255;
}

unsigned char Color::Buc(bool forceClamp) const
{
    return !forceClamp ? m_tab[2]*255 : m_tab[2]<=0 ? 0 : m_tab[2]>=1.0f ? 255 : m_tab[2]*255;
}

void Color::setRf(float r, bool check)
{
    if(check)
    {
        m_tab[0]=(r < 0 ? 0 : (r > 1.0f ? 1.0f : r));
    }
    else
        m_tab[0]=r;
}

void Color::setGf(float g, bool check)
{
    if(check)
    {
        m_tab[1]=(g < 0 ? 0 : (g > 1.0f ? 1.0f : g));
    }
    else
        m_tab[1]=g;
}

void Color::setBf(float b, bool check)
{
    if(check)
    {
        m_tab[2]=(b < 0 ? 0 : (b > 1.0f ? 1.0f : b));
    }
    else
        m_tab[2]=b;
}

void Color::setRi(int r, bool check)
{
    if(check)
    {
        m_tab[0]=(r < 0 ? 0 : (r > 255 ? 1.0f : r/255.0f));
    }
    else
        m_tab[0]=(float)r/255.0f;
}

void Color::setGi(int g, bool check)
{
    if(check)
    {
        m_tab[1]=(g < 0 ? 0 : (g > 255 ? 1.0f : g/255.0f));
    }
    else
        m_tab[1]=(float)g/255.0f;
}

void Color::setBi(int b, bool check)
{
    if(check)
    {
        m_tab[2]=(b < 0 ? 0 : (b > 255 ? 1.0f : b/255.0f));
    }
    else
        m_tab[2]=(float)b/255.0f;
}

void Color::setRuc(unsigned char r)
{
    m_tab[0]=(float)r/255.0f;
}

void Color::setGuc(unsigned char g)
{
    m_tab[1]=(float)g/255.0f;
}

void Color::setBuc(unsigned char b)
{
    m_tab[2]=(float)b/255.0f;
}


void Color::setRand()
{
    setRf(rand()/(float)RAND_MAX);
    setGf(rand()/(float)RAND_MAX);
    setBf(rand()/(float)RAND_MAX);
}

bool Color::operator==(const Color& other) const
{
    return (Rf()==other.Rf() && Gf()==other.Gf() && Bf()==other.Bf());
}

Color& Color::operator=(const Color& other)
{
    setRf(other.Rf());
    setGf(other.Gf());
    setBf(other.Bf());
    setAlpha(other.Alpha());
    return (*this);
}
