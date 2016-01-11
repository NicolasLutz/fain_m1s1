#ifndef RENDERIMG_H
#define RENDERIMG_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QColorDialog>
#include <iostream>
#include <sstream>

#include "vec2i.h"
#include "image2grey.h"
#include "image2color.h"
#include "imgGradient.h"
#include "polygon.h"
#define BASE_H 512
#define BASE_W 512

typedef enum {RGBA_I, UNSIGNED_BYTE_I, BIT_I, CONNEX_I, ERODE_I} ImageType_t;

class RenderImg : public QGLWidget
{
    Q_OBJECT

public:
    RenderImg(QWidget *parent = 0);
    virtual ~RenderImg();

    //MainWindow//
    void load(const std::string& filename);
    void save(const std::string& filename) const;
    void clear();

    //Updates//

    void updateDataTexture();

protected:
    /// Qt callbacks
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    /// transform win coordinate in texel image coordinate
    void coordInTexture(QMouseEvent *event, int& x, int& y);
    void coordInTexture(int& x, int& y);

    // convert from image coordinate to GL coordinates (x)
    inline float xImg2GL(int x)
    {
        return -1.0f + (2.0f*x)/float(m_widthTex-1);
    }

    // convert from image coordinate to GL coordinates (y)
    inline float yImg2GL(int y)
    {
        return 1.0f - (2.0f*y)/float(m_heightTex-1);
    }

    /// Texture information
    GLuint m_texture;
    int m_widthTex;
    int m_heightTex;
    void* m_ptrTex;

    ///Qt
    QColorDialog *m_colorPicker;

    QMenu *m_menu;

    QAction *m_colorPick;
    QAction *m_checker;
    QAction *m_brsh_line;
    QAction *m_brsh_circle;
    QAction *m_fillRec;
    QAction *m_fillRec_balaye;

    QAction *m_actionColorImg;
    QAction *m_actionGreyImg;
    QAction *m_actionBinaryImg;

    /// GL Widget sizes
    int m_winW;
    int m_winH;

    /// for mouse move
    Vec2i m_lastPressedPos;
    Vec2i m_lastReleasedPos;

    /// key modifier state (shift/ctrl/...)
    int m_state_modifier;

    Image2Color m_colorImg;
    Image2Grey m_greyImg;
    Image2Grey m_erodeImg;

    ImageType_t m_imageType;

    PolygonNi m_polygon;

    bool m_polygonMode;

    GradientSobel m_GS;

    unsigned char m_seuil;

public slots:
    void showContextMenu(const QPoint& pos);

    void colorPick();
    void checker();
    void brsh_line();
    void brsh_circle();
    void fillRec();
    void fillRec_balaye();

    void actionColorImg();
    void actionGreyImg();
    void actionBinaryImg();

    //Polygon//
    void polygon_draw();
    void polygon_add();
    void polygon_scale(float scaleX, float scaleY);
    void polygon_rotate();
    void polygon_toggled(bool arg1);
    std::string polygon_informations();
    void polygon_clear();

    //Analysis//
    void analysis_toggled(bool arg1);
    int analysis_showCon();
    void analysis_explicitShapes();
    void analysis_erodeBlack();
    void analysis_seuilChanged(unsigned char seuil);
};

#endif // RENDERIMG_H
