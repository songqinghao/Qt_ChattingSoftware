/********************************************************************************
** Form generated from reading UI file 'CCMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCMAINWINDOW_H
#define UI_CCMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CCMainWindowClass
{
public:
    QVBoxLayout *verticalLayout_6;
    QWidget *titleWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *titleIcon;
    QSpacerItem *horizontalSpacer;
    QPushButton *palcehold1;
    QPushButton *placehold2;
    QPushButton *sysmin;
    QPushButton *sysclose;
    QHBoxLayout *horizontalLayout_3;
    QLabel *headLabel;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *nameLabel;
    QPushButton *statusBtn;
    QPushButton *levelBtn;
    QLineEdit *lineEdit;
    QWidget *appWidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QWidget *searchWidget;
    QLineEdit *searchLineEdit;
    QPushButton *searchBtn;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QTreeWidget *treeWidget;
    QWidget *bottomWidget;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *bottomLayout_up;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *bottomBtn_1;
    QPushButton *bottomBtn_2;
    QPushButton *bottomBtn_3;
    QPushButton *bottomBtn_5;
    QPushButton *bottomBtn_4;
    QPushButton *bottomBtn_6;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *bottomBtn_7;

    void setupUi(QWidget *CCMainWindowClass)
    {
        if (CCMainWindowClass->objectName().isEmpty())
            CCMainWindowClass->setObjectName(QString::fromUtf8("CCMainWindowClass"));
        CCMainWindowClass->resize(290, 680);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(4);
        sizePolicy.setHeightForWidth(CCMainWindowClass->sizePolicy().hasHeightForWidth());
        CCMainWindowClass->setSizePolicy(sizePolicy);
        CCMainWindowClass->setMinimumSize(QSize(280, 680));
        CCMainWindowClass->setMaximumSize(QSize(16777215, 16777215));
        CCMainWindowClass->setMouseTracking(false);
        verticalLayout_6 = new QVBoxLayout(CCMainWindowClass);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        titleWidget = new QWidget(CCMainWindowClass);
        titleWidget->setObjectName(QString::fromUtf8("titleWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(titleWidget->sizePolicy().hasHeightForWidth());
        titleWidget->setSizePolicy(sizePolicy1);
        titleWidget->setProperty("titleskin", QVariant(true));
        verticalLayout_2 = new QVBoxLayout(titleWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(12, -1, -1, -1);
        titleIcon = new QLabel(titleWidget);
        titleIcon->setObjectName(QString::fromUtf8("titleIcon"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(titleIcon->sizePolicy().hasHeightForWidth());
        titleIcon->setSizePolicy(sizePolicy2);
        titleIcon->setMinimumSize(QSize(42, 18));
        titleIcon->setMaximumSize(QSize(42, 18));

        horizontalLayout->addWidget(titleIcon);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        palcehold1 = new QPushButton(titleWidget);
        palcehold1->setObjectName(QString::fromUtf8("palcehold1"));
        sizePolicy2.setHeightForWidth(palcehold1->sizePolicy().hasHeightForWidth());
        palcehold1->setSizePolicy(sizePolicy2);
        palcehold1->setMinimumSize(QSize(24, 24));
        palcehold1->setMaximumSize(QSize(24, 24));

        horizontalLayout->addWidget(palcehold1);

        placehold2 = new QPushButton(titleWidget);
        placehold2->setObjectName(QString::fromUtf8("placehold2"));
        sizePolicy2.setHeightForWidth(placehold2->sizePolicy().hasHeightForWidth());
        placehold2->setSizePolicy(sizePolicy2);
        placehold2->setMinimumSize(QSize(24, 24));
        placehold2->setMaximumSize(QSize(24, 24));

        horizontalLayout->addWidget(placehold2);

        sysmin = new QPushButton(titleWidget);
        sysmin->setObjectName(QString::fromUtf8("sysmin"));
        sizePolicy2.setHeightForWidth(sysmin->sizePolicy().hasHeightForWidth());
        sysmin->setSizePolicy(sizePolicy2);
        sysmin->setMinimumSize(QSize(24, 24));
        sysmin->setMaximumSize(QSize(24, 24));

        horizontalLayout->addWidget(sysmin);

        sysclose = new QPushButton(titleWidget);
        sysclose->setObjectName(QString::fromUtf8("sysclose"));
        sizePolicy2.setHeightForWidth(sysclose->sizePolicy().hasHeightForWidth());
        sysclose->setSizePolicy(sizePolicy2);
        sysclose->setMinimumSize(QSize(24, 24));
        sysclose->setMaximumSize(QSize(24, 24));

        horizontalLayout->addWidget(sysclose);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(12, 6, 12, 6);
        headLabel = new QLabel(titleWidget);
        headLabel->setObjectName(QString::fromUtf8("headLabel"));
        sizePolicy2.setHeightForWidth(headLabel->sizePolicy().hasHeightForWidth());
        headLabel->setSizePolicy(sizePolicy2);
        headLabel->setMinimumSize(QSize(68, 68));
        headLabel->setMaximumSize(QSize(68, 68));

        horizontalLayout_3->addWidget(headLabel);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(4);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        nameLabel = new QLabel(titleWidget);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(nameLabel->sizePolicy().hasHeightForWidth());
        nameLabel->setSizePolicy(sizePolicy3);
        nameLabel->setMinimumSize(QSize(0, 22));
        nameLabel->setMaximumSize(QSize(16777215, 22));
        nameLabel->setStyleSheet(QString::fromUtf8("color:rgb(255,255,255);\n"
"font:75 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        horizontalLayout_2->addWidget(nameLabel);

        statusBtn = new QPushButton(titleWidget);
        statusBtn->setObjectName(QString::fromUtf8("statusBtn"));
        sizePolicy2.setHeightForWidth(statusBtn->sizePolicy().hasHeightForWidth());
        statusBtn->setSizePolicy(sizePolicy2);
        statusBtn->setMinimumSize(QSize(34, 22));
        statusBtn->setMaximumSize(QSize(34, 22));
        statusBtn->setProperty("hasborder", QVariant(true));

        horizontalLayout_2->addWidget(statusBtn);

        levelBtn = new QPushButton(titleWidget);
        levelBtn->setObjectName(QString::fromUtf8("levelBtn"));
        sizePolicy2.setHeightForWidth(levelBtn->sizePolicy().hasHeightForWidth());
        levelBtn->setSizePolicy(sizePolicy2);
        levelBtn->setMinimumSize(QSize(22, 16));
        levelBtn->setMaximumSize(QSize(22, 16));
        levelBtn->setProperty("hasborder", QVariant(true));

        horizontalLayout_2->addWidget(levelBtn);


        verticalLayout->addLayout(horizontalLayout_2);

        lineEdit = new QLineEdit(titleWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 22));
        lineEdit->setMaximumSize(QSize(16777215, 22));
        lineEdit->setFrame(false);

        verticalLayout->addWidget(lineEdit);

        appWidget = new QWidget(titleWidget);
        appWidget->setObjectName(QString::fromUtf8("appWidget"));
        sizePolicy3.setHeightForWidth(appWidget->sizePolicy().hasHeightForWidth());
        appWidget->setSizePolicy(sizePolicy3);
        appWidget->setMinimumSize(QSize(0, 22));
        appWidget->setMaximumSize(QSize(16777215, 22));

        verticalLayout->addWidget(appWidget);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_6->addWidget(titleWidget);

        widget = new QWidget(CCMainWindowClass);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy4);
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(1, 0, 1, 0);
        searchWidget = new QWidget(widget);
        searchWidget->setObjectName(QString::fromUtf8("searchWidget"));
        sizePolicy3.setHeightForWidth(searchWidget->sizePolicy().hasHeightForWidth());
        searchWidget->setSizePolicy(sizePolicy3);
        searchWidget->setMinimumSize(QSize(0, 30));
        searchWidget->setMaximumSize(QSize(16777215, 30));
        searchLineEdit = new QLineEdit(searchWidget);
        searchLineEdit->setObjectName(QString::fromUtf8("searchLineEdit"));
        searchLineEdit->setGeometry(QRect(0, 0, 261, 30));
        searchLineEdit->setMinimumSize(QSize(0, 30));
        searchLineEdit->setMaximumSize(QSize(16777215, 30));
        searchLineEdit->setFrame(false);
        searchBtn = new QPushButton(searchWidget);
        searchBtn->setObjectName(QString::fromUtf8("searchBtn"));
        searchBtn->setGeometry(QRect(266, 7, 16, 16));
        sizePolicy2.setHeightForWidth(searchBtn->sizePolicy().hasHeightForWidth());
        searchBtn->setSizePolicy(sizePolicy2);
        searchBtn->setMinimumSize(QSize(16, 16));
        searchBtn->setMaximumSize(QSize(16, 16));

        verticalLayout_4->addWidget(searchWidget);

        scrollArea = new QScrollArea(widget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 288, 474));
        sizePolicy4.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy4);
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(scrollAreaWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setFrameShape(QFrame::NoFrame);
        treeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        treeWidget->setIndentation(0);
        treeWidget->setHeaderHidden(true);
        treeWidget->setExpandsOnDoubleClick(false);

        verticalLayout_3->addWidget(treeWidget);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_4->addWidget(scrollArea);


        verticalLayout_6->addWidget(widget);

        bottomWidget = new QWidget(CCMainWindowClass);
        bottomWidget->setObjectName(QString::fromUtf8("bottomWidget"));
        sizePolicy3.setHeightForWidth(bottomWidget->sizePolicy().hasHeightForWidth());
        bottomWidget->setSizePolicy(sizePolicy3);
        bottomWidget->setMinimumSize(QSize(0, 60));
        bottomWidget->setMaximumSize(QSize(16777215, 60));
        bottomWidget->setProperty("bottomskin", QVariant(true));
        verticalLayout_5 = new QVBoxLayout(bottomWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(6, 6, 6, 6);
        bottomLayout_up = new QHBoxLayout();
        bottomLayout_up->setSpacing(6);
        bottomLayout_up->setObjectName(QString::fromUtf8("bottomLayout_up"));

        verticalLayout_5->addLayout(bottomLayout_up);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        bottomBtn_1 = new QPushButton(bottomWidget);
        bottomBtn_1->setObjectName(QString::fromUtf8("bottomBtn_1"));
        sizePolicy2.setHeightForWidth(bottomBtn_1->sizePolicy().hasHeightForWidth());
        bottomBtn_1->setSizePolicy(sizePolicy2);
        bottomBtn_1->setMinimumSize(QSize(24, 24));
        bottomBtn_1->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_1);

        bottomBtn_2 = new QPushButton(bottomWidget);
        bottomBtn_2->setObjectName(QString::fromUtf8("bottomBtn_2"));
        sizePolicy2.setHeightForWidth(bottomBtn_2->sizePolicy().hasHeightForWidth());
        bottomBtn_2->setSizePolicy(sizePolicy2);
        bottomBtn_2->setMinimumSize(QSize(24, 24));
        bottomBtn_2->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_2);

        bottomBtn_3 = new QPushButton(bottomWidget);
        bottomBtn_3->setObjectName(QString::fromUtf8("bottomBtn_3"));
        sizePolicy2.setHeightForWidth(bottomBtn_3->sizePolicy().hasHeightForWidth());
        bottomBtn_3->setSizePolicy(sizePolicy2);
        bottomBtn_3->setMinimumSize(QSize(24, 24));
        bottomBtn_3->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_3);

        bottomBtn_5 = new QPushButton(bottomWidget);
        bottomBtn_5->setObjectName(QString::fromUtf8("bottomBtn_5"));
        sizePolicy2.setHeightForWidth(bottomBtn_5->sizePolicy().hasHeightForWidth());
        bottomBtn_5->setSizePolicy(sizePolicy2);
        bottomBtn_5->setMinimumSize(QSize(24, 24));
        bottomBtn_5->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_5);

        bottomBtn_4 = new QPushButton(bottomWidget);
        bottomBtn_4->setObjectName(QString::fromUtf8("bottomBtn_4"));
        sizePolicy2.setHeightForWidth(bottomBtn_4->sizePolicy().hasHeightForWidth());
        bottomBtn_4->setSizePolicy(sizePolicy2);
        bottomBtn_4->setMinimumSize(QSize(24, 24));
        bottomBtn_4->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_4);

        bottomBtn_6 = new QPushButton(bottomWidget);
        bottomBtn_6->setObjectName(QString::fromUtf8("bottomBtn_6"));
        sizePolicy2.setHeightForWidth(bottomBtn_6->sizePolicy().hasHeightForWidth());
        bottomBtn_6->setSizePolicy(sizePolicy2);
        bottomBtn_6->setMinimumSize(QSize(24, 24));
        bottomBtn_6->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_6);

        horizontalSpacer_2 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        bottomBtn_7 = new QPushButton(bottomWidget);
        bottomBtn_7->setObjectName(QString::fromUtf8("bottomBtn_7"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(bottomBtn_7->sizePolicy().hasHeightForWidth());
        bottomBtn_7->setSizePolicy(sizePolicy5);
        bottomBtn_7->setMinimumSize(QSize(63, 24));
        bottomBtn_7->setMaximumSize(QSize(24, 24));

        horizontalLayout_5->addWidget(bottomBtn_7);


        verticalLayout_5->addLayout(horizontalLayout_5);

        verticalLayout_5->setStretch(0, 1);
        verticalLayout_5->setStretch(1, 1);

        verticalLayout_6->addWidget(bottomWidget);


        retranslateUi(CCMainWindowClass);

        QMetaObject::connectSlotsByName(CCMainWindowClass);
    } // setupUi

    void retranslateUi(QWidget *CCMainWindowClass)
    {
        CCMainWindowClass->setWindowTitle(QCoreApplication::translate("CCMainWindowClass", "CCMainWindow", nullptr));
        titleIcon->setText(QString());
        palcehold1->setText(QString());
        placehold2->setText(QString());
        sysmin->setText(QString());
        sysclose->setText(QString());
        headLabel->setText(QString());
        nameLabel->setText(QCoreApplication::translate("CCMainWindowClass", "\345\245\207\345\245\207-\345\267\245\344\275\234\345\217\267", nullptr));
        statusBtn->setText(QString());
        levelBtn->setText(QString());
        lineEdit->setPlaceholderText(QCoreApplication::translate("CCMainWindowClass", "\344\270\252\346\200\247\347\255\276\345\220\215", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("CCMainWindowClass", "\346\220\234\347\264\242\350\201\224\347\263\273\344\272\272\343\200\201\347\276\244", nullptr));
        searchBtn->setText(QString());
        bottomBtn_1->setText(QString());
        bottomBtn_2->setText(QString());
        bottomBtn_3->setText(QString());
        bottomBtn_5->setText(QString());
        bottomBtn_4->setText(QString());
        bottomBtn_6->setText(QString());
        bottomBtn_7->setText(QCoreApplication::translate("CCMainWindowClass", "\345\272\224\347\224\250\345\256\235", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CCMainWindowClass: public Ui_CCMainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCMAINWINDOW_H
