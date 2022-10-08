#pragma once

#include <QObject>
#include"TalkWindowShell.h"

class WindowManager : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();
public:
	QWidget*findWindowName(const QString&qsWindowName);//传入ID：item->data(0, Qt::UserRole + 1).toString()
	void deleteWindowName(const QString&qsWindowName);
	void addWindowName(const QString&qsWindowName,QWidget*qWidget);
	static WindowManager*getInstance();
	TalkWindowShell*getTalkWindowShell();
	QString getCreatingTalkId();		   //获取正在创建的窗口ID
	void addNewTalkWindow(const QString&uid/*,GroupType groupType=COMPANY,const QString&strPeople=""*/);
private:
	TalkWindowShell*m_talkwindowshell;
	QMap<QString, QWidget*>m_windowMap;
	QString m_strCreateingTalkId = "";//正在创建的聊天窗口（qq号）
};
