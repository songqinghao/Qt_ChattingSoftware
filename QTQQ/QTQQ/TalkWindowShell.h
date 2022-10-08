#pragma once

#include <QWidget>
#include "ui_TalkWindowShell.h"
#include"basicwindow.h"
#include<QMap>
#include"CommonUtils.h"
#include<QTcpSocket>
#include<QUdpSocket>
class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;
enum GroupType {
	COMPANY=0,//��˾Ⱥ
	PERSONELGROUP,//���²�
	DEVELOPMENTGROUP,//�з���
	MARKETGROUP,//�г���
	PTOP//ͬ�¼䵥������
};
class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

private:
	void initControl();//��ʼ���ؼ�
	void initTcpSocket();//��ʼ��tcp
	void initUdpSocket();//��ʼ��udp
	void getEmployeesID(QStringList&employeesList);//��ȡ����Ա����qq��
	bool createJSFile(QStringList&employeesList);//����js�ļ�
	void handleReceiveMsg(int senderEmployeeID,int msgType,QString strMsg);//������ܵ�������

public:
	void addTalkWindow(TalkWindow*talkWindow, TalkWindowItem*talkWindowItem, const QString&uid/*,GroupType grouptype*/);//����µ����촰��
	void setCurrentWidget(QWidget*widget);//�����ĸ�Ϊ��ǰ���촰��
	const QMap<QListWidgetItem*, QWidget*>&getTalkWindowItemMap()const;
public slots:
	void onEmotionBtnClicked(bool);	//���鰴ť�����ִ�еĲۺ���
	//�ͻ��˷���tcp����
	//����������+��Ϣ����+�����ļ�����
	void updateSendTcpMsg(QString&strData, int& msgType, QString filename = "");
private slots:
	void onTalkWindowItemClicked(QListWidgetItem*item);	//����б�����ִ�еĲۺ���
	void onEmotionItemClicked(int emotionNum);//���鱻ѡ��
	void processPendingData();//����㲥�յ�������
private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*>m_talkwindowItemMap;//�򿪵����촰�ڵ�ӳ�䣨�����������ұߵ����촰���γ�ӳ�䣩
	EmotionWindow*m_emotionWindow;//���鴰��


private:
	QTcpSocket*m_tcpClientSocket;//�ͻ���TCP
	QUdpSocket*m_udpReceive;	 //udp���ն�
};
