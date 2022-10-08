#include "basicwindow.h"
#include"CommonUtils.h"
#include"NotifyManager.h"
#include<QFile>
#include<QStyleOption>
#include<QPainter>
#include<QApplication>
#include<QMouseEvent>
#include<QDesktopWidget>
#include<QSqlQuery>
extern QString gLoginEmployeeID;
BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	//获取默认颜色，设置背景颜色
	m_colorBackGround = CommonUtils::getDefaultSkinColor();

	//设置窗体风格：无边框
	setWindowFlags(Qt::FramelessWindowHint);
	//设置属性，透明效果
	setAttribute(Qt::WA_TranslucentBackground, true);

	/* NotifyManage::getInstance()：获取窗口实例
	 * SIGNAL(signalSKinChanged())：颜色被改变
	 * SLOT(onSignalSkinChanged(const QColor&)：槽方法响应
	 */
	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)),
		this, SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{
}

void BasicWindow::onSignalSkinChanged(const QColor&color) {
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);	//颜色改变以后要把样式表重新进行添加
}

void BasicWindow::setTitleBarTitle(const QString& title, const QString&icon) {
	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

//初始化标题栏
void BasicWindow::initTitleBar(ButtonType buttonType) {
	_titleBar = new TitleBar(this);
	_titleBar->setButtonType(buttonType);//设置按钮类型
	_titleBar->move(0, 0);				//讲窗口调整能到0，0

	//各个信号与槽连接的建立
	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

}

void BasicWindow::loadStyleSheet(const QString& sheetName) {
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);//以只读的方式读取文件
	if (file.isOpen()) {
		setStyleSheet("");
		//将样式表全部读取，保存到字符串中
		QString qsstyleSheet = QLatin1String(file.readAll());

		//获取用户当前的皮肤RGB值
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		qsstyleSheet += QString("QWidget[titleskin=true]\
								{background-color:rgb(%1,%2,%3);\
								border-top-left-radius:4px;}\
								QWidget[bottomskin=true]\
								{border-top:1px solid rgba(%1,%2,%3,100);\
								background-color:rgba(%1,%2,%3,50);\
								border-bottom-left-radius:4px;\
								border-bottom-right-radius:4px;}")
			.arg(r).arg(g).arg(b);
		setStyleSheet(qsstyleSheet);
	}
	file.close();
}


//初始化背景图
void BasicWindow::initBackGroundColor() {
	QStyleOption opt;  //风格选项
	opt.init(this);    //获取部件的风格选项
	QPainter p(this);  //画家(以当前部件进行初始化)
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);//进行背景图的绘制，style()：获取画刷
}

//子类化部件时，需要重写绘图事件设置背景图
void BasicWindow::paintEvent(QPaintEvent* event) {
	initBackGroundColor();
	QDialog::paintEvent(event);  //和BasicWindow里的方法不一样
}

//获取圆头像
//src:图片来源
//mask:圆头像的背景（空）
//maskSize:圆头像的大小
QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize) {
	if (maskSize == QSize(0, 0)) {//如果给定圆头像大小为0，就将圆头像背景大小作为圆头像大小进行制作
		maskSize = mask.size();
	}
	else {

		//进行给定图片背景的缩放,大小为maskSize的大小，以最大的矩形进行缩放,平滑的缩放
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	//保存转换后的图像（尺寸为maskSize，格式为QImage::Format_ARGB32_Premultiplied）通过这种格式转换好的头像会比较清晰
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);

	//绘制，以结果图像进行初始（画家要来画resultImage）
	QPainter painter(&resultImage);

	//设置图片的叠加模式，_Source将原图像以复制的方式来进行操作，原图是矩形
	painter.setCompositionMode(QPainter::CompositionMode_Source);

	//填充一个矩形，大小就是图片的大小，同时是透明的
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	//与mask进行叠加
	painter.drawPixmap(0, 0, mask);//mask做destination，原来的矩形做source
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//sourceIn返回是源

	//每个头像以一个比例进行叠加进去
	painter.drawPixmap(0, 0, src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));//src做source，原来的mask作为destination
	painter.end();//画完了

	//形成我们的目标头像
	return QPixmap::fromImage(resultImage);
}

//关闭槽方法
void BasicWindow::onShowClose(bool) {
	close();
}
//隐藏槽方法
void BasicWindow::onShowHide(bool) {
	hide();
}
//最小化槽方法
void BasicWindow::onShowMin(bool) {
	showMinimized();
}

//正常显示槽方法
void BasicWindow::onShowNormal(bool) {
	show();
	activateWindow();//设置成活动窗口
}
//退出窗口槽方法
void BasicWindow::onShowQuit(bool) {
	//更新登陆状态为离线
	QString strSqlStatus = QString("UPDATE tab_employees SET online=1 WHERE employeeID=%1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();
	QApplication::quit();//关闭应用程序
}

//鼠标移动事件
void BasicWindow::mouseMoveEvent(QMouseEvent* event) {
	//按下了，且按下的是（左键(event->button() && Qt::LeftButton)）
	if (m_mousePressed && (event->buttons() && Qt::LeftButton)) {
		move(event->globalPos() - m_mousePoint);//event->globalPos()用于返回事件发生时候的全局坐标（相对于屏幕左上角）
		event->accept();
	}
}

//鼠标按下事件
void BasicWindow::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		m_mousePressed = true;
		//pos事件发生时相对于窗口左上角的偏移坐标
		m_mousePoint = event->globalPos() - pos();
		event->accept();
	}
}

//鼠标释放事件
void BasicWindow::mouseReleaseEvent(QMouseEvent*event) {
	m_mousePressed = false;
}

//最小化按钮
void BasicWindow::onButtonMinClicked() {
	if (Qt::Tool == (windowFlags() & Qt::Tool)) {				//???
		hide();
	}
	else {
		showMinimized();
	}
}

void BasicWindow::onButtonRestoreClicked() {
	QPoint windowPos;
	QSize windowSize;
	_titleBar->getRestoreInfo(windowPos, windowSize);
	//设置几何属性
	setGeometry(QRect(windowPos, windowSize));
}

void BasicWindow::onButtonCloseClicked() {
	close();
}

void BasicWindow::onButtonMaxClicked() {
	_titleBar->saveRestoreInfo(pos(), QSize(width(), height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3,
		desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(factRect);
}