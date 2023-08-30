[toc]

# QPalette调色板类

## 前言
Qt提供的调色板类QPalette专门用于管理部件的外观显示，相当于部件或对话框的调色板，管理他们所有的颜色信息。每个部件都包含一个QPalette对象，在显示时，按照它的QPalette对象中对各部分各状态下的颜色的描述进行绘制，使用调色板类对部件只能进行一些简单的调色，如果想要一些复杂的设置那么就得使用样式表进行设置。

下面用到了一个简单的demo对QPalette调色板类的使用进行说明：

最终效果如下图所示：
`通过左边的下拉框对颜色进行选择，并对指定部件赋予颜色`
![在这里插入图片描述](https://img-blog.csdnimg.cn/04da49f0c1854f29ab58035fb6142bc7.png)

## 代码

先贴代码吧，后续对代码进行讲解！！
`dialog.h`

```cpp
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFrame>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
    //创建边框区域
    void createCtrlFrame();
    //创建右边内容区域
    void createContentFrame();


    //给组合框添加颜色列表
    void fillColorList(QComboBox*comboBox);
public slots:
    void ShowWindow(int index);
    void ShowWindowText(int index);
    void ShowButton(int index);
    void ShowButtonText(int index);
    void ShowBase(int index);

private:
    QFrame* m_ctrlFrame;
    QLabel* m_windowLabel;
    QComboBox* m_windowComboBox;
    QLabel* m_windowTextLabel;
    QComboBox* m_windowTextComboBox;
    QLabel* m_buttonLabel;
    QComboBox* m_buttonComboBox;
    QLabel* m_buttonTextLabel;
    QComboBox* m_buttonTextComboBox;
    QLabel* m_baseLabel;
    QComboBox* m_baseComboBox;
    QFrame* m_contentFrame;
    QLabel* m_label1;
    QLabel* m_label2;
    QComboBox* m_comboBox1;
    QLineEdit* m_lineEdit;
    QTextEdit* m_textEdit;
    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;
};

#endif // DIALOG_H

```

```cpp
#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    createCtrlFrame();
    createContentFrame();
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_ctrlFrame);
    mainLayout->addWidget(m_contentFrame);

}

Dialog::~Dialog()
{

}

void Dialog::createCtrlFrame()
{
    m_ctrlFrame = new QFrame;
    m_ctrlFrame->setFrameStyle(QFrame::Sunken|QFrame::Box);
    m_windowLabel = new QLabel("QPalette::Window");
    m_windowComboBox = new QComboBox;
    //将颜色填充到ComboBox的每一个项中
    fillColorList(m_windowComboBox);
    connect(m_windowComboBox,SIGNAL(activated(int)),this,SLOT(ShowWindow(int)));

    m_windowTextLabel = new QLabel("QPalette::WindowText");
    m_windowTextComboBox = new QComboBox;
    fillColorList(m_windowTextComboBox);
    connect(m_windowTextComboBox,SIGNAL(activated(int)),this,SLOT(ShowWindowText(int)));

    m_buttonTextLabel = new QLabel("QPalette::ButtonText");
    m_buttonTextComboBox = new QComboBox;
    fillColorList(m_buttonTextComboBox);
    connect(m_buttonTextComboBox,SIGNAL(activated(int)),this,SLOT(ShowButtonText(int)));

    m_buttonLabel = new QLabel("QPalette::Button");
    m_buttonComboBox = new QComboBox;
    fillColorList(m_buttonComboBox);
    connect(m_buttonComboBox,SIGNAL(activated(int)),this,SLOT(ShowButton(int)));

    m_baseLabel = new QLabel("QPalette::Base");
    m_baseComboBox = new QComboBox;
    fillColorList(m_baseComboBox);
    connect(m_baseComboBox,SIGNAL(activated(int)),this,SLOT(ShowBase(int)));

    QGridLayout* mainLayout = new QGridLayout(m_ctrlFrame);
    mainLayout->setSpacing(20);
    mainLayout->addWidget(m_windowLabel,0,0);
    mainLayout->addWidget(m_windowComboBox,0,1);
    mainLayout->addWidget(m_windowTextLabel,1,0);
    mainLayout->addWidget(m_windowTextComboBox,1,1);
    mainLayout->addWidget(m_buttonLabel,2,0);
    mainLayout->addWidget(m_buttonComboBox,2,1);
    mainLayout->addWidget(m_buttonTextLabel,3,0);
    mainLayout->addWidget(m_buttonTextComboBox,3,1);
    mainLayout->addWidget(m_baseLabel,4,0);
    mainLayout->addWidget(m_baseComboBox,4,1);

}

void Dialog::createContentFrame()
{
    m_contentFrame = new QFrame;
    //自动填充背景设置为true
    m_contentFrame->setAutoFillBackground(true);
    m_label1 = new QLabel(QStringLiteral("请选择一个值"));
    m_label2 = new QLabel(QStringLiteral("请输入字符串"));
    m_comboBox1 = new QComboBox;
    m_lineEdit = new QLineEdit;
    m_textEdit = new QTextEdit;

    m_okBtn = new QPushButton(QStringLiteral("确认"));
    m_cancelBtn = new QPushButton(QStringLiteral("取消"));
    m_okBtn->setAutoFillBackground(true);



    QGridLayout* topLayout = new QGridLayout;
    topLayout->addWidget(m_label1,0,0);
    topLayout->addWidget(m_comboBox1,0,1);
    topLayout->addWidget(m_label2,1,0);
    topLayout->addWidget(m_lineEdit,1,1);
    topLayout->addWidget(m_textEdit,2,0,1,2);

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_okBtn);
    bottomLayout->addWidget(m_cancelBtn);

    QVBoxLayout* mainLayout = new QVBoxLayout(m_contentFrame);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
}

void Dialog::fillColorList(QComboBox *comboBox)
{
    //获取到各个颜色的名字
    QStringList colorList = QColor::colorNames();
    QString color;
    foreach(color,colorList)
    {
        QPixmap pix(QSize(70,20));
        pix.fill(QColor(color));
        comboBox->addItem(QIcon(pix),nullptr);
        comboBox->setIconSize(QSize(70,20));
        //适应方式是根据内容来进行适应
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    }
}

void Dialog::ShowWindow(int index)
{
    QStringList colorList = QColor::colorNames();
    //拿到颜色名字
    QColor color = QColor(colorList[index]);
    QPalette p = m_contentFrame->palette();
    p.setColor(QPalette::Window,color);
    m_contentFrame->setPalette(p);
    //触发窗口重新绘制功能，更新窗口
    m_contentFrame->update();

}

void Dialog::ShowWindowText(int index)
{
    QStringList colorList = QColor::colorNames();
    //拿到颜色名字
    QColor color = QColor(colorList[index]);
    QPalette p = m_contentFrame->palette();
    //会改变框框中label字体颜色的值，而button字体颜色不会改变
    p.setColor(QPalette::WindowText,color);
    m_contentFrame->setPalette(p);
    //触发窗口重新绘制功能，更新窗口
    m_contentFrame->update();
}

void Dialog::ShowButton(int index)
{
    QStringList colorList = QColor::colorNames();
    //拿到颜色名字
    QColor color = QColor(colorList[index]);
    QPalette p = m_contentFrame->palette();

    p.setColor(QPalette::Button,color);
    m_contentFrame->setPalette(p);
    //触发窗口重新绘制功能，更新窗口
    m_contentFrame->update();
}

void Dialog::ShowButtonText(int index)
{
    QStringList colorList = QColor::colorNames();
    //拿到颜色名字
    QColor color = QColor(colorList[index]);
    QPalette p = m_contentFrame->palette();

    p.setColor(QPalette::ButtonText,color);
    m_contentFrame->setPalette(p);
    //触发窗口重新绘制功能，更新窗口
    m_contentFrame->update();
}

void Dialog::ShowBase(int index)
{
    QStringList colorList = QColor::colorNames();
    //拿到颜色名字
    QColor color = QColor(colorList[index]);
    QPalette p = m_contentFrame->palette();
    //设置底色
    p.setColor(QPalette::Base,color);
    m_contentFrame->setPalette(p);
    //触发窗口重新绘制功能，更新窗口
    m_contentFrame->update();
}
```

## 知识点讲解
1、将颜色作为下拉框的选项
在本工程中是使用如下`fillColorList`函数进行实现的

```cpp
void Dialog::fillColorList(QComboBox *comboBox)
{
    //获取到各个颜色的名字
    QStringList colorList = QColor::colorNames();
    QString color;
    foreach(color,colorList)
    {
        QPixmap pix(QSize(70,20));
        //对pix进行颜色填充
        pix.fill(QColor(color));
        //将pix左右Item添加到下拉框中
        comboBox->addItem(QIcon(pix),nullptr);
        //尺寸大小设置的要与上面QPixmap的大小一致
        comboBox->setIconSize(QSize(70,20));
        //适应方式是根据内容来进行适应
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    }
}
```

---
2、改变整个部件的颜色（但是这个改变不包括这个部件内部的部件）
注意：`m_contentFrame->setAutoFillBackground(true);`这个得设置，否则这个设置会失败。

```cpp
//1、先拿到部件的调色板设置
QPalette p = m_contentFrame->palette();
//2、再设置QPalette::Window的颜色
p.setColor(QPalette::Window,color);
//将部件的调色板重新设置为上面的调色板
m_contentFrame->setPalette(p);
//主动触发窗口重新绘制功能，更新窗口
m_contentFrame->update();
```

---
3、改变部件内部嵌套的Label的字体颜色

```cpp
QPalette p = m_contentFrame->palette();
//会改变部件中嵌套的label字体颜色，而button字体颜色不会改变
p.setColor(QPalette::WindowText,color);
m_contentFrame->setPalette(p);
//触发窗口重新绘制功能，更新窗口
m_contentFrame->update();
```
---
4、改变部件内部嵌套的button的边框颜色
注意：`m_okBtn->setAutoFillBackground(true);`这个得设置，否则m_okBtn这个按钮对于边框颜色的设置会失败。

```cpp
QPalette p = m_contentFrame->palette();
p.setColor(QPalette::Button,color);
m_contentFrame->setPalette(p);
//触发窗口重新绘制功能，更新窗口
m_contentFrame->update();
```
---
5、改变部件内部嵌套的button的字体颜色

```cpp
QPalette p = m_contentFrame->palette();
//改变部件内部嵌套的button的字体颜色
p.setColor(QPalette::ButtonText,color);
m_contentFrame->setPalette(p);
//触发窗口重新绘制功能，更新窗口
m_contentFrame->update();
```
---
6、改变部件内部嵌入的文本输入部件（我这里是使用QTextEdit对象）的背景色

```cpp
QPalette p = m_contentFrame->palette();
//设置底色
p.setColor(QPalette::Base,color);
m_contentFrame->setPalette(p);
//触发窗口重新绘制功能，更新窗口
m_contentFrame->update();
```
---

`其实还有其他的参数，但是不可能一个个都列举出来，大家可以参考Qt的帮助文档来找到你想要的参数。`
![在这里插入图片描述](https://img-blog.csdnimg.cn/867a269cfc8d477d96be9ae28aa312aa.png)




