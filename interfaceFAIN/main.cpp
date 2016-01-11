#include <QApplication>
#include "mainwindow.h"
#include "vec2i.h"
#include "GL/gl.h"

int main(int argc, char *argv[])
{
    Vec2i fixQt5LinkingSystemBecauseItsBrokenForNoDamnReasonWhatsoever;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
