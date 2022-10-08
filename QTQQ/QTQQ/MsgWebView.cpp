#include "MsgWebView.h"
#include<QFile>
#include<QMessageBox>
#include<QJsonObject>
#include<QJsonDocument>
#include<QWebChannel>
#include"TalkWindowShell.h"
#include"WindowManager.h"
#include<QSqlQueryModel>
extern QString gstrLoginHeadPath;
MsgHtmlObj::MsgHtmlObj(QObject*parent, QString msgLpic) :QObject(parent)
{
	m_msgLPicPath = msgLpic;
	initHtmlTmpl();
}

void MsgHtmlObj::initHtmlTmpl()
{
	//初始化一个左一个右html文件初始化
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgLHtmlTmpl.replace("%1", m_msgLPicPath);
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
	m_msgRHtmlTmpl.replace("%1", gstrLoginHeadPath);
}

QString MsgHtmlObj::getMsgTmplHtml(const QString & code)
{
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;
	if (file.isOpen())
	{
		strData = QLatin1String(file.readAll());
	}
	else {
		QMessageBox::information(nullptr, "Tips", "Failed to init html");
		return QString("");
	}
	file.close();
	return strData;
}

//url不只是网址还可以存放路径
bool MsgWebPage::acceptNavigationRequest(const QUrl&url, NavigationType type, bool isMainFrame)
{
	//只接受qrc中的Html类型-》qrc:/*.html格式
	if (url.scheme() == QString("qrc")) {//判断url类型
		return true;
	}
	else {
		return false;
	}
}
//QWebEngineView视图用于显示网页内容
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent), m_channel(new QWebChannel(this))
{
	MsgWebPage*page = new MsgWebPage(this);
	setPage(page);//设置为当前网页

	m_msgHtmlObj = new MsgHtmlObj(this);		//创建对象，用于注册到通道中
	m_channel->registerObject("external0", m_msgHtmlObj);//ID+对象

	
	TalkWindowShell*talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowShell::updateSendTcpMsg);
	//获取正在创建的聊天窗口ID（QQ号）
	QString strTalkId = WindowManager::getInstance()->getCreatingTalkId();

	QSqlQueryModel queryEmployeeModel;
	QString strEmployeeID, strPicturePath;//员工qq号+员工头像路径
	QString strExternal;
	bool isGroupTalk = false;


	//获取公司群ID
	queryEmployeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'")
		.arg(QStringLiteral("公司群")));
	QModelIndex companyIndex = queryEmployeeModel.index(0, 0);
	QString strCompanyID = queryEmployeeModel.data(companyIndex).toString();

	if (strTalkId == strCompanyID) {//公司群聊
		isGroupTalk = true;
		queryEmployeeModel.setQuery("SELECT employeeID,picture FROM tab_employees WHERE status=1");
	}
	else {//普通群聊或者单聊
		if (strTalkId.length() == 4) {//其他群聊
			isGroupTalk = true;
			queryEmployeeModel.setQuery(QString("SELECT employeeID,picture FROM tab_employees WHERE status=1 AND departmentID=%1").arg(strTalkId));
		}
		else {//单独聊天
			queryEmployeeModel.setQuery(QString("SELECT picture FROM tab_employees WHERE status=1 AND employeeID=%1").arg(strTalkId));
			QModelIndex index = queryEmployeeModel.index(0, 0);
			strPicturePath = queryEmployeeModel.data(index).toString();

			strExternal = "external_" + strTalkId;
			MsgHtmlObj* msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}

	if (isGroupTalk)
	{
		QModelIndex employeeModelIndex, pictureModelIndex;
		int rows = queryEmployeeModel.rowCount();
		for (int i = 0; i < rows; i++) {
			employeeModelIndex = queryEmployeeModel.index(i, 0);
			pictureModelIndex = queryEmployeeModel.index(i, 1);

			strEmployeeID = queryEmployeeModel.data(employeeModelIndex).toString();
			strPicturePath = queryEmployeeModel.data(pictureModelIndex).toString();

			strExternal = "external_" + strEmployeeID;
			MsgHtmlObj*msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);

		}

	}

	this->page()->setWebChannel(m_channel);		//设置网络通道
	//初始化收消息页面
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));//初始化收消息页面
}

MsgWebView::~MsgWebView()
{
}

//QQ号默认自己0
void MsgWebView::appendMsg(const QString&html, QString strObj)
{
	QJsonObject msgObj; //用于发送消息
	QString qsMsg;
	const QList<QStringList>msgLst = parseHtml(html);//解析html

	int msgType = 1;//消息类型：0为表情，1为文本，2为文件
	bool isImageMsg = false;//默认不是图片
	QString strData;//055 008 155:3个数字来表达表情的宽度
	int imageNum = 0;//表情数目为0

	for (int i = 0; i < msgLst.size(); i++) {
		if (msgLst.at(i).at(0) == "img") {
			QString imagePath = msgLst.at(i).at(1);
			/*区分两种形式
			* "qrc:/MainWindow/.../77.png"
			* ":/MainWindow/.../77.png"
			*/
			QPixmap pixmap;

			//获取表情名称的位置
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			isImageMsg = true;
			//获取表情名称
			QString strEmotionName = imagePath.mid(pos);//就是截取到xx.png
			strEmotionName.replace(".png", "");//将xx.png的png给去掉

			//根据表情名称的长度来进行设置表情数据，不足3位则补足3位，如23.png则数据为023.png
			int emotionNameL = strEmotionName.length();//计算表情长度
			if (emotionNameL == 1) {
				strData = strData + "00" + strEmotionName;
			}
			else if (emotionNameL == 2) {
				strData = strData + "0" + strEmotionName;
			}
			else if (emotionNameL == 3) {
				strData = strData + strEmotionName;
			}
			msgType = 0;//表情消息
			imageNum++;

			//加载表情图片
			if (imagePath.left(3) == "qrc") {//如果前3个字符是qrc
				pixmap.load(imagePath.mid(3));//截取qrc后面的路径
			}
			else {
				pixmap.load(imagePath);
			}


			//表情图片html格式文本组合
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());

			qsMsg += imgPath;
		}
		else if (msgLst.at(i).at(0) == "text") {
			qsMsg += msgLst.at(i).at(1);
			strData = qsMsg;
		}
	}
	msgObj.insert("MSG", qsMsg);//key+值
	//Json对象转换为Json文档返回utf-8格式的字符串
	const QString&Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);

	if (strObj == "0") {//自己发信息
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));//运行java脚本
		if (isImageMsg) {
			strData = QString::number(imageNum) + "images" + strData;
		}
		//发送信号
		emit signalSendMsg(strData, msgType);
	}
	else {//来消息
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));//运行java脚本
	}
	//	this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));//跑java脚本

	//	emit signalSendMsg(strData, msgType);
}

QList<QStringList> MsgWebView::parseHtml(const QString & html)
{
	QDomDocument doc;
	doc.setContent(html);//将html文件转换为dom文件
	const QDomElement&root = doc.documentElement();//节点元素
	//获取身体节点
	const QDomNode&node = root.firstChildElement("body");
	//返回节点的解析
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node)
{
	QList<QStringList>attribute;
	const QDomNodeList&list = node.childNodes();//返回所有左右子节点
	for (int i = 0; i < list.count(); i++) {
		const QDomNode&node = list.at(i);

		if (node.isElement()) {//判断节点是不是元素
			//转换元素
			const QDomElement&element = node.toElement();
			//判断标签名
			if (element.tagName() == "img") {
				QStringList attributeList;
				attributeList << "img" << element.attribute("src");
				attribute << attributeList;
			}
			if (element.tagName() == "span") {
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;
			}

			//如果还有子节点再进行解析
			if (node.hasChildNodes()) {
				attribute << parseDocNode(node);
			}
		}
	}
	return attribute;
}
