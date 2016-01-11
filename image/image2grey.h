#ifndef IMAGE2GREY_H
#define IMAGE2GREY_H

#include <fstream>
#include "image2d.hpp"
#include "imgGradient.h"

class Image2Grey : public Image2<unsigned char>
{
public:

    //Constructeurs//
    Image2Grey();

    Image2Grey(const Image2Grey &other);

    Image2Grey(size_t width, size_t height);

    Image2Grey(const std::string &filename);

    //Opérations//

    void load(const std::string &filename);

    void save(const std::string &filename) const;

    //Project//

    void project_erode_black(Image2Grey& img_out, bool con8=false) const; //requires binary img
};

#endif // IMAGE2GREY_H
