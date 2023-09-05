#include "keyevent.h"
#include "ui_keyevent.h"

KeyEvent::KeyEvent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyEvent)
{
    ui->setupUi(this);
    //设置标题
    setWindowTitle(QStringLiteral("键盘事件"));
    setAutoFillBackground(true);
    setFixedSize(512,256);
    m_width = size().width();
    m_height = size().height();
    //初始化pixmap
    m_pix = new QPixmap(m_width,m_height);
    //m_pix->fill(Qt::white);
    m_image.load("tiger.png");
    m_startx = 100;
    m_starty = 100;
    m_step = 20;
    drawPix();
}

KeyEvent::~KeyEvent()
{
    delete ui;
}

void KeyEvent::drawPix()
{
    m_pix->fill(Qt::white);
    QPainter painter(this);
    //虚线
    QPen pen(Qt::DotLine);
    //指定m_pix为绘图设备
    painter.begin(m_pix);
    //将Pen作为画家的笔
    painter.setPen(pen);


    //按照步长画纵向网格线
    for(int i = m_step; i < m_width; i += m_step)
    {

        painter.drawLine(QPoint(i,0),QPoint(i,m_height));

    }

    //按照步长画横向网格线
    for(int i = m_step; i < m_height; i += m_step)
    {

        painter.drawLine(QPoint(0,i),QPoint(m_width,i));

    }
    painter.end();

    //画图片
    painter.begin(m_pix);
    painter.drawImage(QPoint(m_startx,m_starty),m_image);
    painter.end();

}

void KeyEvent::keyPressEvent(QKeyEvent *event)
{
    //如果按下ctrl修饰键的话
    if(event->modifiers()==Qt::ControlModifier)
    {
        if(event->key() == Qt::Key_Left)
        {
            m_startx = (m_startx - 1)<0 ? m_startx:m_startx - 1;
        }
        if(event->key() == Qt::Key_Right)
        {
            m_startx = (m_startx + 1 + m_image.width()) > m_width ? m_startx:m_startx + 1;
        }
        if(event->key() == Qt::Key_Up)
        {
            m_starty = (m_starty - 1 ) < 0 ? m_starty:m_starty - 1;
        }
        if(event->key() == Qt::Key_Down)
        {
            m_starty = (m_starty + 1 + m_image.height()) > m_height ? m_starty:m_starty + 1;
        }
    }
    else{
        //调整图片位置
        m_startx = m_startx - m_startx % m_step;
        m_starty = m_starty - m_starty % m_step;
        if(event->key() == Qt::Key_Left)
        {
            m_startx = (m_startx - m_step)<0 ? m_startx:m_startx - m_step;
        }
        if(event->key() == Qt::Key_Right)
        {
            m_startx = (m_startx + m_step + m_image.width()) > m_width ? m_startx:m_startx + m_step;
        }
        if(event->key() == Qt::Key_Up)
        {
            m_starty = (m_starty - m_step ) < 0 ? m_starty:m_starty - m_step;
        }
        if(event->key() == Qt::Key_Down)
        {
            m_starty = (m_starty + m_step + m_image.height()) > m_height ? m_starty:m_starty + m_step;
        }
    }
    //绘画界面
    drawPix();
    //触发PainterEvent，真真将m_pix画上去
    update();
}

void KeyEvent::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    //将m_pix绘制上去
    painter.drawPixmap(QPoint(0,0),*m_pix);
    painter.end();
    QWidget::paintEvent(event);
}
