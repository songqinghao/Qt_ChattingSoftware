#pragma once

#include <QtWidgets/QWidget>
#include "ui_CCMainWindow.h"
#include"basicwindow.h"
class QTreeWidgetItem;
class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QString account,bool isAccountLogin,QWidget *parent = Q_NULLPTR);
	~CCMainWindow();
public:
	void setUserName(const QString&userName);		 //设置用户名
	void setLevelPixmap(int level);					 //设置等级图片
	void setHeadPixmap(const QString&headPath);		 //设置头像
	void setStatusMenuIcon(const QString&statusPath);//设置状态
	void setSignName(const QString&signName);		 //设置签名

	//添加应用部件
	//app图像路径，app部件对象名
	QWidget*addOtherAppExtension(const QString&appPath, const QString&appName);
	void initContactree();//初始化联系人
private:
	void initTimer();								 //初始化计时器
	void initControl();
	QString getHeadPicturePath();
	void updateSearchStyle();//更新搜索样式
	void addCompanyDeps(QTreeWidgetItem*pRootGroupItem, int DepID);
private:
	void resizeEvent(QResizeEvent*event);
	bool eventFilter(QObject*obj, QEvent*event);//事件过滤器
	void mousePressEvent(QMouseEvent*event);
private slots:
	void onItemClicked(QTreeWidgetItem*item, int column);
	void onItemExpanded(QTreeWidgetItem*item);
	void onItemCollapsed(QTreeWidgetItem*item);
	void onItemDoubleClicked(QTreeWidgetItem*item, int column);
	void onAppIconClicked();
private:
    Ui::CCMainWindowClass ui;
	bool m_isAccountLogin;
	QString m_account;	//账号或者qq号
//	QMap<QTreeWidgetItem*, QString>m_groupMap;//所有分组项
};
