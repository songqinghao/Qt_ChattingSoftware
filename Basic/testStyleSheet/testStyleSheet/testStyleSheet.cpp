#include "testStyleSheet.h"
#include <QString>
testStyleSheet::testStyleSheet(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	//设置样式
	//ui.pushButton->setStyleSheet("background:yellow");
	//ui.pushButton->setStyleSheet("color:red");

	//对整个页面进行样式设计1，选择对象pushButton进行样式设计，
	setStyleSheet("QPushButton#pushButton{background:yellow;color:red;}");
	ui.horizontalSlider->setStyleSheet("background:red");
}

testStyleSheet::~testStyleSheet()
{}
