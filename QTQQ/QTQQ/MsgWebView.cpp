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
	//��ʼ��һ����һ����html�ļ���ʼ��
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

//url��ֻ����ַ�����Դ��·��
bool MsgWebPage::acceptNavigationRequest(const QUrl&url, NavigationType type, bool isMainFrame)
{
	//ֻ����qrc�е�Html����-��qrc:/*.html��ʽ
	if (url.scheme() == QString("qrc")) {//�ж�url����
		return true;
	}
	else {
		return false;
	}
}
//QWebEngineView��ͼ������ʾ��ҳ����
MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent), m_channel(new QWebChannel(this))
{
	MsgWebPage*page = new MsgWebPage(this);
	setPage(page);//����Ϊ��ǰ��ҳ

	m_msgHtmlObj = new MsgHtmlObj(this);		//������������ע�ᵽͨ����
	m_channel->registerObject("external0", m_msgHtmlObj);//ID+����

	
	TalkWindowShell*talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowShell::updateSendTcpMsg);
	//��ȡ���ڴ��������촰��ID��QQ�ţ�
	QString strTalkId = WindowManager::getInstance()->getCreatingTalkId();

	QSqlQueryModel queryEmployeeModel;
	QString strEmployeeID, strPicturePath;//Ա��qq��+Ա��ͷ��·��
	QString strExternal;
	bool isGroupTalk = false;


	//��ȡ��˾ȺID
	queryEmployeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'")
		.arg(QStringLiteral("��˾Ⱥ")));
	QModelIndex companyIndex = queryEmployeeModel.index(0, 0);
	QString strCompanyID = queryEmployeeModel.data(companyIndex).toString();

	if (strTalkId == strCompanyID) {//��˾Ⱥ��
		isGroupTalk = true;
		queryEmployeeModel.setQuery("SELECT employeeID,picture FROM tab_employees WHERE status=1");
	}
	else {//��ͨȺ�Ļ��ߵ���
		if (strTalkId.length() == 4) {//����Ⱥ��
			isGroupTalk = true;
			queryEmployeeModel.setQuery(QString("SELECT employeeID,picture FROM tab_employees WHERE status=1 AND departmentID=%1").arg(strTalkId));
		}
		else {//��������
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

	this->page()->setWebChannel(m_channel);		//��������ͨ��
	//��ʼ������Ϣҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));//��ʼ������Ϣҳ��
}

MsgWebView::~MsgWebView()
{
}

//QQ��Ĭ���Լ�0
void MsgWebView::appendMsg(const QString&html, QString strObj)
{
	QJsonObject msgObj; //���ڷ�����Ϣ
	QString qsMsg;
	const QList<QStringList>msgLst = parseHtml(html);//����html

	int msgType = 1;//��Ϣ���ͣ�0Ϊ���飬1Ϊ�ı���2Ϊ�ļ�
	bool isImageMsg = false;//Ĭ�ϲ���ͼƬ
	QString strData;//055 008 155:3��������������Ŀ��
	int imageNum = 0;//������ĿΪ0

	for (int i = 0; i < msgLst.size(); i++) {
		if (msgLst.at(i).at(0) == "img") {
			QString imagePath = msgLst.at(i).at(1);
			/*����������ʽ
			* "qrc:/MainWindow/.../77.png"
			* ":/MainWindow/.../77.png"
			*/
			QPixmap pixmap;

			//��ȡ�������Ƶ�λ��
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			isImageMsg = true;
			//��ȡ��������
			QString strEmotionName = imagePath.mid(pos);//���ǽ�ȡ��xx.png
			strEmotionName.replace(".png", "");//��xx.png��png��ȥ��

			//���ݱ������Ƶĳ������������ñ������ݣ�����3λ����3λ����23.png������Ϊ023.png
			int emotionNameL = strEmotionName.length();//������鳤��
			if (emotionNameL == 1) {
				strData = strData + "00" + strEmotionName;
			}
			else if (emotionNameL == 2) {
				strData = strData + "0" + strEmotionName;
			}
			else if (emotionNameL == 3) {
				strData = strData + strEmotionName;
			}
			msgType = 0;//������Ϣ
			imageNum++;

			//���ر���ͼƬ
			if (imagePath.left(3) == "qrc") {//���ǰ3���ַ���qrc
				pixmap.load(imagePath.mid(3));//��ȡqrc�����·��
			}
			else {
				pixmap.load(imagePath);
			}


			//����ͼƬhtml��ʽ�ı����
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());

			qsMsg += imgPath;
		}
		else if (msgLst.at(i).at(0) == "text") {
			qsMsg += msgLst.at(i).at(1);
			strData = qsMsg;
		}
	}
	msgObj.insert("MSG", qsMsg);//key+ֵ
	//Json����ת��ΪJson�ĵ�����utf-8��ʽ���ַ���
	const QString&Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);

	if (strObj == "0") {//�Լ�����Ϣ
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));//����java�ű�
		if (isImageMsg) {
			strData = QString::number(imageNum) + "images" + strData;
		}
		//�����ź�
		emit signalSendMsg(strData, msgType);
	}
	else {//����Ϣ
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));//����java�ű�
	}
	//	this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));//��java�ű�

	//	emit signalSendMsg(strData, msgType);
}

QList<QStringList> MsgWebView::parseHtml(const QString & html)
{
	QDomDocument doc;
	doc.setContent(html);//��html�ļ�ת��Ϊdom�ļ�
	const QDomElement&root = doc.documentElement();//�ڵ�Ԫ��
	//��ȡ����ڵ�
	const QDomNode&node = root.firstChildElement("body");
	//���ؽڵ�Ľ���
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode & node)
{
	QList<QStringList>attribute;
	const QDomNodeList&list = node.childNodes();//�������������ӽڵ�
	for (int i = 0; i < list.count(); i++) {
		const QDomNode&node = list.at(i);

		if (node.isElement()) {//�жϽڵ��ǲ���Ԫ��
			//ת��Ԫ��
			const QDomElement&element = node.toElement();
			//�жϱ�ǩ��
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

			//��������ӽڵ��ٽ��н���
			if (node.hasChildNodes()) {
				attribute << parseDocNode(node);
			}
		}
	}
	return attribute;
}
