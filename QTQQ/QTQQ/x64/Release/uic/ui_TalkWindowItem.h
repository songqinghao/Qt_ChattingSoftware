/********************************************************************************
** Form generated from reading UI file 'TalkWindowItem.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TALKWINDOWITEM_H
#define UI_TALKWINDOWITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TalkWindowItem
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *headlabel;
    QLabel *msgLabel;
    QPushButton *tclosebtn;

    void setupUi(QWidget *TalkWindowItem)
    {
        if (TalkWindowItem->objectName().isEmpty())
            TalkWindowItem->setObjectName(QStringLiteral("TalkWindowItem"));
        TalkWindowItem->resize(404, 42);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TalkWindowItem->sizePolicy().hasHeightForWidth());
        TalkWindowItem->setSizePolicy(sizePolicy);
        TalkWindowItem->setMinimumSize(QSize(0, 42));
        TalkWindowItem->setMaximumSize(QSize(16777215, 42));
        horizontalLayout = new QHBoxLayout(TalkWindowItem);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 6, 6, 6);
        headlabel = new QLabel(TalkWindowItem);
        headlabel->setObjectName(QStringLiteral("headlabel"));
        headlabel->setMinimumSize(QSize(30, 30));
        headlabel->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(headlabel);

        msgLabel = new QLabel(TalkWindowItem);
        msgLabel->setObjectName(QStringLiteral("msgLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(msgLabel->sizePolicy().hasHeightForWidth());
        msgLabel->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(msgLabel);

        tclosebtn = new QPushButton(TalkWindowItem);
        tclosebtn->setObjectName(QStringLiteral("tclosebtn"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tclosebtn->sizePolicy().hasHeightForWidth());
        tclosebtn->setSizePolicy(sizePolicy2);
        tclosebtn->setMinimumSize(QSize(16, 16));
        tclosebtn->setMaximumSize(QSize(16, 16));

        horizontalLayout->addWidget(tclosebtn);


        retranslateUi(TalkWindowItem);

        QMetaObject::connectSlotsByName(TalkWindowItem);
    } // setupUi

    void retranslateUi(QWidget *TalkWindowItem)
    {
        TalkWindowItem->setWindowTitle(QApplication::translate("TalkWindowItem", "TalkWindowItem", Q_NULLPTR));
        headlabel->setText(QString());
        msgLabel->setText(QString());
        tclosebtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TalkWindowItem: public Ui_TalkWindowItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TALKWINDOWITEM_H
