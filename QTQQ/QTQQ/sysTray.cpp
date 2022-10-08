#include "sysTray.h"
#include<Qwidget>
#include"CustomMenu.h"
sysTray::sysTray(QWidget *parent)
	:m_parent(parent)
	, QSystemTrayIcon(parent)
{
	initSystemTray();
	show();
}

sysTray::~sysTray()
{
}

void sysTray::initSystemTray()
{
	setToolTip(QStringLiteral("QQ-CAccept"));	//���ò˵����֣���������Ƶ���ʱ����ʾ�����֣�
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
}

//���ϵͳ�˵�����ʾ���˳�
void sysTray::addSystemTrayMenu()
{
	CustomMenu*customMenu = new CustomMenu(m_parent);
	//onShow�����Ϊ������+logo+��ʾ������
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", QStringLiteral("��ʾ"));
	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", QStringLiteral("�˳�"));
	//�����"onshow"���󴥷����˵���ź���ִ��onShowNormal�۷���
	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowNormal(bool)));
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowQuit(bool)));
	customMenu->exec(QCursor::pos());//�¼�ѭ��������������������������ô���Ҽ��˵���Ч��
	delete customMenu;				 //��������ĳ���˵�����˵����ͷŲ���ִ�в���
	customMenu = nullptr;
}
void sysTray::onIconActivated(QSystemTrayIcon::ActivationReason reason) 
{
	if (reason == QSystemTrayIcon::Trigger) {//���ԭ��������������
		m_parent->show();			//����������ʾ
		m_parent->activateWindow(); //������������Ϊ�����
	}
	else if (reason == QSystemTrayIcon::Context) {//���ԭ�����Ҽ����
		addSystemTrayMenu();					  //��Ӳ˵���
	}
}