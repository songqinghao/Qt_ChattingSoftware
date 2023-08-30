#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QPixmap pixmap;
    pixmap.load(":/flower.png");
    //ui->label->resize(pixmap.size());
    resize(pixmap.size());
    //部件设置遮罩
    setMask(pixmap.mask());
    m_isPressed = false;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,QPixmap(":/flower.png"));
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    m_mousePoint = event->globalPos()-pos();
    QWidget::mousePressEvent(event);

}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    //只有左键才可以拖动
    if(m_isPressed==true&&event->buttons()&Qt::LeftButton)
    {
        move(event->globalPos()-m_mousePoint);
        m_mousePoint = event->globalPos()-pos();
        QWidget::mouseMoveEvent(event);

    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    m_mousePoint = event->globalPos()-pos();
}

