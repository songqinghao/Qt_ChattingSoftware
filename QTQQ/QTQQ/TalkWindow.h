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
	//添加表情图片
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString&name);
	QString getTalkId();
private:
	void initControl();		
	void initGroupTalkStatus();//初始化群聊状态
	int getCompDepID();	  //获取公司的ID号
	//void initCompanyTalk(); //初始化公司群聊天
	//void initPersonelTalk();//初始化人事部聊天
	//void initMarketTalk();	//初始化市场部聊天
	//void initDevelopTalk(); //初始化研发部聊天
	void initTalkWindow();	//初始化群聊
	void initPtoPtalk();    //初始化单聊
	void addPeopInfo(QTreeWidgetItem*pRootGroupItem,int employeeID);		//添加人
private slots:
	void onFileOpenBtnClicked(bool);
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem*item,int column);
private:
	Ui::TalkWindow ui;
	//保存当前聊天窗口的ID
	QString m_talkId;
//	GroupType m_groupType;
	QMap<QTreeWidgetItem*, QString>m_groupPeopleMap;//所有分组联系人姓名
	bool m_isGroupTalk;//是否为群聊

	friend class TalkWindowShell;
};
