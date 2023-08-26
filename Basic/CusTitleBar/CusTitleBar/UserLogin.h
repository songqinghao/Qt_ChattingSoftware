#pragma once

#include <QWidget>
#include "ui_UserLogin.h"
#include "BasicWindow.h"
class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = nullptr);
	~UserLogin();

private:
	Ui::UserLogin ui;
private slots:
	void onLoginBtnClicked();
	void onShowUser();
private:
	void initControl();
};
