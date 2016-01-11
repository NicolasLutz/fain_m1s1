#include "renderimg.h"

RenderImg::RenderImg(QWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
    m_texture(0),
    m_widthTex(BASE_W),
    m_heightTex(BASE_H),
    m_lastPressedPos(0,0),
    m_lastReleasedPos(0,0),
    m_colorImg(m_widthTex, m_heightTex),
    m_greyImg(m_widthTex, m_heightTex),
    m_erodeImg(), //not always used so...
    m_imageType(RGBA_I),
    m_polygonMode(false),
    m_GS(),
    m_seuil(128),
    m_useCon8(false)
{
    m_colorPicker=new QColorDialog(QColor(255,255,255), this);
    m_colorImg.changeColor(Color(1,1,1));

    setContextMenuPolicy(Qt::CustomContextMenu);
    this->connect(  this, SIGNAL(customContextMenuRequested(const QPoint&)),
                    this, SLOT(showContextMenu(const QPoint&)));

    ///Menu
    m_menu=new QMenu();
    m_colorPick=m_menu->addAction("Change color...");
    m_menu->addSeparator();
    m_checker=m_menu->addAction("Draw checker");
    m_brsh_line=m_menu->addAction("Draw line");
    m_brsh_circle=m_menu->addAction("Draw circle");
    m_fillRec=m_menu->addAction("Fill (recursive)");
    m_fillRec_balaye=m_menu->addAction("Fill (balaye)");
    m_menu->addSeparator();
    m_actionColorImg=m_menu->addAction("Switch to Color image");
    m_actionGreyImg=m_menu->addAction("Switch to Grey image");
    m_actionBinaryImg=m_menu->addAction("Switch to Binary image");

    ///Actions
    this->connect(  m_colorPick, SIGNAL(triggered()),
                    this, SLOT(colorPick()));
    this->connect(  m_checker, SIGNAL(triggered()),
                    this, SLOT(checker()));
    this->connect(  m_brsh_line, SIGNAL(triggered()),
                    this, SLOT(brsh_line()));
    this->connect(  m_brsh_circle, SIGNAL(triggered()),
                    this, SLOT(brsh_circle()));
    this->connect(  m_fillRec, SIGNAL(triggered()),
                    this, SLOT(fillRec()));
    this->connect(  m_fillRec_balaye, SIGNAL(triggered()),
                    this, SLOT(fillRec_balaye()));
    this->connect(  m_actionColorImg, SIGNAL(triggered()),
                    this, SLOT(actionColorImg()));
    this->connect(  m_actionGreyImg, SIGNAL(triggered()),
                    this, SLOT(actionGreyImg()));
    this->connect(  m_actionBinaryImg, SIGNAL(triggered()),
                    this, SLOT(actionBinaryImg()));

    m_colorImg.clear();
    m_ptrTex=m_colorImg.getArray();
    updateDataTexture();
}


//MainWindow//

void RenderImg::load(const std::string& filename)
{
    m_colorImg.load(filename);
    m_greyImg.resize(m_colorImg.getWidth(), m_colorImg.getHeight());

    updateDataTexture();
}

void RenderImg::save(const std::string& filename) const
{
    m_colorImg.save(filename);
}

void RenderImg::clear()
{
    m_colorImg.clear();

    updateDataTexture();
}

//Updates//

void RenderImg::updateDataTexture()
{
    switch(m_imageType)
    {
        case RGBA_I:
            m_widthTex=(int)m_colorImg.getWidth();
            m_heightTex=(int)m_colorImg.getHeight();
            m_ptrTex=m_colorImg.getArray();
            glBindTexture(GL_TEXTURE_2D, m_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_widthTex, m_heightTex, 0, GL_RGBA, GL_FLOAT, m_ptrTex);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        case UNSIGNED_BYTE_I:
            m_colorImg.project_toGreyScale(m_greyImg);
            m_widthTex=(int)m_greyImg.getWidth();
            m_heightTex=(int)m_greyImg.getHeight();
            m_ptrTex=m_greyImg.getArray();
            glBindTexture(GL_TEXTURE_2D, m_texture);
            glTexSubImage2D(GL_TEXTURE_2D,0,0,0,m_widthTex, m_heightTex, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_ptrTex);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        case CONNEX_I:
            m_widthTex=(int)m_greyImg.getWidth();
            m_heightTex=(int)m_greyImg.getHeight();
            m_ptrTex=m_greyImg.getArray();
            glBindTexture(GL_TEXTURE_2D, m_texture);
            glTexSubImage2D(GL_TEXTURE_2D,0,0,0,m_widthTex, m_heightTex, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_ptrTex);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        case ERODE_I:
            m_widthTex=(int)m_erodeImg.getWidth();
            m_heightTex=(int)m_erodeImg.getHeight();
            m_ptrTex=m_erodeImg.getArray();
            glBindTexture(GL_TEXTURE_2D, m_texture);
            glTexSubImage2D(GL_TEXTURE_2D,0,0,0,m_widthTex, m_heightTex, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_ptrTex);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        default:
            m_colorImg.project_toBinary(m_greyImg, m_seuil);
            m_widthTex=(int)m_greyImg.getWidth();
            m_heightTex=(int)m_greyImg.getHeight();
            m_ptrTex=m_greyImg.getArray();
            glBindTexture(GL_TEXTURE_2D, m_texture);
            glTexSubImage2D(GL_TEXTURE_2D,0,0,0,m_widthTex, m_heightTex, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_ptrTex);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;
    }
    updateGL();
}


RenderImg::~RenderImg()
{
    glDeleteTextures(1, &m_texture);
    m_texture = 0;
}

void RenderImg::initializeGL()
{
    glClearColor(0.0f,0.0f,0.4f,0.0f);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glBindTexture(GL_TEXTURE_2D, 0);

    updateDataTexture();
}

void RenderImg::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();		// let's go 2D

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f( -1,1);
    glTexCoord2f(0,1);
    glVertex2f(-1,-1);
    glTexCoord2f(1,1);
    glVertex2f( 1,-1);
    glTexCoord2f(1,0);
    glVertex2f( 1,1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glColor3f(1.0f,0,0);
    glVertex2f(2.0f*m_lastPressedPos.X()/m_widthTex-1.0f, -2.0f*m_lastPressedPos.Y()/m_heightTex+1.0f);
    glColor3f(0.0f,1.0f,0);
    glVertex2f(2.0f*m_lastReleasedPos.X()/m_widthTex-1.0f, -2.0f*m_lastReleasedPos.Y()/m_heightTex+1.0f);
    glEnd();
}

void RenderImg::resizeGL(int width, int height)
{
    m_winW = width;
    m_winH = height;

    glViewport(0,0,width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    makeCurrent();
    updateGL();
}

void RenderImg::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
        return;
    int x,y;
    coordInTexture(event, x, y);
    m_lastPressedPos.setX(x);
    m_lastPressedPos.setY(y);

    if(m_polygonMode)
        polygon_add();

    updateDataTexture();
}

void RenderImg::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
        return;
    int x,y;
    coordInTexture(event, x, y);
    m_lastReleasedPos.setX(x);
    m_lastReleasedPos.setY(y);

    updateDataTexture();
}

void RenderImg::keyPressEvent(QKeyEvent* event)
{
    m_state_modifier = event->modifiers();
}

void RenderImg::keyReleaseEvent(QKeyEvent* event)
{
    m_state_modifier = 0;
}

void RenderImg::coordInTexture(QMouseEvent *event, int& x, int& y)
{
    x=(float(event->x())/m_winW)*m_widthTex;
    y=(float(event->y())/m_winH)*m_heightTex;
}

void RenderImg::coordInTexture(int& x, int& y)
{
    x=(float(x)/m_winW)*m_widthTex;
    y=(float(y)/m_winH)*m_heightTex;
}

void RenderImg::showContextMenu(const QPoint& pos)
{
    QPoint p = mapToGlobal(pos);
    m_menu->exec(p);
}

void RenderImg::colorPick()
{
    if(m_colorPicker->exec()==QDialog::Accepted)
    {
        QColor qColor=m_colorPicker->currentColor();
        m_colorImg.changeColor(Color(qColor.redF(), qColor.greenF(), qColor.blueF()));
    }
}

void RenderImg::checker()
{
    m_colorImg.genChecker(5, Color::White);

    updateDataTexture();
}

void RenderImg::brsh_line()
{
    m_colorImg.project_bresenhamLine(m_lastPressedPos, m_lastReleasedPos);

    updateDataTexture();
}

void RenderImg::brsh_circle()
{
    m_colorImg.project_bresenhamCircle(m_lastPressedPos, (int)m_lastPressedPos.distance(m_lastReleasedPos));

    updateDataTexture();
}

void RenderImg::fillRec()
{
    m_colorImg.project_fill_rec(m_lastPressedPos);

    updateDataTexture();
}

void RenderImg::fillRec_balaye()
{
    m_colorImg.project_fill_balaye(m_lastPressedPos);

    updateDataTexture();
}

void RenderImg::actionColorImg()
{
    m_imageType=RGBA_I;

    updateDataTexture();
}

void RenderImg::actionGreyImg()
{
    m_imageType=UNSIGNED_BYTE_I;

    updateDataTexture();
}

void RenderImg::actionBinaryImg()
{
    m_imageType=BIT_I;

    updateDataTexture();
}

//Polygon//

void RenderImg::polygon_draw()
{
    m_colorImg.clear();
    if(m_polygon.size()>=2)
    {
        Vec2i v1, v2;
        PolygonNi::const_iterator it=m_polygon.begin();
        v1=(*it).second;
        m_colorImg.setPixel(Color::White, v1);
        while(++it!=m_polygon.end())
        {
            v2=(*it).second;
            m_colorImg.project_bresenhamLine(v1, v2);
            v1=(*it).second;
            m_colorImg.setPixel(Color::White, v1);
        }
        v2=(*(m_polygon.begin())).second;
        m_colorImg.project_bresenhamLine(v1, v2);
    }

    updateDataTexture();
}

void RenderImg::polygon_add()
{
    m_polygon.insert(m_lastPressedPos);

    polygon_draw();
}

void RenderImg::polygon_scale(float scaleX, float scaleY)
{
    m_polygon.scale(scaleX, scaleY);

    polygon_draw();
}

void RenderImg::polygon_rotate()
{
    m_polygon.rotate90Left();

    polygon_draw();
}

void RenderImg::polygon_toggled(bool arg1)
{
    m_polygonMode=arg1;
    m_polygon.clear();

    polygon_draw();
}

std::string RenderImg::polygon_informations()
{
    m_polygon.computeDatas();
    std::ostringstream os;
    os << "This polygon is " << (m_polygon.isConvex() ? "convex" : "not convex") << std::endl;
    os << "It has " << m_polygon.size() << " vertices." << std::endl;
    os << "It is oriented " << (m_polygon.orientation() == LEFT_O ? "left" :
                               m_polygon.orientation() == RIGHT_O ? "right" :
                               "indescridibly") << '.' << std::endl;
    os << "The z coordinate of its not-normalized normal is " << m_polygon.normal() << '.' << std::endl;
    return os.str();
}

void RenderImg::polygon_clear()
{
    m_polygon.clear();

    polygon_draw();
}


//Analysis//

void RenderImg::analysis_toggled(bool arg1)
{
    //I don't know, let's say "do nothing"?
}

int RenderImg::analysis_showCon()
{
    m_greyImg.clear();
    int lvl=m_colorImg.project_showCon(m_greyImg, m_useCon8, 5);

    m_imageType=CONNEX_I;

    updateDataTexture();
    return lvl;
}

void RenderImg::analysis_explicitShapes()
{
    m_colorImg.project_toGreyScale(m_greyImg);
    m_GS(m_colorImg, m_greyImg, GRAD_33);

    m_imageType=UNSIGNED_BYTE_I;

    updateDataTexture();
}

void RenderImg::analysis_erodeBlack()
{
    if(m_imageType!=ERODE_I)
        m_colorImg.project_toBinary(m_greyImg, m_seuil);
    m_erodeImg.resize(m_greyImg.getWidth(), m_greyImg.getHeight());

    if(m_imageType!=ERODE_I)
        m_greyImg.project_erode_black(m_erodeImg, m_useCon8);
    else
    {
        Image2Grey newErode(m_erodeImg);
        newErode.project_erode_black(m_erodeImg, m_useCon8);
    }

    m_imageType=ERODE_I;

    updateDataTexture();
}

void RenderImg::analysis_dilateBlack()
{
    if(m_imageType!=ERODE_I)
        m_colorImg.project_toBinary(m_greyImg, m_seuil);
    m_erodeImg.resize(m_greyImg.getWidth(), m_greyImg.getHeight());

    if(m_imageType!=ERODE_I)
        m_greyImg.project_dilate_black(m_erodeImg, m_useCon8);
    else
    {
        Image2Grey newErode(m_erodeImg);
        newErode.project_dilate_black(m_erodeImg, m_useCon8);
    }

    m_imageType=ERODE_I;

    updateDataTexture();
}

void RenderImg::analysis_seuilChanged(unsigned char seuil)
{
    m_seuil=seuil;
    if(m_imageType==BIT_I)
        updateDataTexture();
}

//Autre//

void RenderImg::setCon8(bool checked)
{
    m_useCon8=checked;
}
