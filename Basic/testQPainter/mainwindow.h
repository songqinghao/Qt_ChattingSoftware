#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintarea.h"
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QFrame>
#include <QPushButton>
#include <QGridLayout>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void initWidgets();
private slots:
    void ShowShape(int value);
    void ShowPenColor();
private:
    PaintArea*m_paintArea;//绘制区域
    QComboBox* m_shapeCombox;
    QSpinBox*m_penWithSpinBox;
    QLabel* m_penWidthLabel;//线宽
    QLabel* m_shapeLabel;//形状
    QLabel* m_penStyleLabel;//画笔风格
    QComboBox* m_penStyleComboBox;
    QLabel*m_m_penCapLabel;//笔帽风格
    QComboBox* m_penCapComboBox;
    QLabel* m_penJoinLabel;//画笔连接点
    QComboBox* m_penJoinComboBox;
    QLabel* m_fillRuleLabel;//填充效果
    QComboBox* m_m_fillRuleComboBox;
    QLabel* m_spreadLabel;//铺展效果
    QLabel* m_penColorLabel;//画笔颜色
    QFrame* m_penColorFrame;
    QPushButton* m_penColorBtn;
    QLabel* m_brushColorLabel;//画刷颜色
    QFrame* m_brushColorFrame;
    QPushButton* m_brushColorBtn;
    QLabel* m_brushStyleLabel;//画刷风格
    QComboBox*m_brushStyleComboBox;
    QGridLayout* m_rightLayout;



    ~MainWindow();
};
#endif // MAINWINDOW_H
