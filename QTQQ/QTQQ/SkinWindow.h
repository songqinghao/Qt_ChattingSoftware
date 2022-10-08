#pragma once

#include <QWidget>
#include "ui_SkinWindow.h"
#include"basicwindow.h"
class SkinWindow : public BasicWindow
{
	Q_OBJECT

public:
	SkinWindow(QWidget *parent = Q_NULLPTR);
	~SkinWindow();
public:
	void initControl();
public slots:
	void onShowClose();
private:
	Ui::SkinWindow ui;
};
