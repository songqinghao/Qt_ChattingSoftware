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
	void initComboBoxData();//��ʼ����Ͽ�����
	void initTcpSocket();	//��ʼ��tcp
	void initUdpSocket();	//��ʼ��udp
	void setDepNameMap();
	void setStatusMap();
	void setOnlineMap();
	bool connectMySql();
	int getComDepID();//��ȡ��˾Ⱥqq��
	//���±������
	void updateTableData(int depID=0,int employeeID=0);
private slots:
	void onUDPbroadMsg(QByteArray&btData);
	void onRefresh();
	void on_queryDepartmentBtn_clicked();//����Ⱥqq�Ų���Ա��������ź���ۺ����Զ�����
	void on_queryIDBtn_clicked();		 //����Ա��qq��ɸѡ
	void on_logoutBtn_clicked();		 //ע��Ա��qq��
	void on_selectPictureBtn_clicked();  //����Ա��ʱ�Ĵ���
	void on_addBtn_clicked();			 //����Ա��
private:
	Ui::QtQQ_ServerClass ui;
	QTimer*m_timer;//��ʱˢ������
	QSqlQueryModel m_queryInfoModel;//��ѯ����Ա������Ϣģ��
	TcpServer*m_tcpServer;  //tcp�����

	QUdpSocket* m_udpSend;//udp�㲥
	int m_compDepID;//��˾Ⱥqq��
	int m_depID;//����qq��
	int m_employeeID;//Ա��qq��
	QString m_pixPath;//ͷ��·��


	QMap<QString, QString>m_statusMap;//״̬
	QMap<QString, QString>m_depNameMap;//��������
	QMap<QString, QString>m_onlineMap;//����״̬

};
