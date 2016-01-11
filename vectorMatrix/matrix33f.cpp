#include "matrix33f.h"
//UNUSUED

#define _e_ (0.01f)

//====================================================================================================================================
//Constructeurs

Matrix33f::Matrix33f() :
    Array<3, Vec3f>(), m_tx(0), m_ty(0), m_rad(0), m_vx(1), m_vy(1)
{
    _resetId();
}

Matrix33f::Matrix33f(transformation_t transformation, float arg1, float arg2) :
    Array<3, Vec3f>(), m_tx(0), m_ty(0), m_rad(0), m_vx(1), m_vy(1)
{
    switch(transformation)
    {
        case TRANSLATION_T:
            setTranslation(arg1, arg2);
            break;
        case ROTATION_T:
            setRotation(arg1);
            break;
        case SCALING_T:
            setScaling(arg1, arg2);
            break;
        default:
            _resetId();
    }
}

Matrix33f::Matrix33f(const Matrix33f &other) :
    Array<3, Vec3f>(other), m_tx(other.Tx()), m_ty(other.Ty()), m_rad(other.Rad()), m_vx(other.Vx()), m_vy(other.Vy())
{}

//====================================================================================================================================
//Opérateurs

Matrix33f Matrix33f::operator*(const Matrix33f& matrix) const
{
    Matrix33f result;
    for(unsigned int i=0; i<3; i++)
    {
        for(unsigned int j=0; j<3; j++)
        {
            result[i][j]=0;
            for(unsigned int k=0; k<3; k++)
            {
                result[i][j]+=(m_tab[i][k]*matrix[k][j]);
            }
        }
    }
    result.forceTx(matrix.Tx());
    result.forceTy(matrix.Ty());
    result.forceRad(matrix.Rad());
    result.forceVx(matrix.Vx());
    result.forceVy(matrix.Vy());
    return result;
}

Vec3f Matrix33f::operator*(const Vec3f& vector) const
{
    Vec3f result;
    result.setX(m_tab[0].X()*vector.X()+m_tab[0].Y()*vector.Y()+m_tab[0].Z()*vector.Z());
    result.setY(m_tab[1].X()*vector.X()+m_tab[1].Y()*vector.Y()+m_tab[1].Z()*vector.Z());
    result.setZ(m_tab[2].X()*vector.X()+m_tab[2].Y()*vector.Y()+m_tab[2].Z()*vector.Z());
    return result;
}

Vec2f Matrix33f::operator*(const Vec2f& vector) const
{
    Vec2f result;
    result.setX(m_tab[0].X()*vector.X()+m_tab[0].Y()*vector.Y()+m_tab[0].Z()*1.0f);
    result.setY(m_tab[1].X()*vector.X()+m_tab[1].Y()*vector.Y()+m_tab[1].Z()*1.0f);
    return result;
}

//====================================================================================================================================
//Opérations

Matrix33f Matrix33f::invert() const
{
    Matrix33f m;
    float det   =m_tab[0].X()*(m_tab[1].Y()*m_tab[2].Z()-m_tab[2].Y()*m_tab[1].Z())
                -m_tab[0].Y()*(m_tab[1].X()*m_tab[2].Z()-m_tab[2].X()*m_tab[1].Z())
                +m_tab[0].Z()*(m_tab[1].X()*m_tab[2].Y()-m_tab[2].X()*m_tab[1].Y());
    if(det>_e_ || det<-_e_)//rappel : _e_ = epsilon pour erreur flottante
    {
        m[0][0]=(m_tab[1].Y() * m_tab[2].Z() - m_tab[2].Y() * m_tab[1].Z())/det;
        m[0][1]=-(m_tab[0].Y() * m_tab[2].Z() - m_tab[2].Y() * m_tab[0].Z())/det;
        m[0][2]=(m_tab[0].Y() * m_tab[1].Z() - m_tab[1].Y() * m_tab[0].Z())/det;

        m[1][0]=-(m_tab[1].X() * m_tab[2].Z() - m_tab[2].X() * m_tab[1].Z())/det;
        m[1][1]=(m_tab[0].X() * m_tab[2].Z() - m_tab[2].X() * m_tab[0].Z())/det;
        m[1][2]=-(m_tab[0].X() * m_tab[1].Z() - m_tab[1].X() * m_tab[0].Z())/det;

        m[2][0]=(m_tab[1].X() * m_tab[2].Y() - m_tab[2].X() * m_tab[1].Y())/det;
        m[2][1]=-(m_tab[0].X() * m_tab[2].Y() - m_tab[2].X() * m_tab[0].Y())/det;
        m[2][2]=(m_tab[0].X() * m_tab[1].Y() - m_tab[1].X() * m_tab[0].Y())/det;
    }
    return m;
}

//====================================================================================================================================

void Matrix33f::setId()
{
    _resetId();
    m_tx=0;
    m_ty=0;
    m_rad=0;
    m_vx=0;
    m_vy=0;
}

//====================================================================================================================================

void Matrix33f::setTranslation(float tx, float ty)
{
    m_tab[0][0]=1.0f;
    m_tab[0][1]=0;
    m_tab[0][2]=tx;

    m_tab[1][0]=0;
    m_tab[1][1]=1.0f;
    m_tab[1][2]=ty;

    m_tab[2][0]=0;
    m_tab[2][1]=0;
    m_tab[2][2]=1.0f;

    m_tx=tx;
    m_ty=ty;
}

//====================================================================================================================================

void Matrix33f::setRotation(float rad)
{
    float cosinus=cos(rad);
    float sinus=sin(rad);
    m_tab[0][0]=cosinus;
    m_tab[0][1]=sinus;
    m_tab[0][2]=0;

    m_tab[1][0]=-sinus;
    m_tab[1][1]=cosinus;
    m_tab[1][2]=0;

    m_tab[2][0]=0;
    m_tab[2][1]=0;
    m_tab[2][2]=1.0f;

    m_rad=rad;
}

//====================================================================================================================================

void Matrix33f::setScaling(float vx, float vy)
{
    m_tab[0][0]=vx;
    m_tab[0][1]=0;
    m_tab[0][2]=0;

    m_tab[1][0]=0;
    m_tab[1][1]=vy;
    m_tab[1][2]=0;

    m_tab[2][0]=0;
    m_tab[2][1]=0;
    m_tab[2][2]=1.0f;

    m_vx=vx;
    m_vy=vy;
}

//====================================================================================================================================
//Note : On suppose que les matrices ne contiennent pas de coordonnées "parasites"

void Matrix33f::addTranslation(float tx, float ty)
{
    Matrix33f m(TRANSLATION_T, tx, ty);
    *this=m*(*this);

    m_tx=tx;
    m_ty=ty;
}

//====================================================================================================================================

void Matrix33f::addRotation(float rad)
{
    Matrix33f mRotate(ROTATION_T, rad, 0);
    *this=mRotate*(*this);

    m_rad=rad;
}

//====================================================================================================================================

void Matrix33f::addScaling(float vx, float vy)
{
    Matrix33f m(SCALING_T, vx, vy);
    *this=m*(*this);

    m_vx=vx;
    m_vy=vy;
}

//====================================================================================================================================

void Matrix33f::apply(Vec2f &vector) const
{
    Vec3f vec3(vector, 1.0f);
    vec3=(*this)*vec3;
    vector.setX(vec3.X());
    vector.setY(vec3.Y());
}

//====================================================================================================================================
//Accesseurs//

float Matrix33f::Rad() const
{
    return m_rad;
}

float Matrix33f::Tx() const
{
    return m_tx;
}

float Matrix33f::Ty() const
{
    return m_ty;
}

float Matrix33f::Vx() const
{
    return m_vx;
}

float Matrix33f::Vy() const
{
    return m_vy;
}

//====================================================================================================================================
//Setters//

void Matrix33f::forceRad(float rad)
{
    m_rad=rad;
}

void Matrix33f::forceTx(float tx)
{
    m_tx=tx;
}

void Matrix33f::forceTy(float ty)
{
    m_ty=ty;
}

void Matrix33f::forceVx(float vx)
{
    m_vx=vx;
}

void Matrix33f::forceVy(float vy)
{
    m_vy=vy;
}

//====================================================================================================================================
//Private//

void Matrix33f::_resetId()
{
    m_tab[0][0]=1.0f;
    m_tab[0][1]=0;
    m_tab[0][2]=0;

    m_tab[1][0]=0;
    m_tab[1][1]=1.0f;
    m_tab[1][2]=0;

    m_tab[2][0]=0;
    m_tab[2][1]=0;
    m_tab[2][2]=1.0f;
}
