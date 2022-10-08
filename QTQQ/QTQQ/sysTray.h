#pragma once

#include <QSystemTrayIcon>
#include<QSystemTrayIcon>
class sysTray : public QSystemTrayIcon
{
	Q_OBJECT

public:
	sysTray(QWidget *parent);
	~sysTray();
public slots:
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);
private:
	void initSystemTray();
	void addSystemTrayMenu();
private:
	QWidget*m_parent;//���ڱ��游�ര��
};
