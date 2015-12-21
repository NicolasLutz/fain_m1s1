/*====================================================*\
  Vendredi 8 novembre 2013
  Arash HABIBI
  Image.h
\*====================================================*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <GL/gl.h>

#include "Ppm.h"

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define _e_ (0.05)

//-----------------------------------------------------

typedef struct
{
	float _red, _green, _blue;
} Color;

Color C_new(float red, float green, float blue);
bool C_equals(Color *c1, Color *c2);
Color C_rand();
void C_check(Color c, char *message);

//-----------------------------------------------------

typedef struct
{
	int _width, _height;
	int _xzoom, _yzoom;
	int _xoffset, _yoffset;
	double _zoom;
	Color _current_color;
	Color **_buffer;
} Image;

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    Point *stack;
    int headIndex;
    int size;
} VerticeStack;

Image* I_new(int _width, int _height);
Image* I_read(char *ppmfilename);

void I_fill       (Image *img, Color c);
void I_checker    (Image *img, Color c1, Color c2, int step);

void I_changeColor(Image *img, Color c);
void I_plot       (Image *img, int x, int y);
void I_plotColor  (Image *img, int x, int y, Color c);

void I_focusPoint (Image *img, int x, int y);
void I_zoomInit   (Image *img);
void I_zoom       (Image *img, double zoom_coef);
void I_move       (Image *img, int x, int y);

void I_draw       (Image *img);

//TODO

void I_ADNOrigin(Image *img, int x, int y);
void I_bresenham(Image *img, int xA, int yA, int xB, int yB);

void I_remplissage4(Image *img_in, Image *img_out, int x_germe, int y_germe);
void I_remplissage8(Image *img_in, Image *img_out, int x_germe, int y_germe);

void I_showCon4(Image *img);
void I_showCon8(Image *img);

#endif



