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
	void setUserName(const QString&userName);		 //�����û���
	void setLevelPixmap(int level);					 //���õȼ�ͼƬ
	void setHeadPixmap(const QString&headPath);		 //����ͷ��
	void setStatusMenuIcon(const QString&statusPath);//����״̬
	void setSignName(const QString&signName);		 //����ǩ��

	//���Ӧ�ò���
	//appͼ��·����app����������
	QWidget*addOtherAppExtension(const QString&appPath, const QString&appName);
	void initContactree();//��ʼ����ϵ��
private:
	void initTimer();								 //��ʼ����ʱ��
	void initControl();
	QString getHeadPicturePath();
	void updateSearchStyle();//����������ʽ
	void addCompanyDeps(QTreeWidgetItem*pRootGroupItem, int DepID);
private:
	void resizeEvent(QResizeEvent*event);
	bool eventFilter(QObject*obj, QEvent*event);//�¼�������
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
	QString m_account;	//�˺Ż���qq��
//	QMap<QTreeWidgetItem*, QString>m_groupMap;//���з�����
};
