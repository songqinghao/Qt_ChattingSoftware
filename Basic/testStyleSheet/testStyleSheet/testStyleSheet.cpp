#include "testStyleSheet.h"
#include <QString>
testStyleSheet::testStyleSheet(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	//������ʽ
	//ui.pushButton->setStyleSheet("background:yellow");
	//ui.pushButton->setStyleSheet("color:red");

	//������ҳ�������ʽ���1��ѡ�����pushButton������ʽ��ƣ�
	setStyleSheet("QPushButton#pushButton{background:yellow;color:red;}");
	ui.horizontalSlider->setStyleSheet("background:red");
}

testStyleSheet::~testStyleSheet()
{}
