#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QDialog>
#include <QLabel>
class EventFilter : public QDialog
{
    Q_OBJECT

public:
    EventFilter(QWidget *parent = 0,Qt::WindowFlags f = 0);
    ~EventFilter();
public slots:
    //第一个参数是监视的对象，第二个参数是事件
    bool eventFilter(QObject*watched,QEvent *event)override;
private:
    //将图片加载到label中
    QLabel* m_label1;
    QLabel* m_label2;
    QLabel* m_label3;
    QLabel* m_stateLabel;
    //
    QImage m_image1;
    QImage m_image2;
    QImage m_image3;
};

#endif // EVENTFILTER_H
