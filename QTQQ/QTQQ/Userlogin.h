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
	void initControl();//��ʼ���ؼ�
	bool connectMySql();//�������ݿ�
	bool veryfyAccountCode(bool& isAccountLogin,QString& strAccount);//��֤�˺�����
private:
	Ui::Userlogin ui;
};
