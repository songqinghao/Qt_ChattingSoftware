#include "UserLogin.h"
#include "CusTitleBar.h"
#include <QDebug>
UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	//设置属性
	setAttribute(Qt::WA_DeleteOnClose);
	//setWindowFlags(Qt::Tool|Qt::FramelessWindowHint);
	initTitleBar();
	this->setTitleBarTitle("", ":/Resources/MainWindow/aio_toobar_report.png");//设置标题
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
	//获取空的圆头像
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headLabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"), pix, headLabel->size()));//圆头像设置成标签的外表
	//移动到正中心
	headLabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);
	connect(NotifyManager::getInstance(), SIGNAL(signalShowTitle()), this, SLOT(onShowUser()));
}
void UserLogin::onLoginBtnClicked() {
	hide();
	CusTitleBar*mainwindow = new CusTitleBar;
	mainwindow->show();
}

