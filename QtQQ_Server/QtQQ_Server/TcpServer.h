#pragma once

#include <QTcpServer>

class TcpServer : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(int port);
	~TcpServer();
public:
	bool run();//监听指定端口
protected:
	void incomingConnection(qintptr socketDescriptor);//pintptr和int是一样的就是新旧问题
signals:
	void signalTcpMsgComes(QByteArray&);
private slots:
	//处理数据
	void SocketDataProcessing(QByteArray&SendData, int descriptor);
	//断开连接处理
	void SocketDisconnected(int descriptor);
private:
	int m_port;//端口号
	QList<QTcpSocket*>m_tcpSocketConnectList;
};
