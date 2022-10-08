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
	//��ȡĬ����ɫ�����ñ�����ɫ
	m_colorBackGround = CommonUtils::getDefaultSkinColor();

	//���ô������ޱ߿�
	setWindowFlags(Qt::FramelessWindowHint);
	//�������ԣ�͸��Ч��
	setAttribute(Qt::WA_TranslucentBackground, true);

	/* NotifyManage::getInstance()����ȡ����ʵ��
	 * SIGNAL(signalSKinChanged())����ɫ���ı�
	 * SLOT(onSignalSkinChanged(const QColor&)���۷�����Ӧ
	 */
	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)),
		this, SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{
}

void BasicWindow::onSignalSkinChanged(const QColor&color) {
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);	//��ɫ�ı��Ժ�Ҫ����ʽ�����½������
}

void BasicWindow::setTitleBarTitle(const QString& title, const QString&icon) {
	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

//��ʼ��������
void BasicWindow::initTitleBar(ButtonType buttonType) {
	_titleBar = new TitleBar(this);
	_titleBar->setButtonType(buttonType);//���ð�ť����
	_titleBar->move(0, 0);				//�����ڵ����ܵ�0��0

	//�����ź�������ӵĽ���
	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

}

void BasicWindow::loadStyleSheet(const QString& sheetName) {
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);//��ֻ���ķ�ʽ��ȡ�ļ�
	if (file.isOpen()) {
		setStyleSheet("");
		//����ʽ��ȫ����ȡ�����浽�ַ�����
		QString qsstyleSheet = QLatin1String(file.readAll());

		//��ȡ�û���ǰ��Ƥ��RGBֵ
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


//��ʼ������ͼ
void BasicWindow::initBackGroundColor() {
	QStyleOption opt;  //���ѡ��
	opt.init(this);    //��ȡ�����ķ��ѡ��
	QPainter p(this);  //����(�Ե�ǰ�������г�ʼ��)
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);//���б���ͼ�Ļ��ƣ�style()����ȡ��ˢ
}

//���໯����ʱ����Ҫ��д��ͼ�¼����ñ���ͼ
void BasicWindow::paintEvent(QPaintEvent* event) {
	initBackGroundColor();
	QDialog::paintEvent(event);  //��BasicWindow��ķ�����һ��
}

//��ȡԲͷ��
//src:ͼƬ��Դ
//mask:Բͷ��ı������գ�
//maskSize:Բͷ��Ĵ�С
QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize) {
	if (maskSize == QSize(0, 0)) {//�������Բͷ���СΪ0���ͽ�Բͷ�񱳾���С��ΪԲͷ���С��������
		maskSize = mask.size();
	}
	else {

		//���и���ͼƬ����������,��СΪmaskSize�Ĵ�С�������ľ��ν�������,ƽ��������
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	//����ת�����ͼ�񣨳ߴ�ΪmaskSize����ʽΪQImage::Format_ARGB32_Premultiplied��ͨ�����ָ�ʽת���õ�ͷ���Ƚ�����
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);

	//���ƣ��Խ��ͼ����г�ʼ������Ҫ����resultImage��
	QPainter painter(&resultImage);

	//����ͼƬ�ĵ���ģʽ��_Source��ԭͼ���Ը��Ƶķ�ʽ�����в�����ԭͼ�Ǿ���
	painter.setCompositionMode(QPainter::CompositionMode_Source);

	//���һ�����Σ���С����ͼƬ�Ĵ�С��ͬʱ��͸����
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	//��mask���е���
	painter.drawPixmap(0, 0, mask);//mask��destination��ԭ���ľ�����source
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//sourceIn������Դ

	//ÿ��ͷ����һ���������е��ӽ�ȥ
	painter.drawPixmap(0, 0, src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));//src��source��ԭ����mask��Ϊdestination
	painter.end();//������

	//�γ����ǵ�Ŀ��ͷ��
	return QPixmap::fromImage(resultImage);
}

//�رղ۷���
void BasicWindow::onShowClose(bool) {
	close();
}
//���ز۷���
void BasicWindow::onShowHide(bool) {
	hide();
}
//��С���۷���
void BasicWindow::onShowMin(bool) {
	showMinimized();
}

//������ʾ�۷���
void BasicWindow::onShowNormal(bool) {
	show();
	activateWindow();//���óɻ����
}
//�˳����ڲ۷���
void BasicWindow::onShowQuit(bool) {
	//���µ�½״̬Ϊ����
	QString strSqlStatus = QString("UPDATE tab_employees SET online=1 WHERE employeeID=%1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();
	QApplication::quit();//�ر�Ӧ�ó���
}

//����ƶ��¼�
void BasicWindow::mouseMoveEvent(QMouseEvent* event) {
	//�����ˣ��Ұ��µ��ǣ����(event->button() && Qt::LeftButton)��
	if (m_mousePressed && (event->buttons() && Qt::LeftButton)) {
		move(event->globalPos() - m_mousePoint);//event->globalPos()���ڷ����¼�����ʱ���ȫ�����꣨�������Ļ���Ͻǣ�
		event->accept();
	}
}

//��갴���¼�
void BasicWindow::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		m_mousePressed = true;
		//pos�¼�����ʱ����ڴ������Ͻǵ�ƫ������
		m_mousePoint = event->globalPos() - pos();
		event->accept();
	}
}

//����ͷ��¼�
void BasicWindow::mouseReleaseEvent(QMouseEvent*event) {
	m_mousePressed = false;
}

//��С����ť
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
	//���ü�������
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