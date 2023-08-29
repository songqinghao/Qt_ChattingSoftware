#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_testStyleSheet.h"
#include <QtWidgets>
class testStyleSheet : public QMainWindow
{
    Q_OBJECT

public:
    testStyleSheet(QWidget *parent = nullptr);
    ~testStyleSheet();

private:
    Ui::testStyleSheetClass ui;
};
