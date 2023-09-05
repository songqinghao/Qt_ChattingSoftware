/********************************************************************************
** Form generated from reading UI file 'keyevent.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYEVENT_H
#define UI_KEYEVENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KeyEvent
{
public:

    void setupUi(QWidget *KeyEvent)
    {
        if (KeyEvent->objectName().isEmpty())
            KeyEvent->setObjectName(QString::fromUtf8("KeyEvent"));
        KeyEvent->resize(599, 396);

        retranslateUi(KeyEvent);

        QMetaObject::connectSlotsByName(KeyEvent);
    } // setupUi

    void retranslateUi(QWidget *KeyEvent)
    {
        KeyEvent->setWindowTitle(QApplication::translate("KeyEvent", "KeyEvent", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KeyEvent: public Ui_KeyEvent {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYEVENT_H
