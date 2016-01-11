#ifndef POLYGON_H
#define POLYGON_H

#include "vec2i.h"
#include "vec2f.h"
#include <map>

typedef enum {UNKNOWN_O, LEFT_O, RIGHT_O, SPECIAL_O} Orientation_t;

class PolygonNi
{
public:
    PolygonNi();
    ~PolygonNi();

    unsigned int insert(const Vec2i& vertice); ///< inserts and returns index of inserted
    void clear();

    const Vec2f& fCenter() const;
    const Vec2i& iCenter() const;

    //Map//

    typedef std::map<unsigned int, Vec2i>::iterator iterator;
    typedef std::map<unsigned int, Vec2i>::const_iterator const_iterator;

    size_t size() const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    //Opérations//

    void rotate90Left();
    void scale(float scaleX, float scaleY);

    void computeDatas();

    //Accesseurs//

    bool isConvex() const;
    Orientation_t orientation() const;
    int normal() const;


private:

    void _updateICenter();
    void _workOnConvexity(int& vpPast, int& vpFuture,
                          const_iterator& itV1, const_iterator& itV2, const_iterator& itV3,
                          Vec2i& v2v1, Vec2i& v2v3, const_iterator &itMostLeft);

    unsigned int m_count;
    std::map<unsigned int, Vec2i> m_map;
    Vec2f m_fCenter;
    Vec2i m_iCenter;

    bool m_isConvex;
    Orientation_t m_orientation;
    int m_normal; //z coord of polygon
};

#endif // POLYGON_H
