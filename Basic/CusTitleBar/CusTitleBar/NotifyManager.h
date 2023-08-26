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
	//通知其他窗口改变颜色
	void notifyOtherWindowChangedSkin(const QColor& color);
	
private:
	static NotifyManager* instance;
};
