#include "UserLogin.h"
#include "CusTitleBar.h"
#include <QDebug>
UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	//��������
	setAttribute(Qt::WA_DeleteOnClose);
	//setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);
	initTitleBar();
	this->setTitleBarTitle("", ":/Resources/MainWindow/aio_toobar_report.png");//���ñ���
	loadStyleSheet("UserLogin");
	initControl();
	
}

UserLogin::~UserLogin()
{}

void UserLogin::onShowUser()
{
	qDebug() << "receive signal";
	this->show();
}

void UserLogin::initControl()
{
	QLabel*headLabel = new QLabel(this);
	headLabel->setFixedSize(68, 68);
	//��ȡ�յ�Բͷ��
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headLabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"), pix, headLabel->size()));//Բͷ�����óɱ�ǩ�����
	//�ƶ���������
	headLabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);
	connect(NotifyManager::getInstance(), SIGNAL(signalShowTitle()), this, SLOT(onShowUser()));
}
void UserLogin::onLoginBtnClicked() {
	hide();
	CusTitleBar*mainwindow = new CusTitleBar;
	mainwindow->show();
}

