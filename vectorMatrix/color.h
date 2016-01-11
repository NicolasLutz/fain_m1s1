#ifndef COLOR_H
#define COLOR_H

#include "vec3f.h"
#include <cstdlib>

class Color : public Vec3f
{
public:
    Color();
    Color(const Color& other);
    Color(float r, float g=0, float b=0, float alpha=1);

    float Rf() const;
    float Gf() const;
    float Bf() const;

    float Alpha() const;
    void setAlpha(float alpha, bool forceClamp=false);

    int Ri(bool forceClamp=false) const;
    int Gi(bool forceClamp=false) const;
    int Bi(bool forceClamp=false) const;

    unsigned int Rui(bool forceClamp=false) const;
    unsigned int Gui(bool forceClamp=false) const;
    unsigned int Bui(bool forceClamp=false) const;

    unsigned char Ruc(bool forceClamp=false) const;
    unsigned char Guc(bool forceClamp=false) const;
    unsigned char Buc(bool forceClamp=false) const;

    //I can't believe I needed all of them

    void setRf(float r, bool check=false);
    void setGf(float g, bool check=false);
    void setBf(float b, bool check=false);

    void setRi(int r, bool check=false);
    void setGi(int g, bool check=false);
    void setBi(int b, bool check=false);

    void setRuc(unsigned char r);
    void setGuc(unsigned char g);
    void setBuc(unsigned char b);

    void setRand();

    bool operator==(const Color& other) const;
    Color& operator=(const Color& other);

    static Color Black;
    static Color White;
    static Color Red;
    static Color Green;
    static Color Blue;
    static Color Yellow;
    static Color Cyan;
    static Color Magenta;
protected:

    float m_alpha;
};

#endif // COLOR_H
