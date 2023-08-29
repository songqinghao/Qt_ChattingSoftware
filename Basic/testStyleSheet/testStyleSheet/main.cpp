#include "testStyleSheet.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    testStyleSheet w;
    w.show();
    return a.exec();
}
