#ifndef ARRAY_H
#define ARRAY_H

#include <stdexcept>
#include <iostream>

template<size_t size, typename T> class Array
{
public:

    //Constructeurs//

    Array();
    Array(const Array<size,T> &other);

    //Opérateurs//

    Array<size, T>& operator=(const Array<size,T> &other);
    T& operator[](unsigned int i);
    const T& operator[](unsigned int i) const;

    bool operator==(const Array<size,T> &other) const;
    bool operator!=(const Array<size,T> &other) const;

    //Accesseurs//

    size_t Size() const;

    //Opérations//

    void exchangeWith(Array<size,T> &other);

    //Itérateurs//

    class iterator
    {
    public:
        typedef iterator self_type;
        typedef int difference_type;
        typedef T   value_type;
        typedef T*  pointer;
        typedef T&  reference;
        typedef std::bidirectional_iterator_tag iterator_category;

        iterator() {}
        iterator(value_type* ptr) : m_ptr(ptr) {}

        inline self_type operator=(self_type other) { m_ptr = other.m_ptr; return *this; }
        inline self_type operator++() { m_ptr++; return *this; }
        inline self_type operator++(int unusued) { self_type i = *this; m_ptr++; return i; }
        inline self_type operator--() { m_ptr--; return *this; }
        inline self_type operator--(int unusued) { self_type i = *this; m_ptr--; return i; }
        inline value_type& operator*() { return *m_ptr; }
        inline value_type* operator->() { return m_ptr; }
        inline bool operator==(const self_type& other) { return m_ptr == other.m_ptr; }
        inline bool operator!=(const self_type& other) { return m_ptr != other.m_ptr; }
    private:
        value_type* m_ptr;
    };

    class const_iterator
    {
    public:
        typedef const_iterator self_type;
        typedef int difference_type;
        typedef T   value_type;
        typedef T*  pointer;
        typedef T&  reference;
        typedef std::bidirectional_iterator_tag iterator_category;

        const_iterator() {}
        const_iterator(const value_type *ptr) : m_ptr(ptr) {}

        inline self_type operator=(self_type other) { m_ptr = other.m_ptr; return *this; }
        inline self_type operator++() { m_ptr++; return *this; }
        inline self_type operator++(int unusued) { self_type i = *this; m_ptr++; return i; }
        inline self_type operator--() { m_ptr--; return *this; }
        inline self_type operator--(int unusued) { self_type i = *this; m_ptr--; return i; }
        inline const value_type& operator*() { return *m_ptr; }
        inline const value_type* operator->() { return m_ptr; }
        inline bool operator==(const self_type& other) const { return m_ptr == other.m_ptr; }
        inline bool operator!=(const self_type& other) const { return m_ptr != other.m_ptr; }
    private:
        const value_type* m_ptr;
    };

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    //Autres//
    template<size_t ssize, typename sT>
    friend std::ostream& operator<< (std::ostream &stream, const Array<ssize,sT> &object);

    template<size_t ssize, typename sT>
    friend std::istream& operator>> (std::istream &stream, Array<ssize,sT> &object);

protected:
    T m_tab[size];
};


//------------------------------------------------
//------------------------------------------------
//====================================================================================================================================
//Constructeurs//

template<size_t size, typename T>
Array<size,T>::Array()
{}

template<size_t size, typename T>
Array<size,T>::Array(const Array<size,T> &other)
{
    typename Array<size,T>::iterator itThis;
    typename Array<size,T>::const_iterator citOther;
    for(itThis=begin(), citOther=other.begin(); itThis!=end() && citOther!=other.end(); ++itThis, ++citOther)
    {
        *itThis=*citOther;
    }
}

//====================================================================================================================================
//Opérateurs//

template<size_t size, typename T>
Array<size,T>& Array<size,T>::operator=(const Array<size, T> &other)
{
    typename Array<size,T>::iterator itThis;
    typename Array<size,T>::const_iterator citOther;
    for(itThis=begin(), citOther=other.begin(); itThis!=end() && citOther!=other.end(); ++itThis, ++citOther)
    {
        *itThis=*citOther;
    }
    return *this;
}

//====================================================================================================================================

template<size_t size, typename T>
T& Array<size,T>::operator[](unsigned int i)
{
    if(i>=size || i<0)
    {
         throw std::out_of_range("argument out of bounds");
    }
    return m_tab[i];
}

template<size_t size, typename T>
const T& Array<size,T>::operator[](unsigned int i) const
{
    if(i>=size || i<0)
    {
         throw std::out_of_range("argument out of bounds");
    }
    return m_tab[i];
}

//====================================================================================================================================

template<size_t size, typename T>
bool Array<size,T>::operator==(const Array<size,T> &other) const
{
    bool ret=true;
    typename Array<size,T>::const_iterator citThis, citOther;
    for(citThis=begin(), citOther=other.begin(); ret && citThis!=end() && citOther!=other.end(); ++citThis, ++citOther)
    {
        if(*citThis!=*citOther)
            ret=false;
    }
    return ret;
}

template<size_t size, typename T>
bool Array<size,T>::operator!=(const Array<size,T> &other) const
{
    return !operator==(other);
}

//====================================================================================================================================
//Accesseurs//

template<size_t size, typename T>
size_t Array<size, T>::Size() const
{
    return size;
}

//====================================================================================================================================
//Opérations//

template<size_t size, typename T>
void Array<size,T>::exchangeWith(Array<size,T> &other)
{
    Array<size,T> copy;
    copy=other;
    for(unsigned int i=0;i<size;i++)
    {
        other[i]=m_tab[i];
        m_tab[i]=copy[i];
    }
}

//====================================================================================================================================
//Itérateurs//

template<size_t size, typename T>
typename Array<size,T>::iterator Array<size,T>::begin()
{
    return iterator(m_tab);
}

template<size_t size, typename T>
typename Array<size,T>::iterator Array<size,T>::end()
{
    return iterator(m_tab + size);
}

//====================================================================================================================================

template<size_t size, typename T>
typename Array<size,T>::const_iterator Array<size,T>::begin() const
{
    return const_iterator(m_tab);
}

template<size_t size, typename T>
typename Array<size,T>::const_iterator Array<size,T>::end() const
{
    return const_iterator(m_tab + size);
}

//====================================================================================================================================
//Autres//

template<size_t size, typename T>
std::ostream& operator<< (std::ostream &stream, const Array<size,T> &object)
{
    typename Array<size,T>::const_iterator cit;
    for(cit=object.begin(); cit!=object.end(); ++cit)
    {
        stream << *cit << ' ';
    }
    return stream;
}

template<size_t size, typename T>
std::istream& operator>> (std::istream &stream, Array<size,T> &object)
{
    typename Array<size,T>::iterator it;
    for(it=object.begin(); it!=object.end(); ++it)
    {
        stream >> *it;
    }
    return stream;
}

#endif // ARRAY_H
