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
