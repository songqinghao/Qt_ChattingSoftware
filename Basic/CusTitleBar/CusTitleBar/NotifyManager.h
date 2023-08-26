#pragma once

#include <QObject>
#include "CommonUtils.h"
class NotifyManager  : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();

signals:
	void signalSkinChanged(const QColor&color);
	void signalShowTitle();
public:
	static NotifyManager*getInstance();
	//֪ͨ�������ڸı���ɫ
	void notifyOtherWindowChangedSkin(const QColor& color);
	
private:
	static NotifyManager* instance;
};
