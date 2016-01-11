#ifndef MATRIX33F_H
#define MATRIX33F_H

#include "vec3f.h"
#include <cmath>

typedef enum{ANY_T, TRANSLATION_T, ROTATION_T, SCALING_T} transformation_t;

class Matrix33f : public Array<3, Vec3f>
{
public:

    //Constructeurs

    Matrix33f();
    Matrix33f(transformation_t transformation, float arg1, float arg2=0);
    Matrix33f(const Matrix33f &other);

    //Opérateurs
    //Accès à l'élément de la ième ligne et jème colonne : Matrix33f[i][j] ou Matrix33f[i].<X(),Y(),Z()>
    //Accès à la ième ligne : Matrix33f[i]
    //Commence à 0

    Matrix33f operator*(const Matrix33f& matrix) const;
    Vec3f operator*(const Vec3f& vector) const;
    Vec2f operator*(const Vec2f& vector) const;

    //Opérations

    Matrix33f invert() const;

    void setId();

    void setTranslation(float tx, float ty);
    void setRotation(float rad);
    void setScaling(float vx, float vy);

    void addTranslation(float tx, float ty);
    void addRotation(float rad);
    void addScaling(float vx, float vy);

    void apply(Vec2f &vector) const;

    //Accesseurs//

    ///Permet de retracer les transformations effectuées (interface + bbox)
    float Rad() const;
    float Tx() const;
    float Ty() const;
    float Vx() const;
    float Vy() const;

    //Setters//

    void forceRad(float rad);
    void forceTx(float tx);
    void forceTy(float ty);
    void forceVx(float vx);
    void forceVy(float vy);


private:

    void _resetId();

    float m_tx;
    float m_ty;
    float m_rad;
    float m_vx;
    float m_vy;
};

#endif // MATRIX33F_H
