#pragma once

#include <QtWidgets/QWidget>
#include "ui_CusTitleBar.h"
#include "TitleBar.h"
#include "NotifyManager.h"
class CusTitleBar : public QWidget
{
    Q_OBJECT

public:
    CusTitleBar(QWidget *parent = nullptr);
    ~CusTitleBar();
public slots:
	void onShowTitleBar();

private:
    Ui::CusTitleBarClass ui;
	QPushButton*showUser;
};
