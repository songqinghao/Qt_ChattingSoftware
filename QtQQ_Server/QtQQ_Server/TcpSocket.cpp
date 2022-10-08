#include "TcpSocket.h"

TcpSocket::TcpSocket()
{
}


TcpSocket::~TcpSocket()
{
}

void TcpSocket::run()
{
	//设置描述符
	m_descriptor = this->socketDescriptor();

	connect(this, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
	connect(this, SIGNAL(disconnected()), this, SLOT(onClientDisconnect()));
}

//
void TcpSocket::onClientDisconnect()
{
	emit signalClientDisconnect(m_descriptor);
}


void TcpSocket::onReceiveData()
{
	//读取收到的所有信息
	QByteArray buffer=this->readAll();
	if (!buffer.isEmpty()) {
		QString strData = QString::fromLocal8Bit(buffer);

		//发射接受到了客户端数据的信号
		emit signalGetDataFromClient(buffer, m_descriptor);
	}
}
