#ifndef Image2D_US_HPP
#define Image2D_US_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include "vec2i.h"
#include "Iimage.hpp"

template<typename T> class Image2 : public IImage //US stands for unknown size
{
public:

    //Constructeurs//

    Image2();

    Image2(const Image2<T> &other);

    Image2(size_t width, size_t height, const T& defaultValue=T());

    virtual ~Image2();

    //Opérateurs//

    Image2& operator=(const Image2<T> &other);

    //Accesseurs//

    T& getPixel(unsigned int i, unsigned int j);
    const T& getPixel(unsigned int i, unsigned int j) const;

    T& getPixel(const Vec2i& pos);
    const T& getPixel(const Vec2i& pos) const;

    void setPixel(const T& pixel, unsigned int i, unsigned int j);
    void setPixel(const T& pixel, int i, int j);
    void setPixel(const T& pixel, const Vec2i& pos);

    void resize(size_t width, size_t height);

    size_t getWidth() const;
    size_t getHeight() const;

    void* getArray() const;

    const T& getDefault() const;
    void setDefault(const T& defaultValue);

    bool changed();

    //Opérations//

    void clear();

    //Itérateurs//

    class iterator
    {
    public:
        typedef iterator self_type;
        typedef int difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::forward_iterator_tag iterator_category;

        iterator() {}
        iterator(value_type* ptr);
        iterator(value_type* ptr,
                      unsigned int xStart, unsigned int xStop,
                      unsigned int yStart, size_t width);

        self_type operator=(const self_type& other);
        self_type operator++();
        self_type operator++(int unusued);
        value_type& operator*();
        value_type* operator->();
        bool operator==(const self_type& other);
        bool operator!=(const self_type& other);

        //Hors interface//
        Vec2i Vertice(); ///< returns the vertice in img coordinates

    private:
        value_type*     m_ptr;
        unsigned int    m_xStart; ///< at which x are we supposed to start?
        unsigned int    m_xStop; ///< at which x is the box's edge?
        size_t          m_spaceBetweenLines; ///< used for jumping to another line
        unsigned int    m_xIndex;
        unsigned int    m_yIndex;
    };

    iterator begin();
    iterator end();

    iterator beginOf(int xStart, int xStop, int yStart);
    iterator endOf(int xStop, int yStop);

    //Const itérateur//
    class const_iterator
    {
    public:
        typedef const_iterator self_type;
        typedef int difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::forward_iterator_tag iterator_category;

        const_iterator() {}
        const_iterator(const value_type* ptr);
        const_iterator(const value_type* ptr,
                      unsigned int xStart, unsigned int xStop,
                      unsigned int yStart, size_t width);

        self_type operator=(const self_type& other);
        self_type operator++();
        self_type operator++(int unusued);
        const value_type& operator*();
        const value_type* operator->();
        bool operator==(const self_type& other);
        bool operator!=(const self_type& other);

        //Hors interface//
        Vec2i Vertice();

    private:
        const value_type*   m_ptr;
        unsigned int        m_xStart; ///< at which x are we supposed to start?
        unsigned int        m_xStop; ///< at which x is the box's edge?
        size_t              m_spaceBetweenLines; ///< used for jumping to another line
        unsigned int        m_xIndex;
        unsigned int        m_yIndex;
    };

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator beginOf(int xStart, int xStop, int yStart) const;
    const_iterator endOf(int xStop, int yStop) const;

protected:

    size_t m_width;
    size_t m_height;
    T *m_tab;
    T m_default; ///< reference returned in case getPixel() is not valid
    bool m_changed;
};

//====================================================================================================================================
//Constructeurs//

template<typename T>
Image2<T>::Image2() :
    m_width(0), m_height(0), m_tab(0), m_changed(true)
{}

template<typename T>
Image2<T>::Image2(const Image2<T> &other) :
    m_width(other.getWidth()), m_height(other.getHeight()), m_tab(new T[m_width*m_height]), m_default(other.getDefault()), m_changed(true)
{
    memcpy(m_tab,other.getArray(),m_width*m_height*sizeof(T));
}

template<typename T>
Image2<T>::Image2(size_t width, size_t height, const T &defaultValue) :
    m_width(width), m_height(height), m_tab(new T[m_width*m_height]), m_default(defaultValue), m_changed(true)
{}

template<typename T>
Image2<T>::~Image2()
{
    delete m_tab;
}

//====================================================================================================================================
//Opérateurs//

template<typename T>
Image2<T>& Image2<T>::operator=(const Image2<T> &other)
{
    if(m_tab!=NULL)
        delete m_tab;
    m_width=other.getWidth();
    m_height=other.getHeight();
    m_tab=new T[m_width*m_height];
    memcpy(m_tab,other.getArray(),m_width*m_height*sizeof(T));
    m_changed=true;
    return (*this);
}

//====================================================================================================================================
//Accesseurs//

template<typename T>
T& Image2<T>::getPixel(const Vec2i& pos)
{
    m_changed=true;
    if((unsigned int)pos.X()<m_width && (unsigned int)pos.Y()<m_height)
        return m_tab[(unsigned int)pos.Y()*m_width+(unsigned int)pos.X()];
    return m_default;
}

template<typename T>
T& Image2<T>::getPixel(unsigned int i, unsigned int j)
{
    m_changed=true;
    if(i<m_width && j<m_height)
        return m_tab[j*m_width+i];
    return m_default;
}

template<typename T>
const T& Image2<T>::getPixel(const Vec2i& pos) const
{
    if((unsigned int)pos.X()<m_width && (unsigned int)pos.Y()<m_height)
        return m_tab[(unsigned int)pos.Y()*m_width+(unsigned int)pos.X()];
    return m_default;
}

template<typename T>
const T& Image2<T>::getPixel(unsigned int i, unsigned int j) const
{
    if(i<m_width && j<m_height)
        return m_tab[j*m_width+i];
    return m_default;
}

//====================================================================================================================================

template<typename T>
void Image2<T>::setPixel(const T& pixel, unsigned int i, unsigned int j)
{
    m_changed=true;
    if(i<m_width && j<m_height)
        m_tab[j*m_width+i]=pixel;
}

template<typename T>
void Image2<T>::setPixel(const T& pixel, int i, int j)
{
    m_changed=true;
    if(((unsigned int)i)<m_width && ((unsigned int)j)<m_height)
        m_tab[j*m_width+i]=pixel;
}

template<typename T>
void Image2<T>::setPixel(const T& pixel, const Vec2i& pos)
{
    setPixel(pixel, pos.X(), pos.Y());
}

template<typename T>
void Image2<T>::resize(size_t width, size_t height)
{
    if(m_tab!=NULL)
        delete m_tab;
    m_width=width;
    m_height=height;
    m_tab=new T[m_width*m_height];
}

//====================================================================================================================================

template<typename T>
size_t Image2<T>::getWidth() const
{
    return m_width;
}

//====================================================================================================================================

template<typename T>
size_t Image2<T>::getHeight() const
{
    return m_height;
}

//====================================================================================================================================

template<typename T>
void* Image2<T>::getArray() const
{
    return (void *)m_tab;
}

template<typename T>
const T& Image2<T>::getDefault() const
{
    return m_default;
}

template<typename T>
void Image2<T>::setDefault(const T& defaultValue)
{
    m_changed=true;
    m_default=defaultValue;
}

template<typename T>
bool Image2<T>::changed()
{
    bool ret=m_changed;
    m_changed=false;
    return ret;
}

//====================================================================================================================================
//Itérateurs//

template<typename T>
void Image2<T>::clear()
{
    m_changed=true;
    iterator end=this->end();
    for(iterator it=begin();it!=end;++it)
    {
        *it=T(0);
    }
}

//====================================================================================================================================
//Itérateurs//

template<typename T>
typename Image2<T>::iterator Image2<T>::begin()
{
    m_changed=true;
    iterator it(m_tab, 0, m_width-1, 0, m_width);
    return it;
}

template<typename T>
typename Image2<T>::iterator Image2<T>::end()
{
    m_changed=true;
    iterator it(m_tab+(m_height*m_width));
    return it;
}

template<typename T>
typename Image2<T>::iterator Image2<T>::beginOf(int xStart, int xStop, int yStart)
{
    m_changed=true;
    unsigned int validXStart=std::max(0, std::min(xStart, (int)m_width-1));
    unsigned int validXStop=std::max(0, std::min(xStop, (int)m_width-1));
    unsigned int validYStart=std::max(0, std::min(yStart, (int)m_height-1));
    iterator it(m_tab+(validYStart*m_width)+validXStart, validXStart, validXStop, validYStart, m_width);
    return it;
}

template<typename T>
typename Image2<T>::iterator Image2<T>::endOf(int xStop, int yStop)
{
    m_changed=true;
    unsigned int validXStop=std::max(0, std::min(xStop, (int)m_width-1));
    unsigned int validYStop=std::max(0, std::min(yStop, (int)m_height-1));
    iterator it(m_tab+(validYStop*m_width)+validXStop+1);
    return it;
}

//====================================================================================================================================
//Const Itérateur//

template<typename T>
typename Image2<T>::const_iterator Image2<T>::begin() const
{
    const_iterator it(m_tab, 0, m_width-1, 0, m_width);
    return it;
}

template<typename T>
typename Image2<T>::const_iterator Image2<T>::end() const
{
    const_iterator it(m_tab+(m_height*m_width));
    return it;
}

template<typename T>
typename Image2<T>::const_iterator Image2<T>::beginOf(int xStart, int xStop, int yStart) const
{
    unsigned int validXStart=std::max(0, std::min(xStart, (int)m_width-1));
    unsigned int validXStop=std::max(0, std::min(xStop, (int)m_width-1));
    unsigned int validYStart=std::max(0, std::min(yStart, (int)m_height-1));
    const_iterator it(m_tab+(validYStart*m_width)+validXStart, validXStart, validXStop, validYStart, m_width);
    return it;
}

template<typename T>
typename Image2<T>::const_iterator Image2<T>::endOf(int xStop, int yStop) const
{
    unsigned int validXStop=std::max(0, std::min(xStop, (int)m_width-1));
    unsigned int validYStop=std::max(0, std::min(yStop, (int)m_height-1));
    const_iterator it(m_tab+(validYStop*m_width)+validXStop+1);
    return it;
}

//====================================================================================================================================
//====================================================================================================================================
//Image2<T>::Itérateur//

template<typename T>
Image2<T>::iterator::iterator(value_type* ptr):
    m_ptr(ptr)
{}

template<typename T>
Image2<T>::iterator::iterator(value_type* ptr, unsigned int xStart, unsigned int xStop,
                                         unsigned int yStart, size_t width):
  m_ptr(ptr)
, m_xStart(xStart)
, m_xStop(xStop)
, m_spaceBetweenLines((width-m_xStop)+m_xStart)
, m_xIndex(xStart)
, m_yIndex(yStart)
{}

template<typename T>
typename Image2<T>::iterator::self_type Image2<T>::iterator::operator=(const self_type& other)
{
    m_ptr = other.m_ptr;
    m_xStart=other.m_xStart;
    m_xStop=other.m_xStop;
    m_spaceBetweenLines=other.m_spaceBetweenLines;
    m_xIndex=other.m_xIndex;
    m_yIndex=other.m_yIndex;
    return *this;
}

template<typename T>
typename Image2<T>::iterator::self_type Image2<T>::iterator::operator++()
{
    if(m_xIndex==m_xStop)
    {
        m_xIndex=m_xStart;
        m_ptr+=m_spaceBetweenLines;
        m_yIndex++;
    }
    else
    {
        m_xIndex++;
        m_ptr++;
    }
    return *this;
}

template<typename T>
typename Image2<T>::iterator::self_type Image2<T>::iterator::operator++(int unusued)
{
    iterator i=*this;
    if(m_xIndex==m_xStop)
    {
        m_xIndex=m_xStart;
        m_ptr+=m_spaceBetweenLines;
        m_yIndex++;
    }
    else
    {
        m_xIndex++;
        m_ptr++;
    }
    return i;
}

template<typename T>
typename Image2<T>::iterator::value_type& Image2<T>::iterator::operator*()
{
    return *m_ptr;
}

template<typename T>
typename Image2<T>::iterator::value_type* Image2<T>::iterator::operator->()
{
    return m_ptr;
}

template<typename T>
bool Image2<T>::iterator::operator==(const self_type& other)
{
    return m_ptr == other.m_ptr;
}

template<typename T>
bool Image2<T>::iterator::operator!=(const self_type& other)
{
    return m_ptr < other.m_ptr;
}

template<typename T>
Vec2i Image2<T>::iterator::Vertice()
{
    Vec2i v(m_xIndex, m_yIndex);
    return v;
}


//====================================================================================================================================
//====================================================================================================================================
//Image2<T>::Const Itérateur//

template<typename T>
Image2<T>::const_iterator::const_iterator(const value_type* ptr):
    m_ptr(ptr)
{}

template<typename T>
Image2<T>::const_iterator::const_iterator(const value_type* ptr, unsigned int xStart, unsigned int xStop, unsigned int yStart, size_t width):
  m_ptr(ptr)
, m_xStart(xStart)
, m_xStop(xStop)
, m_spaceBetweenLines((width-m_xStop)+m_xStart)
, m_xIndex(xStart)
, m_yIndex(yStart)
{}

template<typename T>
typename Image2<T>::const_iterator::self_type Image2<T>::const_iterator::operator=(const self_type& other)
{
    m_ptr = other.m_ptr;
    m_xStart=other.m_xStart;
    m_xStop=other.m_xStop;
    m_spaceBetweenLines=other.m_spaceBetweenLines;
    m_xIndex=other.m_xIndex;
    m_yIndex=other.m_yIndex;
    return *this;
}

template<typename T>
typename Image2<T>::const_iterator::self_type Image2<T>::const_iterator::operator++()
{
    if(m_xIndex==m_xStop)
    {
        m_xIndex=m_xStart;
        m_ptr+=m_spaceBetweenLines;
        m_yIndex++;
    }
    else
    {
        m_xIndex++;
        m_ptr++;
    }
    return *this;
}

template<typename T>
typename Image2<T>::const_iterator::self_type Image2<T>::const_iterator::operator++(int unusued)
{
    const_iterator i=*this;
    if(m_xIndex==m_xStop)
    {
        m_xIndex=m_xStart;
        m_ptr+=m_spaceBetweenLines;
        m_yIndex++;
    }
    else
    {
        m_xIndex++;
        m_ptr++;
    }
    return i;
}

template<typename T>
const typename Image2<T>::const_iterator::value_type& Image2<T>::const_iterator::operator*()
{
    return *m_ptr;
}

template<typename T>
const typename Image2<T>::const_iterator::value_type* Image2<T>::const_iterator::operator->()
{
    return m_ptr;
}

template<typename T>
bool Image2<T>::const_iterator::operator==(const self_type& other)
{
    return m_ptr == other.m_ptr;
}

template<typename T>
bool Image2<T>::const_iterator::operator!=(const self_type& other)
{
    return m_ptr < other.m_ptr;
}

template<typename T>
Vec2i Image2<T>::const_iterator::Vertice()
{
    Vec2i v(m_xIndex, m_yIndex);
    return v;
}

#endif // Image2D_US_UNKNOWNSIZE_HPP
