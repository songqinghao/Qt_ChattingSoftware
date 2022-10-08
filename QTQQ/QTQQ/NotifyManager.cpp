#include "NotifyManager.h"
NotifyManager* NotifyManager::instance = nullptr;

NotifyManager::NotifyManager() :QObject(nullptr)
{

}

NotifyManager::~NotifyManager()
{
}


NotifyManager* NotifyManager::getInstance() {
	if (instance == nullptr) {
		instance = new NotifyManager();
	}
	return instance;
}

void NotifyManager::notifyOtherWindowChangedSkin(const QColor&color) {
	emit signalSkinChanged(color);
	CommonUtils::setDefaultSkinColor(color);//CommoUtils类配置文件里的颜色重写
}
