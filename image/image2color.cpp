#include "image2color.h"

#define _spc_output_ << ' ' <<

Color Image2Color::s_currentColor(1,1,1);

//====================================================================================================================================
//Constructeurs//
Image2Color::Image2Color() : Image2<Color>()
{

}

Image2Color::Image2Color(const Image2Color &other) : Image2<Color>(other)
{

}

Image2Color::Image2Color(size_t width, size_t height) : Image2<Color>(width, height)
{

}

Image2Color::Image2Color(const std::string &filename) : Image2<Color>()
{
    load(filename);
}

//====================================================================================================================================

void Image2Color::load(const std::string &filename)
{
    //Requires a commentless file.
    //Every time you comment ppm files, somebody kills a puppy.
    std::ifstream ifs(filename.c_str());
    if(!ifs.is_open())
        return;
    char c_dummy;
    int magicNumber;

    ifs >> c_dummy;
    if(c_dummy!='P')
    {
        std::cerr << "Error: " << filename << " isn't a pixmap file." << std::endl;
        return;
    }
    ifs >> magicNumber;
    switch(magicNumber)
    {
    case 1:
        _loadPbm(ifs);
        break;
    case 2:
        _loadPgm(ifs);
        break;
    case 3:
        _loadPpm(ifs);
        break;
    default:
        std::cerr << "Error: " << filename << " isn't a ppm nor a pgm image." << std::endl;
        return;
    }
    ifs.close();
}

void Image2Color::_loadPpm(std::ifstream& ifs)
{
    int dummyR, dummyG, dummyB; //force le programme à fetcher seulement les nombres
    ifs >> m_width;
    ifs >> m_height;
    unsigned int format;
    ifs >> format;
    size_t size=m_width*m_height;
    m_tab=new Color[size];
    for(iterator it=begin(); it!=end() && ifs.good(); ++it)
    {
        ifs >> dummyR >> dummyG >> dummyB;
        *it=Color((float)dummyR/format, (float)dummyG/format, (float)dummyB/format);
    }
}

void Image2Color::_loadPgm(std::ifstream& ifs)
{
    int dummy; //force le programme à fetcher seulement les nombres
    float fGrey;
    ifs >> m_width;
    ifs >> m_height;
    unsigned int format;
    ifs >> format; //format
    size_t size=m_width*m_height;
    m_tab=new Color[size];
    for(iterator it=begin(); it!=end() && ifs.good(); ++it)
    {
        ifs >> dummy;
        fGrey=(float)dummy/format;
        *it=Color(fGrey, fGrey, fGrey);
    }
}

void Image2Color::_loadPbm(std::ifstream& ifs)
{
    int dummy;
    ifs >> m_width;
    ifs >> m_height;
    ifs >> dummy; //format
    size_t size=m_width*m_height;
    m_tab=new Color[size];
    for(iterator it=begin(); it!=end() && ifs.good(); ++it)
    {
        ifs >> dummy;
        if(dummy>0)
            *it=Color::White;
        else
            *it=Color::Black;
    }
}

void Image2Color::save(const std::string &filename) const
{
    int j=5;
    std::ofstream ofs(filename.c_str());
    if(!ofs.is_open())
        return;
    ofs << "P3" << std::endl;
    ofs << m_width _spc_output_ m_height << std::endl;
    ofs << 255;
    const_iterator itEnd=end();
    for(const_iterator it=begin(); it!=itEnd; ++it)
    {
        if(j++>=5) //le format accepte un maximum de 70 char/ligne, on se limitera à 6 vecteurs
        {
            j=0;
            ofs << std::endl;
        }
        else
            ofs << ' ';
        ofs << (*it).Rui(true) _spc_output_ (*it).Gui(true) _spc_output_ (*it).Bui(true);
    }
    return;
}

void Image2Color::fill(const Color& c)
{
    iterator itEnd=end();
    for(iterator it=begin(); it!=itEnd; ++it)
        (*it)=c;
}

void Image2Color::genChecker(int step, const Color& c1, const Color& c2)
{
    iterator itEnd=end();
    for(iterator it=begin(); it!=itEnd; ++it)
    {
        Vec2i vertice=it.Vertice();
        int n_x = vertice.X()/step;
        int n_y = vertice.Y()/step;
        if((n_x+n_y)%2==0)
            (*it)=c1;
        else
            (*it)=c2;
    }
}

void Image2Color::changeColor(const Color& c)
{
    s_currentColor=c;
}

void Image2Color::plot(const Vec2i& vertice, const Color& c)
{
    plot(vertice.X(), vertice.Y(), c);
}

void Image2Color::plot(int x, int y, const Color& c)
{
    unsigned int xui=(unsigned int)x;
    unsigned int yui=(unsigned int)y;
    if(xui<m_width && yui<m_height)
        setPixel(c, xui, yui);
}

void Image2Color::focusPoint(const Vec2i& vertice)
{
    int dx = vertice.X() - m_xZoom;
    int dy = vertice.Y() - m_yZoom;
    m_xOffset -= dx*(1-1.0/m_zoom);
    m_yOffset -= dy*(1-1.0/m_zoom);
    m_xZoom = vertice.X();
    m_yZoom = vertice.Y();
}

void Image2Color::zoomInit()
{
    m_xOffset = 0;
    m_yOffset = 0;
    m_zoom = 1.0;
}

void Image2Color::zoom(double zoom_coef)
{
    m_zoom = m_zoom * zoom_coef;
}

void Image2Color::move(const Vec2i& vertice)
{
    m_xOffset += vertice.X();
    m_yOffset += vertice.Y();
}

//TODO

Vec2i Image2Color::toOrthonormal(const Vec2i& vertice) const
{
    return Vec2i(vertice.X(), m_height-vertice.Y());
}

Vec2i Image2Color::fromOrthonormal(const Vec2i& vertice) const
{
    return Vec2i(vertice.X(), m_height-vertice.Y());
}

//------------------------------------------------------------------------
// Changement de repère

void Image2Color::windowToImage(const Vec2i& winOffset, Vec2i& imgOffset) const
{

    imgOffset.setX(m_xOffset + m_xZoom + (winOffset.X()-m_xZoom) / m_zoom);
    imgOffset.setY(m_yOffset + m_yZoom + (winOffset.Y()-m_yZoom) / m_zoom);
}

//------------------------------------------------------------------------
// Changement de repère inverse
void Image2Color::imageToWindow(const Vec2i& imgOffset, Vec2i& winOffset)
{

    winOffset.setX(m_xOffset + m_xZoom + (imgOffset.X()-m_xZoom-m_xOffset) * m_zoom);
    winOffset.setY(m_yOffset + m_yZoom + (imgOffset.Y()-m_yZoom-m_yOffset) * m_zoom);
}

void Image2Color::project_bresenhamLine(const Vec2i& A, const Vec2i& B, const Color& color)
{
    changeColor(color);
    int i, C, x, y, Dx, Dy, Dx2, Dy2, incX, incY;
    Dx=std::abs(B.X()-A.X());
    Dy=std::abs(B.Y()-A.Y());
    Dx2=2*Dx;
    Dy2=2*Dy;
    incX= B.X()>A.X() ? 1 : -1; //xB à droite de xA ?
    incY= B.Y()>A.Y() ? 1 : -1; //yB en haut de yA ?
    x=A.X(); y=A.Y();
    plot(x,y);
    if(Dx>Dy) //premiers octants ? (cos>sin)
    {
        C=Dy2-Dx;
        for(i=1; i<Dx; i++)
        {
            if(C<0)
                C+=Dy2;
            else
            {
                y+=incY;
                C+=Dy2-Dx2;
            }
            x+=incX;
            plot(x,y);
        }
    }
    else
    {
        C=Dx2-Dy;
        for(i=1; i<Dy; i++)
        {
            if(C<0)
                C+=Dx2;
            else
            {
                x+=incX;
                C+=Dx2-Dy2;
            }
            y+=incY;
            plot(x,y);
        }
    }
}

void Image2Color::project_bresenhamCircle(const Vec2i& O, int r, const Color &color)
{
    changeColor(color);
    int x=0,y=r,m=5 - 4*r;
    while(x<=y)
    {
        plot(x+O.X(), y+O.Y());
        plot(y+O.X(), x+O.Y());

        plot(-x+O.X(), y+O.Y());
        plot(-y+O.X(), x+O.Y());

        plot(x+O.X(), -y+O.Y());
        plot(y+O.X(), -x+O.Y());

        plot(-x+O.X(), -y+O.Y());
        plot(-y+O.X(), -x+O.Y());

        if(m>0)
        {
            y=y-1;
            m=m-8*y;
        }
        x=x+1;
        m=m + 8*x + 4;
    }
}

void Image2Color::project_fill_rec(const Vec2i& origin, const Color& color, bool con8)
{
    Color c_in=getPixel(origin.X(), origin.Y());
    if(c_in==color)
        return;
    std::stack<Vec2i> vs;
    Vec2i p(origin.X(), origin.Y());
    vs.push(p);
    plot(p, color);
    while(!vs.empty())
    {
        p=vs.top();
        vs.pop();
        if(++p.X()<(int)m_width && getPixel(p.X(), p.Y())==c_in)
        {
            vs.push(p);
            plot(p, color);
        }
        --p.X();
        if(--p.X()>=0 && getPixel(p.X(), p.Y())==c_in)
        {
            vs.push(p);
            plot(p, color);
        }
        ++p.X();
        if(++p.Y()<(int)m_height && getPixel(p.X(), p.Y())==c_in)
        {
            vs.push(p);
            plot(p, color);
        }
        --p.Y();
        if(--p.Y()>=0 && getPixel(p.X(), p.Y())==c_in)
        {
            vs.push(p);
            plot(p, color);
        }
        ++p.Y();
    }
}

void Image2Color::project_fill_balaye(const Vec2i& origin, const Color &color, bool con8)
{
    Color cDetect;
    if((cDetect=getPixel(origin))==color)
        return;
    Vec2i pw, pe, p(origin);
    std::stack<Vec2i> vs;
    vs.push(p);
    while(!vs.empty())
    {
        p=vs.top();
        vs.pop();
        if(getPixel(p)==cDetect)
        {
            pw=p;
            pe=p;
            while(pw.X()-->0 && getPixel(pw)==cDetect);
            while(pe.X()++<(int)m_width-1 && getPixel(pe)==cDetect);
            while(++pw.X()!=pe.X())
            {
                plot(pw,color);
                //Down
                if(pw.Y()<(int)m_height-1  && getPixel(pw.X(),pw.Y()+1)==cDetect)
                    vs.push(Vec2i(pw.X(), pw.Y()+1));
                //Up
                if(pw.Y()>0 && getPixel(pw.X(),pw.Y()-1)==cDetect)
                    vs.push(Vec2i(pw.X(), pw.Y()-1));
                //--Diagonales--//
                if(con8)
                {
                    //DownLeft
                    if(pw.Y()<(int)m_height-1 && pw.X()>0 && getPixel(pw.X()-1, pw.Y()+1)==cDetect)
                        vs.push(Vec2i(pw.X()-1, pw.Y()+1));
                    //DownRight
                    if(pw.Y()<(int)m_height-1 && pw.X()<(int)m_width-1 && getPixel(pw.X()+1, pw.Y()+1)==cDetect)
                        vs.push(Vec2i(pw.X()+1, pw.Y()+1));
                    //UpLeft
                    if(pw.Y()>0 && pw.X()>0 && getPixel(pw.X()-1, pw.Y()-1)==cDetect)
                        vs.push(Vec2i(pw.X()+1, pw.Y()-1));
                    //UpRight
                    if(pw.Y()>0 && pw.X()<(int)m_width-1 && getPixel(pw.X()+1, pw.Y()-1)==cDetect)
                        vs.push(Vec2i(pw.X()+1, pw.Y()-1));
                }
            }
        }
    }
}

bool Image2Color::project_fill_on_other(const Vec2i& origin, Image2Grey& img_out, unsigned char out_color, bool con8)
{
        //TODO: con8
    if(out_color==0)
        return false;
    if(img_out.getPixel(origin)>=out_color)
        return true; //already filled, and with the correct color
    Color cDetect=getPixel(origin);
    Vec2i pw, pe, p(origin);
    std::stack<Vec2i> vs;
    vs.push(p);
    while(!vs.empty())
    {
        p=vs.top();
        vs.pop();
        if(img_out.getPixel(p)==0 && getPixel(p)==cDetect)
        {//resp. not filled and matches the detection color
            pw=p;
            pe=p;
            while(pw.X()-->0 && getPixel(pw)==cDetect);
            while(pe.X()++<(int)m_width-1 && getPixel(pe)==cDetect);
            while(++pw.X()!=pe.X())
            {
                img_out.setPixel(out_color, pw);
                //Down
                if(pw.Y()<(int)m_height-1  && getPixel(pw.X(),pw.Y()+1)==cDetect)
                    vs.push(Vec2i(pw.X(), pw.Y()+1));
                //Up
                if(pw.Y()>0 && getPixel(pw.X(),pw.Y()-1)==cDetect)
                    vs.push(Vec2i(pw.X(), pw.Y()-1));
                //--Diagonales--//
                if(con8)
                {
                    //DownLeft
                    if(pw.Y()<(int)m_height-1 && pw.X()>0 && getPixel(pw.X()-1, pw.Y()+1)==cDetect)
                        vs.push(Vec2i(pw.X()-1, pw.Y()+1));
                    //DownRight
                    if(pw.Y()<(int)m_height-1 && pw.X()<(int)m_width-1 && getPixel(pw.X()+1, pw.Y()+1)==cDetect)
                        vs.push(Vec2i(pw.X()+1, pw.Y()+1));
                    //UpLeft
                    if(pw.Y()>0 && pw.X()>0 && getPixel(pw.X()-1, pw.Y()-1)==cDetect)
                        vs.push(Vec2i(pw.X()+1, pw.Y()-1));
                    //UpRight
                    if(pw.Y()>0 && pw.X()<(int)m_width-1 && getPixel(pw.X()+1, pw.Y()-1)==cDetect)
                        vs.push(Vec2i(pw.X()+1, pw.Y()-1));
                }
            }
        }
    }
    return true;
}

//Analyse//


void Image2Color::project_toGreyScale(Image2Grey& imgGrey) const
{
    const_iterator itEnd=end();
    Image2Grey::iterator itGrey=imgGrey.begin();
    for(const_iterator it=begin(); it!=itEnd; ++it, ++itGrey)
    {
        (*itGrey)=((*it).Ruc()+(*it).Guc()+(*it).Buc())/3;
    }
}

void Image2Color::project_toBinary(Image2Grey &imgGrey, unsigned char seuil) const
{
    const_iterator itEnd=end();
    Image2Grey::iterator itGrey=imgGrey.begin();
    for(const_iterator it=begin(); it!=itEnd; ++it, ++itGrey)
    {
        (*itGrey)=(((*it).Ruc()+(*it).Guc()+(*it).Buc())/3)>seuil ? 255 : 0;
    }
}

int Image2Color::project_showCon(Image2Grey &img_out, bool con8, unsigned char step)
{
    unsigned char lvl=255; //< min level found
    Color cDetect=getPixel(0,0); //< this will be the background
    unsigned char conLvlColor=255; //< this is the lvl of the connexity depth. 255 is the background.
    Vec2i pAround(0,0);

    //First let's run around and fill the background
    while(pAround.X()<(int)m_width-1)
    {
        if(getPixel(pAround)==cDetect)
            project_fill_on_other(pAround, img_out, conLvlColor, con8);
        pAround.X()++;
    }
    while(pAround.Y()<(int)m_height-1)
    {
        if(getPixel(pAround)==cDetect)
            project_fill_on_other(pAround, img_out, conLvlColor, con8);
        pAround.Y()++;
    }
    while(pAround.X()>0)
    {
        if(getPixel(pAround)==cDetect)
            project_fill_on_other(pAround, img_out, conLvlColor, con8);
        pAround.X()--;
    }
    while(pAround.Y()>0)
    {
        if(getPixel(pAround)==cDetect)
            project_fill_on_other(pAround, img_out, conLvlColor, con8);
        pAround.Y()--;
    }
    //TODO: create an iterator that goes back and forth before going to the next line
    //Seriously though I'm not going to have the time to do this
    Image2Grey::iterator itEnd=img_out.end();
    for(Image2Grey::iterator it=img_out.begin(); it!=itEnd; ++it)
    {
        if((*it)==0)//place where we haven't filled
        {
            conLvlColor-=step;//drop the lvl down and fill img_out with it
            lvl=std::min(conLvlColor, lvl);
            project_fill_on_other(it.Vertice(), img_out, conLvlColor, con8);
        }
        conLvlColor=(*it);
    }
    return (int)(255-lvl)/step;
}
