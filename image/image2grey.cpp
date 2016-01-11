#include "image2grey.h"

#define _spc_output_ << ' ' <<

//====================================================================================================================================
//Constructeurs//
Image2Grey::Image2Grey() : Image2<unsigned char>()
{

}

Image2Grey::Image2Grey(const Image2Grey &other) : Image2<unsigned char>(other)
{

}

Image2Grey::Image2Grey(size_t width, size_t height) : Image2<unsigned char>(width, height)
{

}

Image2Grey::Image2Grey(const std::string &filename) : Image2<unsigned char>()
{
    load(filename);
}

//====================================================================================================================================

void Image2Grey::load(const std::string &filename)
{
    //old. please ignore!
    std::ifstream ifs(filename.c_str());
    if(!ifs.is_open())
        return;
    int dummy; //fetch eventual number
    char c_dummy; //fetch eventual character
    ifs >> c_dummy;
    if(c_dummy!='P')
        return;
    ifs >> dummy;
    if(dummy!=2) //Image pas en niveau de gris
    {
        std::cerr << "Error: " << filename << " isn't a grey scale pgm image." << std::endl;
        return;
    }
    ifs >> m_width;
    ifs >> m_height;
    unsigned int format;
    ifs >> format;
    if(format==255)
    {
        size_t size=m_width*m_height;
        m_tab=new unsigned char[size];
        for(iterator it=begin(); it!=end() && ifs.good(); ++it)
        {
            ifs >> dummy;
            *it=(unsigned char)dummy;
        }
        ifs.close();
    }
}

void Image2Grey::save(const std::string &filename) const
{
    int j=10;
    std::ofstream ofs(filename.c_str());
    if(!ofs.is_open())
        return;
    ofs << "P2" << std::endl;
    ofs << m_width _spc_output_ m_height << std::endl;
    ofs << 255;
    for(Image2Grey::const_iterator it=begin();it!=end();++it)
    {
        if(j++>=10) //le format accepte un maximum de 70 char/ligne, on se limitera à 10 nombres
        {
            j=0;
            ofs << std::endl;
        }
        else
            ofs << ' ';
        ofs << ((int)(*it));
    }
    return;
}

void Image2Grey::project_erode_black(Image2Grey& img_out, bool con8) const
{
    //make sure img and img out have the same size
    const_iterator itEnd=end();
    iterator it_out=img_out.begin();
    for(const_iterator it=begin(); it!=itEnd; ++it, ++it_out)
    {
        unsigned char color;
        int kx=0, ky=0;
        bool stop=false;
        for(int x=it.Vertice().X()-1; !stop && x<=it.Vertice().X()+1; ++x)
        {
            for(int y=it.Vertice().Y()-1; !stop && y<=it.Vertice().Y()+1; ++y)
            {
                int k=kx+(3*ky);
                if(con8)
                    color=getPixel((unsigned int)x, (unsigned int)y)*GradientSobel::SobelErode4[k];
                else
                    color=getPixel((unsigned int)x, (unsigned int)y)*GradientSobel::SobelErode8[k];
                if(color!=0)
                {
                    (*it_out)=255;
                    stop=true;
                }
                else
                    ++ky;
            }
        }
        ++kx;
    }
}

#undef _spc_output_
