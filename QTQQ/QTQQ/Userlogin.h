#pragma once

#include <QWidget>
#include "ui_Userlogin.h"
#include"basicwindow.h"

class Userlogin : public BasicWindow
{
	Q_OBJECT

public:
	Userlogin(QWidget *parent = Q_NULLPTR);
	~Userlogin();
private slots:
	void onLoginBtnClicked();
private:
	void initControl();//初始化控件
	bool connectMySql();//连接数据库
	bool veryfyAccountCode(bool& isAccountLogin,QString& strAccount);//验证账号密码
private:
	Ui::Userlogin ui;
};
