#pragma once

#include <QWebEngineView>
#include<QDomNode>
class MsgHtmlObj :public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)//别人发来的消息
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)//自己发送的消息
public:
	MsgHtmlObj(QObject*parent,QString msgLpic="");//发信息来的人头像的路径
signals:
	void signalMsgHtml(const QString&html);

private:
	void initHtmlTmpl();//初始化聊天网页
	QString getMsgTmplHtml(const QString&code);//返回初始化的网页
private:
	QString m_msgLHtmlTmpl;//别人发来的消息
	QString m_msgRHtmlTmpl;//自己发送的消息
	QString m_msgLPicPath;
};

//网页类
class MsgWebPage :public QWebEnginePage
{
	Q_OBJECT
public:
	//只接受qrc中的Html类型
	MsgWebPage(QObject*parent = nullptr) :QWebEnginePage(parent) 
	{
			
	}
protected:
	//用于请求的限制
	bool acceptNavigationRequest(const QUrl&url,NavigationType type,bool isMainFrame);//接受导航请求
};



class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	//QWebEngineView视图用于显示网页内容
	MsgWebView(QWidget *parent);
	~MsgWebView();

	void appendMsg(const QString&html,QString strObj = "0");  //追加消息
private:
	QList<QStringList>parseHtml(const QString&html);//解析Html
	QList<QStringList>parseDocNode(const QDomNode&node);//解析节点(qt中所有的DOM节点（属性、说明、文本）都可以使用QDomNode进行表示)
signals:
	void signalSendMsg(QString&strData, int& msgType, QString sFile = "");
private:
	MsgHtmlObj*m_msgHtmlObj;
	QWebChannel*m_channel;//网络通道
};
