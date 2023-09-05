#ifndef KEYEVENT_H
#define KEYEVENT_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QKeyEvent>
namespace Ui {
class KeyEvent;
}

class KeyEvent : public QWidget
{
    Q_OBJECT

public:
    explicit KeyEvent(QWidget *parent = nullptr);
    ~KeyEvent();
    //画图片
    void drawPix();
protected:
    //重写键盘事件和绘图事件
    void keyPressEvent(QKeyEvent* event)override;
    void paintEvent(QPaintEvent* event)override;
private:
    Ui::KeyEvent *ui;
    QPixmap* m_pix;
    QImage m_image;
    //图标的顶点位置
    int m_startx;
    int m_starty;
    //界面的宽度高度
    int m_width;
    int m_height;
    //步长
    int m_step;
};

#endif // KEYEVENT_H
