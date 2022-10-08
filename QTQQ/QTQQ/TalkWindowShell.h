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
	COMPANY=0,//公司群
	PERSONELGROUP,//人事部
	DEVELOPMENTGROUP,//研发部
	MARKETGROUP,//市场部
	PTOP//同事间单独聊天
};
class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

private:
	void initControl();//初始化控件
	void initTcpSocket();//初始化tcp
	void initUdpSocket();//初始化udp
	void getEmployeesID(QStringList&employeesList);//获取所有员工的qq号
	bool createJSFile(QStringList&employeesList);//创建js文件
	void handleReceiveMsg(int senderEmployeeID,int msgType,QString strMsg);//处理接受到的数据

public:
	void addTalkWindow(TalkWindow*talkWindow, TalkWindowItem*talkWindowItem, const QString&uid/*,GroupType grouptype*/);//添加新的聊天窗口
	void setCurrentWidget(QWidget*widget);//设置哪个为当前聊天窗口
	const QMap<QListWidgetItem*, QWidget*>&getTalkWindowItemMap()const;
public slots:
	void onEmotionBtnClicked(bool);	//表情按钮点击后执行的槽函数
	//客户端发送tcp数据
	//参数：数据+消息类型+用于文件发送
	void updateSendTcpMsg(QString&strData, int& msgType, QString filename = "");
private slots:
	void onTalkWindowItemClicked(QListWidgetItem*item);	//左侧列表点击后执行的槽函数
	void onEmotionItemClicked(int emotionNum);//表情被选中
	void processPendingData();//处理广播收到的数据
private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*>m_talkwindowItemMap;//打开的聊天窗口的映射（左边聊天项和右边的聊天窗口形成映射）
	EmotionWindow*m_emotionWindow;//表情窗口


private:
	QTcpSocket*m_tcpClientSocket;//客户端TCP
	QUdpSocket*m_udpReceive;	 //udp接收端
};
