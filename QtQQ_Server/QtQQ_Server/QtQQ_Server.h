#pragma once

#include <QtWidgets/QDialog>
#include "ui_QtQQ_Server.h"
#include"TcpServer.h"
#include<QSqlQueryModel>
#include<qmap.h>
#include<QTimer>
#include<QUdpSocket>
const int gtcpPort = 8888;
const int gudpPort = 6666;
class QtQQ_Server : public QDialog
{
	Q_OBJECT

public:
	QtQQ_Server(QWidget *parent = Q_NULLPTR);
private:
	void initComboBoxData();//初始化组合框数据
	void initTcpSocket();	//初始化tcp
	void initUdpSocket();	//初始化udp
	void setDepNameMap();
	void setStatusMap();
	void setOnlineMap();
	bool connectMySql();
	int getComDepID();//获取公司群qq号
	//更新表的数据
	void updateTableData(int depID=0,int employeeID=0);
private slots:
	void onUDPbroadMsg(QByteArray&btData);
	void onRefresh();
	void on_queryDepartmentBtn_clicked();//根据群qq号查找员工，点击信号与槽函数自动连接
	void on_queryIDBtn_clicked();		 //根据员工qq号筛选
	void on_logoutBtn_clicked();		 //注销员工qq号
	void on_selectPictureBtn_clicked();  //新增员工时的寸照
	void on_addBtn_clicked();			 //新增员工
private:
	Ui::QtQQ_ServerClass ui;
	QTimer*m_timer;//定时刷新数据
	QSqlQueryModel m_queryInfoModel;//查询所有员工的信息模型
	TcpServer*m_tcpServer;  //tcp服务端

	QUdpSocket* m_udpSend;//udp广播
	int m_compDepID;//公司群qq号
	int m_depID;//部门qq号
	int m_employeeID;//员工qq号
	QString m_pixPath;//头像路径


	QMap<QString, QString>m_statusMap;//状态
	QMap<QString, QString>m_depNameMap;//部门名称
	QMap<QString, QString>m_onlineMap;//在线状态

};
