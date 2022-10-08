#include "CustomMenu.h"
#include"CommonUtils.h"
CustomMenu::CustomMenu(QWidget *parent)
	: QMenu(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);//����͸��
	CommonUtils::loadStyleSheet(this, "Menu");
}

CustomMenu::~CustomMenu()
{
}

void CustomMenu::addCustomMenu(const QString & text, const QString & icon,const QString&name)
{
	/*Ҫ�������������������
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", QStringLiteral("��ʾ"));
	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", QStringLiteral("�˳�"));
	//�����"onshow"���󴥷����˵���ź���ִ��onShowNormal�۷���
	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowNormal(bool)));
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowQuit(bool)));
	*/
	QAction*pAction = addAction(QIcon(icon), name);
	m_menuActionMap.insert(text, pAction);
}

QAction * CustomMenu::getAction(const QString & text)
{

	return m_menuActionMap[text];
}
