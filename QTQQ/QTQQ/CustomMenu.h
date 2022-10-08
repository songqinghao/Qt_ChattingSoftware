#pragma once

#include <QMenu>
#include<QMap>
class CustomMenu : public QMenu
{
	Q_OBJECT

public:
	CustomMenu(QWidget *parent=nullptr);
	~CustomMenu();
public:
	//动作按钮文本+菜单icon+菜单文本
	void addCustomMenu(const QString&text, const QString&icon,const QString&name);
	QAction*getAction(const QString&text);
private:
	QMap<QString, QAction*>m_menuActionMap;
};
