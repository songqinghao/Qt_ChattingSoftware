#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include"Userlogin.h"
#include<QApplication>
#include<QSurfaceFormat>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QApplication::setAttribute(Qt::AA_DisableShaderDiskCache);

	a.setQuitOnLastWindowClosed(false);  //当最后一个窗口关闭时应用程序不退出
	Userlogin *userLogin=new Userlogin;
	userLogin->show();	

	return a.exec();
}
