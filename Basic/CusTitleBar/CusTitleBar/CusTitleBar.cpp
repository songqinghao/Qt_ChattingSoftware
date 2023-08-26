#include "CusTitleBar.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QDialog>
#include <QPainter>
#include <QMouseEvent>
#include <QStyleOption>
#include <QDebug>
#include "NotifyManager.h"
CusTitleBar::CusTitleBar(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	showUser = new QPushButton(this);
	showUser->setText(QStringLiteral("显示登录窗口"));
	connect(showUser, SIGNAL(clicked()), this, SLOT(onShowTitleBar()));
}

CusTitleBar::~CusTitleBar()
{}

void CusTitleBar::onShowTitleBar()
{
	emit NotifyManager::getInstance()->signalShowTitle();
	qDebug() << "发送title信号";
}




