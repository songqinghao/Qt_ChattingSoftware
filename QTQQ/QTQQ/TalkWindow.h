#pragma once

#include <QWidget>
#include "ui_TalkWindow.h"
#include"TalkWindowShell.h"
class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent,const QString&uid/*,GroupType groupType*/);
	~TalkWindow();
public:
	//��ӱ���ͼƬ
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString&name);
	QString getTalkId();
private:
	void initControl();		
	void initGroupTalkStatus();//��ʼ��Ⱥ��״̬
	int getCompDepID();	  //��ȡ��˾��ID��
	//void initCompanyTalk(); //��ʼ����˾Ⱥ����
	//void initPersonelTalk();//��ʼ�����²�����
	//void initMarketTalk();	//��ʼ���г�������
	//void initDevelopTalk(); //��ʼ���з�������
	void initTalkWindow();	//��ʼ��Ⱥ��
	void initPtoPtalk();    //��ʼ������
	void addPeopInfo(QTreeWidgetItem*pRootGroupItem,int employeeID);		//�����
private slots:
	void onFileOpenBtnClicked(bool);
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem*item,int column);
private:
	Ui::TalkWindow ui;
	//���浱ǰ���촰�ڵ�ID
	QString m_talkId;
//	GroupType m_groupType;
	QMap<QTreeWidgetItem*, QString>m_groupPeopleMap;//���з�����ϵ������
	bool m_isGroupTalk;//�Ƿ�ΪȺ��

	friend class TalkWindowShell;
};
