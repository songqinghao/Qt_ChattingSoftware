#pragma once

#include <QWidget>
#include "ui_ReceiveFile.h"
#include"basicwindow.h"
class ReceiveFile : public BasicWindow
{
	Q_OBJECT

public:
	ReceiveFile(QWidget *parent = Q_NULLPTR);
	~ReceiveFile();
	void setMsg(QString&msgLabel);

private slots:
	void on_cancelBtn_clicked();
	void on_okBtn_clicked();
signals:
	void refuseFile();
private:
	Ui::ReceiveFile ui;
};
