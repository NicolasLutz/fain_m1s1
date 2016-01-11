#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QKeyEvent>
#include <QFileDialog>

#include "renderimg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_actionPolygon_Mode_toggled(bool arg1);

    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_actionClear_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_actionAnalysis_Mode_toggled(bool arg1);

    void on_pushButton_5_clicked();

    void on_pushButton_reset_clicked();

    void on_horizontalSlider_3_valueChanged(int value);

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    RenderImg *m_render;

    bool m_stopSignal;
    std::string m_polygonInformations;
};

#endif // MAINWINDOW_H
