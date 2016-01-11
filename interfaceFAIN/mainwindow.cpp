#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setVisible(false);
    ui->widget->setVisible(false);
    m_render = new RenderImg(this);
    ui->horizontalLayout->insertWidget(0,m_render,99);
    m_render->setFocusPolicy(Qt::ClickFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

#define DVALUE_TO_INT (100.0L)

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if(!m_stopSignal)
    {
        m_stopSignal=true;
        ui->doubleSpinBox->setValue((value/DVALUE_TO_INT));
    }
    m_stopSignal=false;
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    if(!m_stopSignal)
    {
        m_stopSignal=true;
        ui->horizontalSlider->setValue((int)(arg1*DVALUE_TO_INT));
    }
    m_stopSignal=false;
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    if(!m_stopSignal)
    {
        m_stopSignal=true;
        ui->doubleSpinBox_2->setValue((value/DVALUE_TO_INT));
    }
    m_stopSignal=false;
}


void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    if(!m_stopSignal)
    {
        m_stopSignal=true;
        ui->horizontalSlider_2->setValue((int)(arg1*DVALUE_TO_INT));
    }
    m_stopSignal=false;
}

#undef DVALUE_TO_INT

void MainWindow::on_pushButton_2_clicked()
{
    //Apply scale
    m_render->polygon_scale(ui->doubleSpinBox->value(), ui->doubleSpinBox_2->value());
}

void MainWindow::on_pushButton_clicked()
{
    //Apply rotation
    m_render->polygon_rotate();
}

void MainWindow::on_actionPolygon_Mode_toggled(bool arg1)
{
    ui->widget->setVisible(arg1);
    m_render->polygon_toggled(arg1);
}

void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                    QString("Load file"), QDir::currentPath(),
                                    QString("any pixmap file (*.ppm *.pgm *.pbm);;all (*.*)"));
    if (!fileName.isEmpty())
    {
        std::string strFN = fileName.toStdString();
        //load texture
        m_render->load(strFN);
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                    QString("Save file"), QDir::currentPath(),
                                    QString("ppm file (*.ppm);;all (*.*)"));
    if (!fileName.isEmpty())
    {
        std::string strFN = fileName.toStdString();
        //save texture
        m_render->save(strFN);
    }
}

void MainWindow::on_actionClear_triggered()
{
    m_render->clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    //Compute informations
    m_polygonInformations=m_render->polygon_informations();
    ui->textEdit->setText(QString(m_polygonInformations.c_str()));
}

void MainWindow::on_pushButton_4_clicked()
{
    //Compute connexity
    ui->label_con->setText(QString::number(m_render->analysis_showCon()));
}

void MainWindow::on_actionAnalysis_Mode_toggled(bool arg1)
{
    ui->widget_2->setVisible(arg1);
    m_render->analysis_toggled(arg1);
}

void MainWindow::on_pushButton_5_clicked()
{
    //Explicit shapes
    m_render->analysis_explicitShapes();
}

void MainWindow::on_pushButton_reset_clicked()
{
    //Reset polygon
    m_render->polygon_clear();
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    //Slider seuil
    m_render->analysis_seuilChanged((unsigned char)value);
}

void MainWindow::on_pushButton_6_clicked()
{
    //Erode black
    m_render->analysis_erodeBlack();
}
