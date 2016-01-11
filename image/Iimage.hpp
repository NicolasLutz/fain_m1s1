#ifndef IMAGE_H
#define IMAGE_H

class IImage
{
public:
    virtual ~IImage() {}
    virtual void* getArray() const=0;
    virtual void clear()=0;
    virtual size_t getWidth() const=0;
    virtual size_t getHeight() const=0;
};

#endif // IMAGE_H
