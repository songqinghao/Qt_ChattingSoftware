#pragma once

#include <QWebEngineView>
#include<QDomNode>
class MsgHtmlObj :public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)//���˷�������Ϣ
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)//�Լ����͵���Ϣ
public:
	MsgHtmlObj(QObject*parent,QString msgLpic="");//����Ϣ������ͷ���·��
signals:
	void signalMsgHtml(const QString&html);

private:
	void initHtmlTmpl();//��ʼ��������ҳ
	QString getMsgTmplHtml(const QString&code);//���س�ʼ������ҳ
private:
	QString m_msgLHtmlTmpl;//���˷�������Ϣ
	QString m_msgRHtmlTmpl;//�Լ����͵���Ϣ
	QString m_msgLPicPath;
};

//��ҳ��
class MsgWebPage :public QWebEnginePage
{
	Q_OBJECT
public:
	//ֻ����qrc�е�Html����
	MsgWebPage(QObject*parent = nullptr) :QWebEnginePage(parent) 
	{
			
	}
protected:
	//�������������
	bool acceptNavigationRequest(const QUrl&url,NavigationType type,bool isMainFrame);//���ܵ�������
};



class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	//QWebEngineView��ͼ������ʾ��ҳ����
	MsgWebView(QWidget *parent);
	~MsgWebView();

	void appendMsg(const QString&html,QString strObj = "0");  //׷����Ϣ
private:
	QList<QStringList>parseHtml(const QString&html);//����Html
	QList<QStringList>parseDocNode(const QDomNode&node);//�����ڵ�(qt�����е�DOM�ڵ㣨���ԡ�˵�����ı���������ʹ��QDomNode���б�ʾ)
signals:
	void signalSendMsg(QString&strData, int& msgType, QString sFile = "");
private:
	MsgHtmlObj*m_msgHtmlObj;
	QWebChannel*m_channel;//����ͨ��
};
