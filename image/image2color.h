#ifndef IMAGE2COLOR_H
#define IMAGE2COLOR_H

#include "image2d.hpp"
#include "image2grey.h"
#include "vec2i.h"
#include "color.h"
#include "GL/gl.h"
#include "imgGradient.h"
#include <fstream>
#include <stack>
#include <queue>

class Image2Grey;

class Image2Color : public Image2<Color>
{
public:

    //Constructeurs//
    Image2Color();

    Image2Color(const Image2Color &other);

    Image2Color(size_t width, size_t height);

    Image2Color(const std::string &filename);

    //Opérations//

    void load(const std::string &filename);

    void save(const std::string &filename) const;

    void fill       (const Color& c=s_currentColor);
    void genChecker (int step, const Color& c1, const Color& c2=s_currentColor);

    void changeColor(const Color& c);

    void plot  (const Vec2i& vertice, const Color& c=s_currentColor);
    void plot  (int x, int y, const Color& c=s_currentColor);

    void focusPoint (const Vec2i& vertice);
    void zoomInit   ();
    void zoom       (double zoom_coef);
    void move       (const Vec2i& vertice);

    //void draw() const;
    Vec2i toOrthonormal(const Vec2i& vertice) const;
    Vec2i fromOrthonormal(const Vec2i& vertice) const;

    ///PROJET///
    //Dessin//

    void project_bresenhamLine(const Vec2i& A, const Vec2i& B, const Color &color=s_currentColor);
    void project_bresenhamCircle(const Vec2i& O, int r, const Color &color=s_currentColor);
    void project_fill_rec(const Vec2i& origin, const Color &color=s_currentColor, bool con8=false);
    void project_fill_balaye(const Vec2i& origin, const Color &color=s_currentColor, bool con8=false);

    //Fills using project_fill_balaye, except the output image is another.
    //The other image must be black AND HAVE THE SAME SIZE THAN THIS.
    //The function returns false if the color in input is black. This should mean the connexity is > 255.
    bool project_fill_on_other(const Vec2i& origin, Image2Grey& img_out, const unsigned char out_color, bool con8=false);

    //Analyse//

    void project_toGreyScale(Image2Grey &imgGrey) const;
    void project_toBinary(Image2Grey &imgGrey, unsigned char seuil) const;

    //img_out is the img the connexity is shown.
    //steps specifies how fast the lvl of connexity should drop to 0.
    int project_showCon(Image2Grey &img_out, bool con8=false, unsigned char step=1);


    int project_erodeAt(const Vec2i& origin, bool con8=false);

    //Static

    void windowToImage(const Vec2i& winOffset, Vec2i& imgOffset) const;
    void imageToWindow(const Vec2i& imgOffset, Vec2i& winOffset);

private:

    void _loadPpm(std::ifstream& ifs);
    void _loadPgm(std::ifstream& ifs);
    void _loadPbm(std::ifstream& ifs);

    void _fillConBorder(bool& region_done, const Vec2i& pAround, const Color& cRand, const Color &cDetect);

    static Color s_currentColor;

    int m_xZoom;
    int m_yZoom;

    int m_xOffset;
    int m_yOffset;

    double m_zoom;
};

#endif // IMAGE2COLOR_H
