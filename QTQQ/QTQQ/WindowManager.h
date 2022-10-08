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
	QWidget*findWindowName(const QString&qsWindowName);//����ID��item->data(0, Qt::UserRole + 1).toString()
	void deleteWindowName(const QString&qsWindowName);
	void addWindowName(const QString&qsWindowName,QWidget*qWidget);
	static WindowManager*getInstance();
	TalkWindowShell*getTalkWindowShell();
	QString getCreatingTalkId();		   //��ȡ���ڴ����Ĵ���ID
	void addNewTalkWindow(const QString&uid/*,GroupType groupType=COMPANY,const QString&strPeople=""*/);
private:
	TalkWindowShell*m_talkwindowshell;
	QMap<QString, QWidget*>m_windowMap;
	QString m_strCreateingTalkId = "";//���ڴ��������촰�ڣ�qq�ţ�
};
