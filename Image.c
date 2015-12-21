/*====================================================*\
  Vendredi 8 novembre 2013
  Arash HABIBI
  Image.c
\*====================================================*/

#include "Image.h"

//------------------------------------------------------------------------

Color C_new(float red, float green, float blue)
{
	Color c;
	c._red = red;
	c._green = green;
	c._blue = blue;
	return c;
}

Color C_rand()
{
    static Color c_black={0.,0.,0.};
    static Color c_white={1.,1.,1.};
    Color c;
    label:
    c._red=rand()/(float)RAND_MAX;
    c._green=rand()/(float)RAND_MAX;
    c._blue=rand()/(float)RAND_MAX;
    if(C_equals(&c, &c_black) || C_equals(&c, &c_white))
        goto label;
    return c;
}

//------------------------------------------------------------------------

void C_check(Color c, char *message)
{
	fprintf(stderr,"%s : %f %f %f\n",message,c._red,c._green,c._blue);
}

//------------------------------------------------------------------------

bool C_equals(Color *c1, Color *c2)
{
    return ( (c1->_red-c2->_red) < _e_ && (c1->_red-c2->_red) > -_e_) &&
            ((c1->_green-c2->_green) < _e_ && (c1->_green-c2->_green) > -_e_) &&
            ((c1->_blue-c2->_blue) < _e_ && (c1->_blue-c2->_blue) > -_e_) ;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

Image* I_new(int width, int height)
{
	Image *img_new = (Image*)malloc(sizeof(Image));
	img_new->_width = width;
	img_new->_height = height;
	img_new->_xzoom = 0;
	img_new->_yzoom = 0;
	img_new->_zoom = 1.0;

	img_new->_xoffset=0;
	img_new->_yoffset=0;

	img_new->_current_color = C_new(255,255,255);

	img_new->_buffer = (Color**)calloc(width,sizeof(Color*));

	int x;
	for(x=0;x<width;x++)
		img_new->_buffer[x] = (Color*)calloc(height,sizeof(Color));

	return img_new;
}

//------------------------------------------------------------------------

static void _plot(Image *img, int x, int y, Color c)
{
	img->_buffer[x][y] = c;
}

//-----

static int _isPpm(char *imagefilename)
{
	FILE *imagefile;
	imagefile = fopen(imagefilename,"r");
	if(imagefile==NULL) {perror(imagefilename); exit(1); }

	else
	{
		int c1 = fgetc(imagefile);
		int c2 = fgetc(imagefile);
		fclose(imagefile);

		if((c1=='P')&&(c2=='6'))	return 1;
		else						return 0;
	}
}

//-----

Image* I_read(char *imagefilename)
{
	Image *img;
	char command[100];

	if(_isPpm(imagefilename))	sprintf(command,"cp %s input.ppm",imagefilename);
	else					sprintf(command,"convert %s input.ppm",imagefilename);

	int stat = system(command);
	if(stat!=0)
	{
		fprintf(stderr,"Convert : %s -> input.ppm impossible conversion.\n", imagefilename);
		exit(1);
	}
	else
	{
		Ppm ppm = PPM_nouv("input.ppm", PPM_LECTURE);
		system("rm input.ppm");

		fprintf(stderr,"%d x %d\n",PPM_largeur(ppm),PPM_hauteur(ppm));

		if(ppm!=NULL)
		{
			img = I_new(PPM_largeur(ppm),PPM_hauteur(ppm));
			int nb_bits=ppm->_nb_bits;
			int valmax = ppm->_valmax;

			int nb_pixels = img->_width*img->_height;

			if(nb_bits <= 8)
			{
				unsigned char *donnees = (unsigned char*)calloc(3*nb_pixels,sizeof(unsigned char));
				PPM_lectureDonneesChar(ppm, donnees);

				int x,y;
				for(y=0;y<img->_height;y++)
					for(x=0;x<img->_width;x++)
					{
						int indice = (img->_height-y)*img->_width + x;
						Color c = C_new((1.0*donnees[3*indice  ])/valmax,
										(1.0*donnees[3*indice+1])/valmax,
										(1.0*donnees[3*indice+2])/valmax);
						_plot(img,x,y,c);
					}
			}
			else
			{
				unsigned short *donnees = (unsigned short*)calloc(3*nb_pixels,sizeof(unsigned short));
				PPM_lectureDonneesShort(ppm, donnees);
				int x,y;
				for(y=0;y<img->_height;y++)
					for(x=0;x<img->_width;x++)
					{
						int indice = (img->_height-y)*img->_width + x;
						Color c = C_new((1.0*donnees[3*indice  ])/valmax,
										(1.0*donnees[3*indice+1])/valmax,
										(1.0*donnees[3*indice+2])/valmax);
						img->_buffer[x][y] = c;
					}
			}
			PPM_fermeture(ppm);
			return(img);
		}
		else
			return(NULL);
	}
}

//------------------------------------------------------------------------

void I_fill(Image *img, Color c)
{
	int x,y;
	for(x=0;x<img->_width;x++)
		for(y=0;y<img->_height;y++)
			img->_buffer[x][y]=c;
}

//------------------------------------------------------------------------

void I_checker(Image *img, Color c1, Color c2, int step)
{
	int x,y;
	for(x=0;x<img->_width;x++)
		for(y=0;y<img->_height;y++)
		{
			int n_x = x/step;
			int n_y = y/step;
			if((n_x+n_y)%2==0)	_plot(img,x,y,c1);
			else				_plot(img,x,y,c2);
		}
}

//------------------------------------------------------------------------

void I_changeColor(Image *img, Color c)
{
	img->_current_color = c;
}

//------------------------------------------------------------------------

void I_plot(Image *img, int x, int y)
{
	if((x>=0)&&(x<img->_width)&&
	   (y>=0)&&(y<img->_height))
		img->_buffer[x][y] = img->_current_color;
	else
	{
		fprintf(stderr,"I_plot : ERROR !!!\n");
		fprintf(stderr,"x (=%d) must be in the [%d,%d] range and\n", x, 0, img->_width);
		fprintf(stderr,"y (=%d) must be in the [%d,%d] range\n", y, 0, img->_height);
	}
}

//------------------------------------------------------------------------

void I_plotColor(Image *img, int x, int y, Color c)
{
	if((x>=0)&&(x<img->_width)&&
	   (y>=0)&&(y<img->_height))
		img->_buffer[x][y] = c;
	else
	{
		fprintf(stderr,"I_plotColor : ERROR !!!\n");
		fprintf(stderr,"x (=%d) must be in the [%d,%d] range and\n", x, 0, img->_width);
		fprintf(stderr,"y (=%d) must be in the [%d,%d] range\n", y, 0, img->_height);
	}
}

//------------------------------------------------------------------------
// Changement de repère

static void _windowToImage(Image *img, int xwin, int ywin, int *ximg, int *yimg)
{

	*ximg = img->_xoffset + img->_xzoom + (xwin-img->_xzoom) / img->_zoom;
	*yimg = img->_yoffset + img->_yzoom + (ywin-img->_yzoom) / img->_zoom;
}

//-----
// Changement de repère inverse
/*
static void _imageToWindow(Image *img, int ximg, int yimg, int *xwin, int *ywin)
{

	*xwin = img->_xoffset + img->_xzoom + (ximg-img->_xzoom-img->_xoffset) * img->_zoom;
	*ywin = img->_yoffset + img->_yzoom + (yimg-img->_yzoom-img->_yoffset) * img->_zoom;
}
*/
//-----

void I_focusPoint(Image *img, int xwin, int ywin)
{
	int dx = xwin - img->_xzoom;
	int dy = ywin - img->_yzoom;
	img->_xoffset -= dx*(1-1.0/img->_zoom);
	img->_yoffset -= dy*(1-1.0/img->_zoom);
	img->_xzoom = xwin;
	img->_yzoom = ywin;
}

//------------------------------------------------------------------------

void I_zoomInit(Image *img)
{
	img->_xoffset = 0;
	img->_yoffset = 0;
	img->_zoom = 1.0;
}

//------------------------------------------------------------------------

void I_zoom(Image *img, double zoom_coef)
{
	img->_zoom = img->_zoom * zoom_coef;
}

//------------------------------------------------------------------------

void I_move(Image *img, int x, int y)
{
	img->_xoffset += x;
	img->_yoffset += y;
}

//------------------------------------------------------------------------

void I_draw(Image *img)
{
	glBegin(GL_POINTS);
	int xwin, ywin, ximg, yimg;
	for(xwin=0;xwin<img->_width;xwin++)
		for(ywin=0;ywin<img->_height;ywin++)
		{
			_windowToImage(img, xwin, ywin, &ximg, &yimg);
			Color c;
			if((ximg>=0)&&(ximg<img->_width)&&
			   (yimg>=0)&&(yimg<img->_height))
				c = img->_buffer[ximg][yimg];
			else
				c = C_new(0,0,0);

			glColor3f(c._red,c._green,c._blue);
			glVertex2i(xwin,ywin);
		}
	glEnd();
}

//------------------------------------------------------------------------
//TODO

void I_ADNOrigin(Image *img, int x, int y)
//ADN premier octant
{
    int xMoy;
    float yMoy=0.5, yInc;
    yInc=(float)(y/(float)x);
    for(xMoy=0; xMoy<=x; xMoy++)
    {
        I_plot(img, xMoy, (int)yMoy);
        yMoy+=yInc;
    }
}

void I_bresenham(Image *img, int xA, int yA, int xB, int yB)
{
    int i, C, x, y, Dx, Dy, Dx2, Dy2, incX, incY;
    Dx=abs(xB-xA);
    Dy=abs(yB-yA);
    Dx2=2*Dx;
    Dy2=2*Dy;
    incX= xB>xA ? 1 : -1; //xB à droite de xA ?
    incY= yB>yA ? 1 : -1; //yB en haut de yA ?
    x=xA; y=yA;
    I_plot(img, x, y);
    if(Dx>Dy) //premiers octants ? (cos>sin)
    {
        C=Dy2-Dx;
        for(i=0; i<Dx; i++)
        {
            if(C<0)
                C+=Dy2;
            else
            {
                y+=incY;
                C=Dy2-Dx2;
            }
            x+=incX;
            I_plot(img, x,y);
        }
    }
    else
    {
        C=Dx2-Dy;
        for(i=0; i<Dy; i++)
        {
            if(C<0)
                C+=Dx2;
            else
            {
                x+=incX;
                C=Dx2-Dy2;
            }
            y+=incY;
            I_plot(img, x,y);
        }
    }
}

Point P_new(int x, int y)
{
    Point p;
    p.x=x;
    p.y=y;
    return p;
}

VerticeStack *VS_new(int size)
{
    VerticeStack *vs=(VerticeStack *)malloc(sizeof(VerticeStack));
    vs->stack=(Point *)malloc(size*sizeof(Point));
    vs->size=size;
    vs->headIndex=-1;
    return vs;
}

Point VS_top(VerticeStack *vs)
{
    return vs->stack[vs->headIndex];
}

bool VS_empty(VerticeStack *vs)
{
    return vs->headIndex<0;
}

void VS_pop(VerticeStack *vs)
{
    vs->headIndex--;
}

void VS_push(VerticeStack *vs, Point p)
{
    if(++vs->headIndex >= vs->size)
    {
        vs->size=2*vs->size;
        vs->stack=realloc(vs->stack, vs->size*sizeof(p));
    }
    vs->stack[vs->headIndex]=p;
}

void VS_destroy(VerticeStack *vs)
{
    free(vs->stack);
    free(vs);
}

void I_remplissage4(Image *img_in, Image *img_out, int x_germe, int y_germe)
{
    Color c_black={0,0,0};
    Color c_white={1.,1.,1.};
    Color c_in=img_in->_buffer[x_germe][y_germe];
    I_fill(img_out, c_black);
    VerticeStack *vs=VS_new(img_in->_height*img_in->_width);
    Point p;
    VS_push(vs, p=P_new(x_germe, y_germe));
    fprintf(stderr,"starting to fill\n");
    I_plotColor(img_out, p.x, p.y, c_white);
    while(!VS_empty(vs))
    {
        p=VS_top(vs);
        VS_pop(vs);
        if(p.x<img_in->_width-1)
        {
            if(C_equals(&img_in->_buffer[++p.x][p.y],&c_in))
            {
                VS_push(vs, p);
                I_plotColor(img_out, p.x--, p.y, c_white);
            }
        }
        if(p.x>0)
        {
            if(C_equals(&img_in->_buffer[--p.x][p.y],&c_in))
            {
                VS_push(vs, p);
                I_plotColor(img_out, p.x++, p.y, c_white);
            }
        }
        if(p.y<img_in->_height-1)
        {
            if(C_equals(&img_in->_buffer[p.x][++p.y],&c_in))
            {
                VS_push(vs, p);
                I_plotColor(img_out, p.x, p.y--, c_white);
            }
        }
        if(p.y>0)
        {
            if(C_equals(&img_in->_buffer[p.x][--p.y],&c_in))
            {
                VS_push(vs, p);
                I_plotColor(img_out, p.x, p.y++, c_white);
            }
        }
    }
}

void I_remplissage8(Image *img_in, Image *img_out, int x_germe, int y_germe)
{

}

void _showConFill(Image *img, Point *pBalaye, VerticeStack *vs, Color *cDetect, Color *cFill, bool con8)
{
    Point pw, pe, p;
    VS_push(vs, p=P_new(pBalaye->x, pBalaye->y));
    while(!VS_empty(vs))
    {
        p=VS_top(vs);
        VS_pop(vs);
        if(C_equals(&img->_buffer[p.x][p.y],cDetect))
        {
            pw=p;
            pe=p;
            while(pw.x>0 && C_equals(&img->_buffer[--pw.x][pw.y],cDetect));
            while(pe.x<img->_width-1 && C_equals(&img->_buffer[++pe.x][pe.y],cDetect));
            while(++pw.x!=pe.x)
            {
                I_plotColor(img,pw.x,pw.y,*cFill);
                //Down
                if(pw.y<img->_height-1  && C_equals(&img->_buffer[pw.x][pw.y+1], cDetect))
                    VS_push(vs, P_new(pw.x, pw.y+1));
                //Up
                if(pw.y>0 && C_equals(&img->_buffer[pw.x][pw.y-1], cDetect))
                    VS_push(vs, P_new(pw.x, pw.y-1));
                //--Diagonales--//
                if(con8)
                {
                    //DownLeft
                    if(pw.y<img->_height-1  && pw.x>0 && C_equals(&img->_buffer[pw.x-1][pw.y+1], cDetect))
                        VS_push(vs, P_new(pw.x-1, pw.y+1));
                    //DownRight
                    if(pw.y<img->_height-1  && pw.x<img->_width-1 && C_equals(&img->_buffer[pw.x+1][pw.y+1], cDetect))
                        VS_push(vs, P_new(pw.x+1, pw.y+1));
                    //UpLeft
                    if(pw.y>0 && pw.x>0 && C_equals(&img->_buffer[pw.x-1][pw.y-1], cDetect))
                        VS_push(vs, P_new(pw.x+1, pw.y-1));
                    //UpRight
                    if(pw.y>0 && pw.x<img->_width-1 && C_equals(&img->_buffer[pw.x+1][pw.y-1], cDetect))
                        VS_push(vs, P_new(pw.x+1, pw.y-1));
                }
            }
        }
    }
}

void I_showCon8(Image *img)
{
    Color c_black={0,0,0};
    Color c_white={1.,1.,1.};
    Color cRand;
    VerticeStack *vs=VS_new(img->_height*img->_width);
    Point pBalaye=P_new(0,0);
    while(pBalaye.y<img->_height)
    {
        if(C_equals(&img->_buffer[pBalaye.x][pBalaye.y],&c_black))
        {
            cRand=C_rand();
            _showConFill(img, &pBalaye, vs, &c_black, &cRand, true);
        }
        if(++pBalaye.x==img->_width)
        {
            pBalaye.x=0;
            pBalaye.y++;
        }
    }
    VS_destroy(vs);
}

void I_showCon4(Image *img)
{//TODO : détection extérieur
    bool region_done=false;
    Color c_white={1.,1.,1.};
    Color c_black={0.,0.,0.};
    Color cRand=C_rand();//color used for the border componant
    VerticeStack *vs=VS_new(img->_height*img->_width);
    Point pBalaye=P_new(0,0), pAround=P_new(0,0);
    //First let's run around and fill the white border
    while(pAround.x<img->_width-1)
    {
        if(!region_done && C_equals(&img->_buffer[pAround.x][pAround.y],&c_white))
        {
            _showConFill(img, &pAround, vs, &c_white, &cRand, false);
            region_done=true;
        }
        else if(region_done && C_equals(&img->_buffer[pAround.x][pAround.y],&c_black))
            region_done=false;
        pAround.x++;
    }
    while(pAround.y<img->_height-1)
    {
        if(!region_done && C_equals(&img->_buffer[pAround.x][pAround.y],&c_white))
        {
            _showConFill(img, &pAround, vs, &c_white, &cRand, false);
            region_done=true;
        }
        else if(region_done && C_equals(&img->_buffer[pAround.x][pAround.y],&c_black))
            region_done=false;
        pAround.y++;
    }
    while(pAround.x>0)
    {
        if(!region_done && C_equals(&img->_buffer[pAround.x][pAround.y],&c_white))
        {
            _showConFill(img, &pAround, vs, &c_white, &cRand, false);
            region_done=true;
        }
        else if(region_done && C_equals(&img->_buffer[pAround.x][pAround.y],&c_black))
            region_done=false;
        pAround.x--;
    }
    while(pAround.y>0)
    {
        if(!region_done && C_equals(&img->_buffer[pAround.x][pAround.y],&c_white))
        {
            _showConFill(img, &pAround, vs, &c_white, &cRand, false);
            region_done=true;
        }
        else if(region_done && C_equals(&img->_buffer[pAround.x][pAround.y],&c_black))
            region_done=false;
        pAround.y--;
    }
    while(pBalaye.y<img->_height)
    {
        if(C_equals(&img->_buffer[pBalaye.x][pBalaye.y],&c_white))
        {
            cRand=C_rand();
            _showConFill(img, &pBalaye, vs, &c_white, &cRand, false);
        }
        if(++pBalaye.x==img->_width)
        {
            pBalaye.x=0;
            pBalaye.y++;
        }
    }
    VS_destroy(vs);
}

