#include "eventfilter.h"
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>
EventFilter::EventFilter(QWidget *parent,Qt::WindowFlags f)
    : QDialog(parent,f)
{
    setWindowTitle(QStringLiteral("事件过滤"));

    m_label1 = new QLabel;
    m_label2 = new QLabel;
    m_label3 = new QLabel;

    m_stateLabel = new QLabel(QStringLiteral("鼠标按下标志"));
    //加个框框
    //m_stateLabel->setFrameStyle(QFrame::Box);
    //设置对齐方式：水平居中
    m_stateLabel->setAlignment(Qt::AlignHCenter);
    m_image1.load("./fly1.png");
    m_image2.load("./fly2.png");
    m_image3.load("./fly3.png");

    m_label1->setPixmap(QPixmap::fromImage(m_image1));
    m_label2->setPixmap(QPixmap::fromImage(m_image2));
    m_label3->setPixmap(QPixmap::fromImage(m_image3));

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_label1);
    layout->addWidget(m_label2);
    layout->addWidget(m_label3);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);
    mainLayout->addWidget(m_stateLabel);

    resize(m_image2.width() * 3,m_image2.height() * 2);
    //给m_label123安装事件过滤，指定(this)当前整个窗体监视我们的事件对象
    m_label1->installEventFilter(this);
    m_label2->installEventFilter(this);
    m_label3->installEventFilter(this);

}

EventFilter::~EventFilter()
{

}
//第一个参数是监视对象，第二个参数是事件
bool EventFilter::eventFilter(QObject *watched, QEvent *event)
{
    QMatrix matrix;
    QImage tmpImg;
    //放大图片
    matrix.scale(2.0,2.0);

    if(watched == m_label1)
    {
        //鼠标按下事件
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouseEvent = (QMouseEvent*)event;
            if(mouseEvent->buttons()&Qt::LeftButton)
            {
                m_stateLabel->setText(QStringLiteral("左键按下图片1"));
            }
            if(mouseEvent->buttons()&Qt::MidButton)
            {
                m_stateLabel->setText(QStringLiteral("中键按下图片1"));
            }
            if(mouseEvent->buttons()&Qt::RightButton)
            {
                m_stateLabel->setText(QStringLiteral("右键按下图片1"));
            }

            tmpImg = m_image1.transformed(matrix);
            m_label1->setPixmap(QPixmap::fromImage(tmpImg));
        }

        if(event->type()==QEvent::MouseButtonRelease)
        {
            m_stateLabel->setText(QStringLiteral("鼠标释放图片1"));
            m_label1->setPixmap(QPixmap::fromImage(m_image1));
        }
    }
    else if(watched == m_label2)
    {
        //鼠标按下事件
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouseEvent = (QMouseEvent*)event;
            if(mouseEvent->buttons()&Qt::LeftButton)
            {
                m_stateLabel->setText(QStringLiteral("左键按下图片2"));
            }
            if(mouseEvent->buttons()&Qt::MidButton)
            {
                m_stateLabel->setText(QString("中键按下图片2"));
            }
            if(mouseEvent->buttons()&Qt::RightButton)
            {
                m_stateLabel->setText(QString("右键按下图片2"));
            }

            tmpImg = m_image2.transformed(matrix);
            m_label2->setPixmap(QPixmap::fromImage(tmpImg));
        }

        if(event->type()==QEvent::MouseButtonRelease)
        {
            m_stateLabel->setText(QStringLiteral("鼠标释放图片2"));
            m_label2->setPixmap(QPixmap::fromImage(m_image2));
        }
    }
    else if(watched == m_label3)
    {
        //鼠标按下事件
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouseEvent = (QMouseEvent*)event;
            if(mouseEvent->buttons()&Qt::LeftButton)
            {
                m_stateLabel->setText(QStringLiteral("左键按下图片3"));
            }
            if(mouseEvent->buttons()&Qt::MidButton)
            {
                m_stateLabel->setText(QStringLiteral("中键按下图片3"));
            }
            if(mouseEvent->buttons()&Qt::RightButton)
            {
                m_stateLabel->setText(QStringLiteral("右键按下图片3"));
            }

            tmpImg = m_image3.transformed(matrix);
            m_label3->setPixmap(QPixmap::fromImage(tmpImg));
        }

        if(event->type()==QEvent::MouseButtonRelease)
        {
            m_stateLabel->setText(QStringLiteral("鼠标释放图片3"));
            m_label3->setPixmap(QPixmap::fromImage(m_image3));
        }
    }

    //将事件交给上层对话框进行处理
    return QDialog::eventFilter(watched,event);
}
