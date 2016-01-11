#include "polygon.h"

PolygonNi::PolygonNi():
    m_count(0), m_map(), m_fCenter(0,0), m_isConvex(true), m_orientation(UNKNOWN_O), m_normal(0)
{}

PolygonNi::~PolygonNi()
{}

unsigned int PolygonNi::insert(const Vec2i& vertice)
{
    m_map.insert(std::pair<unsigned int, Vec2i>(m_count++, vertice));
    m_fCenter+=((Vec2f(vertice)-m_fCenter)/(m_count));
    _updateICenter();
    return m_count-1;
}

void PolygonNi::clear()
{
    m_map.clear();
    m_count=0;
    m_fCenter.setX(0);
    m_fCenter.setY(0);
    m_isConvex=true;
    m_orientation=UNKNOWN_O;
    m_normal=0;
    _updateICenter();
}

const Vec2f& PolygonNi::fCenter() const
{
    return m_fCenter;
}

const Vec2i& PolygonNi::iCenter() const
{
    return m_iCenter;
}

//Map//

size_t PolygonNi::size() const
{
    return m_count;
}

PolygonNi::iterator PolygonNi::begin()
{
    return m_map.begin();
}

PolygonNi::iterator PolygonNi::end()
{
    return m_map.end();
}

PolygonNi::const_iterator PolygonNi::begin() const
{
    return m_map.begin();
}

PolygonNi::const_iterator PolygonNi::end() const
{
    return m_map.end();
}

//Opérations//

void PolygonNi::rotate90Left()
{
    for(iterator it=begin(); it!=end(); ++it)
    {
        (*it).second.rotate90Left(m_iCenter);
    }
}

void PolygonNi::scale(float scaleX, float scaleY)
{
    for(iterator it=begin(); it!=end(); ++it)
    {
        (*it).second.scale(m_iCenter, scaleX, scaleY);
    }
}

void PolygonNi::computeDatas()
{
    if(size()<3)
        return;
    //Is it convex?
    const_iterator itV1=begin(), itV2=begin(), itV3=begin();
    ++itV2;
    ++itV3;++itV3;
    Vec2i v2v1, v2v3;
    const_iterator itLeft=itV2; //For the orientation
    int vpPast=0, vpFuture=0; //vectorial product
    while(itV3!=end())
    {
        _workOnConvexity(vpPast, vpFuture, itV1, itV2, itV3, v2v1, v2v3, itLeft);
        ++itV1;
        ++itV2;
        ++itV3;
    }
    //Continue two times like a cycle
    itV3=begin();
    _workOnConvexity(vpPast, vpFuture, itV1, itV2, itV3, v2v1, v2v3, itLeft);
    ++itV1;
    itV2=begin();
    ++itV3;
    _workOnConvexity(vpPast, vpFuture, itV1, itV2, itV3, v2v1, v2v3, itLeft);

    //What is its orientation and its normal?
    {
        m_normal=vpPast; //vpPast is currently set correctly
        if(m_isConvex) //Since the normal is the same sign everywhere...
            m_orientation=(m_normal>0 ? LEFT_O : m_normal<0 ? RIGHT_O : SPECIAL_O);
        else
        { //Find left and right of iterator of the most left vertice
            int ileft;
            unsigned int uileft, uiright;
            ileft  = (int)((*itLeft).first)-1;
            uileft = ileft>0 ? (unsigned int)ileft : m_count-1;
            uiright=(((*itLeft).first)+1)%m_count;
            //Calculate gradient a.k.a literally just throw which vertice is the most left from (*itLeft).second
            //since itLeft contains the vertice the most at the left
            m_orientation=((*m_map.find(uileft)).second.lowerThan((*m_map.find(uiright)).second) ? RIGHT_O : LEFT_O);
        }

    }

}

//Accesseurs//

bool PolygonNi::isConvex() const
{
    return m_isConvex;
}

Orientation_t PolygonNi::orientation() const
{
    return m_orientation;
}

int PolygonNi::normal() const
{
    return m_normal;
}

//Private//

void PolygonNi::_workOnConvexity(int& vpPast, int& vpFuture,
                                 const_iterator &itV1, const_iterator &itV2, const_iterator &itV3,
                                 Vec2i& v2v1, Vec2i& v2v3, const_iterator& itMostLeft)
{
    if(!(*itMostLeft).second.lowerThan((*itV3).second))
        itMostLeft=itV3;
    //It doesn't work ?? why ???
/*    v2v1=(*itV1).second-(*itV2).second;
    v2v3=(*itV3).second-(*itV2).second;*/
    v2v1.setX((*itV1).second.X()-(*itV2).second.X());
    v2v1.setY((*itV1).second.Y()-(*itV2).second.Y());
    v2v3.setX((*itV3).second.X()-(*itV2).second.X());
    v2v3.setY((*itV3).second.Y()-(*itV2).second.Y());
    if(m_isConvex)
    {
        vpFuture=(v2v1^v2v3);
        if(vpFuture>0 && vpPast<0)
            m_isConvex=false;
        else if(vpFuture<0 && vpPast>0)
            m_isConvex=false;
        else
            vpPast=vpFuture;
    }
}

void PolygonNi::_updateICenter()
{
    m_iCenter.setX((int)(m_fCenter.X()+0.5f));
    m_iCenter.setY((int)(m_fCenter.Y()+0.5f));
}
