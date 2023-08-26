#include "TitleBar.h"
#include<QHBoxLayout>
#include<QPainter>
#include<QMouseEvent>
#include<QFile>
#include<QDebug>
#include "NotifyManager.h"
//预编译，预设置
#define BUTTON_HEIGHT 27	//按钮高度
#define BUTTON_WIDTH  27	//按钮宽度
#define TITLE_HEIGHT  27	//标题栏高度




//自定义标题栏
TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
	, m_isPressed(false)
	, m_buttonType(MIN_MAX_BUTTON)
{
	initControl();
	initConnections();
	loadStyleSheet("Title");	//标题栏样式表

}

TitleBar::~TitleBar()
{
}

//初始化控件
void TitleBar::initControl() {
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonMin = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);

	//按钮设置固定大小
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	//设置对象名
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");

	//设置布局
	QHBoxLayout* mylayout = new QHBoxLayout(this);//水平布局
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);
	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);

	mylayout->setContentsMargins(5, 0, 0, 0);	//设置布局间隙（和窗口边框）
	mylayout->setSpacing(0);					//设置部件间的间隙 
	//标题内容的尺寸策略，就和我们现在正在使用应用一样（水平可以扩展，垂直方向固定）
	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setFixedHeight(TITLE_HEIGHT);			//设置标题栏高度

	setWindowFlags(Qt::FramelessWindowHint);//窗体风格采用无边框
}


//初始化信号与槽的连接
void TitleBar::initConnections() {
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	
}

//设置标题栏图标
void TitleBar::setTitleIcon(const QString& filePath) {

	QPixmap titleIcon(filePath);
	m_pIcon->setFixedSize(titleIcon.size());//设置图标大小
	m_pIcon->setPixmap(titleIcon);
}

//设置标题栏内容
void TitleBar::setTitleContent(const QString& titleContent) {
	m_pTitleContent->setText(titleContent);//设置标题栏的内容
	m_titleContent = titleContent;
}

//设置标题栏的长度
void TitleBar::setTitleWidth(int width) {
	setFixedWidth(width);
}

void TitleBar::setButtonType(ButtonType buttontype) {
	m_buttonType = buttontype;
	switch (buttontype) {
	case MIN_BUTTON://只有最小化按钮和关闭按钮	
	{
		m_pButtonRestore->setVisible(false);//是否可见
		m_pButtonMax->setVisible(false);
	}
	break;

	case MIN_MAX_BUTTON://有最大最小还有关闭按钮
	{
		m_pButtonRestore->setVisible(false);
	}
	break;

	case ONLY_CLOSE_BUTTON://只有关闭按钮
	{
		m_pButtonRestore->setVisible(false);
		m_pButtonMax->setVisible(false);
		m_pButtonMin->setVisible(false);
	}
	break;

	default:
		break;
	}
}


//保存窗口最大化前窗口的位置及大小
void TitleBar::saveRestoreInfo(const QPoint& point, const QSize& size) {
	m_restorePos = point;
	m_restoreSize = size;
}

//获取窗口最大化前窗口的位置及大小
void TitleBar::getRestoreInfo(QPoint& point, QSize& size) {
	point = m_restorePos;
	size = m_restoreSize;
}

//绘制标题栏
void TitleBar::paintEvent(QPaintEvent* event) {
	//设置背景色
	QPainter painter(this);
	QPainterPath pathBack;					//绘图路径
	pathBack.setFillRule(Qt::WindingFill);	//设置填充规则
	//x斜率和y斜率为3
	pathBack.addRoundedRect(QRect(0, 0, width(), height()), 3, 3);  //添加圆角矩形到绘图路径
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);   //像素转换采用平滑的像素转换（渲染）

	//当窗口最大化或还原后，窗口长度改变，标题栏相应做出改变
	//parentWidth()返回父部件
	//如果标题栏的宽度和父部件的宽度不一致
	if (width() != parentWidget()->width()) {
		setFixedWidth(parentWidget()->width());
	}
	QWidget::paintEvent(event);

}


//双击响应事件，主要实现双击标题栏进行最大化和最小化的操作
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event) {

	//必须存在最大化和最小化的按钮时才有效
	if (m_buttonType == MIN_MAX_BUTTON) {
		//如果最大化键可见的话就放大（说明是小化的就可以放大）
		if (m_pButtonMax->isVisible()) {
			onButtonMaxClicked();
		}
		else {
			//不可见的话就是还原操作（变小）
			onButtonRestoreClicked();
		}
	}
	return QWidget::mouseDoubleClickEvent(event);
}

//移动窗口事件（通过鼠标按下鼠标移动鼠标释放实现拖动标题栏实现移动窗口效果）
void TitleBar::mousePressEvent(QMouseEvent*event) {
	if (m_buttonType == MIN_MAX_BUTTON) {
		//在窗口最大化时禁止拖动
		if (m_pButtonMax->isVisible()) {
			m_isPressed = true;
			m_startMovePos = event->globalPos();  //globalPos();返回事件发生时鼠标的全局位置
		}
	}
	else {
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);
}

//鼠标移动事件
void TitleBar::mouseMoveEvent(QMouseEvent* event) {
	if (m_isPressed) {
		//鼠标全局坐标相对于原来偏移了多少
		QPoint movePoint = event->globalPos() - m_startMovePos;//返回值就是移动多少
		//获取父部件的全局坐标
		QPoint widgetPos = parentWidget()->pos();
		m_startMovePos = event->globalPos();
		parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());//父部件进行移动
		//parentWidget()->move(movePoint.x(), movePoint.y());//父部件进行移动
		//qDebug() << "parentWidget()->pos();" << widgetPos.x() << " " << widgetPos.y();
		//qDebug() << "global;" << event->globalPos().x() << " " << event->globalPos().y();
	}
	return QWidget::mouseMoveEvent(event);
}

//鼠标释放事件
void TitleBar::mouseReleaseEvent(QMouseEvent* event) {
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

//加载样式表
void TitleBar::loadStyleSheet(const QString& sheetName) {
	QFile file(":/Resources/QSS/" + sheetName + ".css");	//sheetName在这就是表示titlebar
	if (file.open(QFile::ReadOnly)) {//只读
		QString styleSheet = this->styleSheet();		//当前样式表
		//QLatin1String只保存了地址和长度，比QString开销小
		styleSheet += QLatin1String(file.readAll());	//将文件的内容和现有的样式表进行合并
		setStyleSheet(styleSheet);						//设置样式表
	}
}


void TitleBar::onButtonMinClicked() {
	emit signalButtonMinClicked();
}

void TitleBar::onButtonRestoreClicked() {
	m_pButtonRestore->setVisible(false);  //恢复按钮不可见
	m_pButtonMax->setVisible(true);		  //最大化按钮可见
	emit signalButtonRestoreClicked();
}

void TitleBar::onButtonCloseClicked() {
	emit signalButtonCloseClicked();
}


void TitleBar::onButtonMaxClicked() {
	m_pButtonMax->setVisible(false);	//放大键不可见
	m_pButtonRestore->setVisible(true);	//还原键可见
	emit signalButtonMaxClicked();
}

