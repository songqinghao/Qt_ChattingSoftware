#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initWidgets();
}

void MainWindow::initWidgets()
{
    m_paintArea = new PaintArea;
    //形状
    m_shapeLabel = new QLabel(tr("形状"));//tr表示可用于翻译

    m_shapeCombox = new QComboBox;
    m_shapeCombox->addItem(tr("Line"),PaintArea::Line);
    m_shapeCombox->addItem(tr("Rectangle"),PaintArea::Rectangle);
    m_shapeCombox->addItem(tr("RoundedRect"),PaintArea::RoundRect);
    m_shapeCombox->addItem(tr("Ellipse"),PaintArea::Ellipse);
    m_shapeCombox->addItem(tr("Polygon"),PaintArea::Polygon);
    m_shapeCombox->addItem(tr("Polyline"),PaintArea::Polyline);
    m_shapeCombox->addItem(tr("Points"),PaintArea::Points);
    m_shapeCombox->addItem(tr("Arc"),PaintArea::Arc);
    m_shapeCombox->addItem(tr("Path"),PaintArea::Path);
    m_shapeCombox->addItem(tr("Text"),PaintArea::Text);
    m_shapeCombox->addItem(tr("Pixmap"),PaintArea::Pixmap);
    connect(m_shapeCombox,SIGNAL(activated(int)),this,SLOT(ShowShape(int)));

    //画笔颜色
    m_penColorLabel = new QLabel(tr("画笔颜色"));
    m_penColorFrame = new QFrame;
    //设置边框风格
    m_penColorFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    m_penColorFrame->setAutoFillBackground(true);//设置填充背景
    m_penColorFrame->setPalette(QPalette(Qt::black));//初始化颜色
    m_penColorBtn = new QPushButton("更改");
    connect(m_penColorBtn,SIGNAL(clicked()),this,SLOT(ShowPenColor()));
}

void MainWindow::ShowShape(int value)
{

}

MainWindow::~MainWindow()
{
}

