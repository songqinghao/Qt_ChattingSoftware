#include "TalkWindowShell.h"
#include"EmotionWindow.h"
#include<qlistwidget.h>
#include"CommonUtils.h"
#include<QListWidget>
#include"TalkWindow.h"
#include"TalkWindowItem.h"
#include<QSqlQueryModel>
#include<qmessagebox.h>
#include<QFile>
#include<QSqlQuery>
#include"WindowManager.h"
#include"ReceiveFile.h"
const int gtcpPort = 8888;
const int gudpPort = 6666;
QString gfileName;	//�ļ�����
QString gfileData;	//�ļ�����

extern QString gLoginEmployeeID;
TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//������ر�ʱ��Դ����ʱ����
	initControl();
	//��ʼ��tcp
	initTcpSocket();
	initUdpSocket();
	
	//���û����ӵ���Դ�ļ�����ô�Ͳ�Ҫ:/
	QFile file("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size()) {
		//д��js�ļ�
		QStringList employeesIDList;
		getEmployeesID(employeesIDList);
		if (!createJSFile(employeesIDList)) {
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����js����ʧ��"));

		}
	}
}

TalkWindowShell::~TalkWindowShell()
{
	qDebug() << "__FUNCTION__" << endl;
	delete m_emotionWindow;
	m_emotionWindow=nullptr;
	
}

void TalkWindowShell::initControl()
{
	//������ʽ��
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("�������촰��"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();	//���ر��鴰��
	QList<int>leftWidgetSize;//�������ߴ�
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);//���������óߴ�

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));

}

void TalkWindowShell::initTcpSocket()
{
	m_tcpClientSocket = new QTcpSocket(this);
	//�ͻ���Ҫ��������˽�������(���ص�ַ�Ǳ���,�˿ں�ΪgtcpPort)
	m_tcpClientSocket->connectToHost("127.0.0.1",gtcpPort);

}
void TalkWindowShell::initUdpSocket()
{
	m_udpReceive = new QUdpSocket(this);
	for (quint16 port = gudpPort; port < gudpPort + 200; port++)
	{
		//�󶨶˿ں�
		if (m_udpReceive->bind(port, QUdpSocket::ShareAddress))
		{
			break;
		}
	}
	connect(m_udpReceive, &QUdpSocket::readyRead, this, &TalkWindowShell::processPendingData);
}
//��ȡԱ����qq��
void  TalkWindowShell::getEmployeesID(QStringList&employeesList)
{
	QSqlQueryModel queryModel;
	queryModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status=1"));
	
	//���м�¼��������Ա����������
	int employeesNum = queryModel.rowCount();
	QModelIndex index;
	for (int i = 0; i < employeesNum; i++) {
		index = queryModel.index(i, 0);//��+��
		employeesList << queryModel.data(index).toString();
	}
}

bool TalkWindowShell::createJSFile(QStringList & employeesList)
{
	//��ȡtxt�ļ�����
	QString strFileTxt = "Resources/MainWindow/MsgHtml/msgtmpl.txt";
	QFile fileRead(strFileTxt);
	QString strFile;
	if (fileRead.open(QIODevice::ReadOnly)) {
		strFile = fileRead.readAll();
		fileRead.close();
	}
	else {
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��ȡ msgtmpl.txt ʧ��!"));
		return false;
	}

	//�滻 ��external0,appendHtml0�����Լ�����Ϣʹ�ã�
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		//���¿�ֵ
		QString strSourceInitNull = "var external = null;";
		//���³�ʼ��
		QString strSourceInit = "external = channel.objects.external;";
		//����new QWebChannel
		//QString strSourceNew =
		//	"new QWebChannel(qt.webChannelTransport,\
		//	function(channel) {\
		//	external = channel.objects.external;\
		//}\
		//);";
		QString strSourceNew =
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
		}\
		); \
		";

		//����׷��recvHtml,�ű�����˫�����޷�ֱ�ӽ��и�ֵ�������ļ���ȡ
		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly))
		{
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		}
		else {
			QMessageBox::information(this,
				QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("��ȡ recvHtml.txt ʧ��!"));
			return false;
		}
		//�����滻��Ľű�
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;
		//�ж���Ա�����滻���ٴ�
		for (int i = 0; i < employeesList.length(); i++) {
			//�༭�滻���ֵ
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";
			strReplaceInitNull += "\n";

			//�༭�滻��ĳ�ʼֵ
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";
			strReplaceInit += "\n";
			//�༭�滻���newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";
			strReplaceNew += "\n";
			//�༭�滻���recvHtml
			QString strRecvHtml = strSourceRecvHtml;
			strRecvHtml.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strRecvHtml.replace("recvHtml", QString("recvHtml_%1").arg(employeesList.at(i)));
			strReplaceRecvHtml += strRecvHtml;
			strReplaceRecvHtml += "\n";
			strReplaceRecvHtml += "\n";
		}
		
		strFile.replace(strSourceInitNull, strReplaceInitNull);
		strFile.replace(strSourceInit, strReplaceInit);
		strFile.replace(strSourceNew, strReplaceNew);
		strFile.replace(strSourceRecvHtml, strReplaceRecvHtml);
		QTextStream stream(&fileWrite);
		stream << strFile;
		fileWrite.close();
		return true;
	}
	QMessageBox::information(this,
		QString::fromLocal8Bit("��ʾ"),
		QString::fromLocal8Bit("д msgtmpl.js ʧ��!"));
	return false;
}

void TalkWindowShell::handleReceiveMsg(int senderEmployeeID, int msgType, QString strMsg)
{
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setText(strMsg);

	if (msgType == 1) {//�ı���Ϣ
		msgTextEdit.document()->toHtml();
	}
	else if (msgType == 0) {//������Ϣ
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;//�������
		for (int i = 0; i < emotionNum; i++) {
			msgTextEdit.addEmotionUrl(strMsg.mid(i * 3, emotionWidth).toInt());//���ر���
		}
	}

	QString&& html = msgTextEdit.document()->toHtml();
	if (!html.contains(".png") && !html.contains("</span>")) {
		QString fontHtml;
		QString text = strMsg;  //�����û�������ı���ͼƬҲ��ת��Ϊ�ı���
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", text);
			file.close();
		}
		else {
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("δ�ҵ��ļ�msgFont.txt �����ڣ�"));
			return;
		}
		if (!html.contains(fontHtml)) {
			html.replace(text, fontHtml);
		}
	}
	//��ӵ���ҳ��ȥ
	TalkWindow*talkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	talkWindow->ui.msgWidget->appendMsg(html, QString::number(senderEmployeeID));
}

void TalkWindowShell::addTalkWindow(TalkWindow * talkWindow, TalkWindowItem * talkWindowItem,const QString&uid/*, GroupType grouptype*/)
{
	ui.rightStackedWidget->addWidget(talkWindow);
	//connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()), talkWindow, SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem*aItem = new QListWidgetItem(ui.listWidget);
	m_talkwindowItemMap.insert(aItem, talkWindow);

	aItem->setSelected(true);//����Ϊ��ѡ��
	
	//�ж�Ⱥ�Ļ��ǵ���
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture FROM tab_department WHERE departmentID=%1").arg(uid);
	sqlDepModel.setQuery(strQuery);
	int rows = sqlDepModel.rowCount();

	if (rows == 0) {//����
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID=%1").arg(uid);
		sqlDepModel.setQuery(strQuery);//����ģ��
	}

	QModelIndex index;
	index = sqlDepModel.index(0, 0);
	QImage img;
	img.load(sqlDepModel.data(index).toString());


	//����б��е�һ��������ͷ��
	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));
	
	//�����
	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);

	connect(talkWindowItem,&TalkWindowItem::signalCloseClicked,
		[talkWindowItem, talkWindow, aItem, this]() {
		m_talkwindowItemMap.remove(aItem);//��Ӧ���촰�ڽ����Ƴ�
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));//�Ƴ��б��еĶ�Ӧ������кţ�
		delete talkWindowItem;
		ui.rightStackedWidget->removeWidget(talkWindow);//��ҳ�������ҲҪ����Ӧ���촰�����Ƴ�
		if (ui.rightStackedWidget->count() < 1) {  //����������û��������Ļ�
			close();  //��������ҳ��ȫ���ر�
		}
	});
}

void TalkWindowShell::setCurrentWidget(QWidget * widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}
const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::getTalkWindowItemMap() const
{
	return m_talkwindowItemMap;
}

//�ı����ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��QQ��+����ϢԱ��QQ��(Ⱥqq��)+��Ϣ����+���ݳ���(5)+���ݣ�hello��
//�������ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��qq��+����ϢԱ��qq�ţ�Ⱥ�ţ�+��Ϣ����+�������+images+��������
//msgType 0������Ϣ  1�ı���Ϣ 2�ļ���Ϣ
void TalkWindowShell::updateSendTcpMsg(QString & strData, int & msgType, QString filename)
{
	TalkWindow*curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	QString talkId = curTalkWindow->getTalkId();

	QString strGroupFlag;
	QString strSend;
	if (talkId.length() == 4) {//Ⱥqq����
		strGroupFlag = "1";
	}
	else {
		strGroupFlag = "0";
	}
	int nstrDataLength=strData.length();
	int dataLength = QString::number(nstrDataLength).length();
	//const int sourceDataLength = dataLength;
	QString strdataLength;//���ڱ�ʾ���ݵĳ���

	if (msgType == 1) {//�����ı���Ϣ
		//�ı���Ϣ�ĳ���Լ��Ϊ5λ
		if (dataLength == 1) {
			strdataLength = "0000" + QString::number(nstrDataLength);
		}
		else if (dataLength == 2)
		{
			strdataLength = "000" + QString::number(nstrDataLength);
		}
		else if (dataLength == 3)
		{
			strdataLength = "00" + QString::number(nstrDataLength);
		}
		else if (dataLength == 4)
		{
			strdataLength = "0" + QString::number(nstrDataLength);
		}
		else if (dataLength == 5)
		{
			strdataLength = QString::number(nstrDataLength);
		}
		else {
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����������ݳ��ȣ�"));
		}

		//�ı����ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��QQ��+����ϢԱ��QQ��(Ⱥqq��)+��Ϣ����+���ݳ���(5)+���ݣ�hello��
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1"+strdataLength+strData;
	}
	else if (msgType == 0) {//������Ϣ
		//�������ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��qq��+����ϢԱ��qq�ţ�Ⱥ�ţ�+��Ϣ����+�������+images+��������
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0"+ strData;
	}
	else if (msgType == 2) {//�ļ���Ϣ
		//�ļ����ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��qq��+����ϢԱ��qq��+��Ϣ���ͣ�2��+�ļ�����+"bytes"+�ļ�����+"data_begin"+�ļ�����
		QString strLength = QString::number(strData.toUtf8().length());//��ȡ�ֽ�����ĳ���
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2" + strLength+"bytes" + filename + "data_begin" + strData;

	}

	QByteArray dataBt;
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8();
	m_tcpClientSocket->write(dataBt);
}
//���鱻���
void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	TalkWindow*curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());//���ض�ҳ�沿���ĵ�ǰ����
	if (curTalkWindow)
	{
		curTalkWindow->addEmotionImage(emotionNum);
	}
}
/***********************************************************************************************
	���ݰ���ʽ��
	�ı����ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��QQ��+����ϢԱ��QQ�ţ�ȺQQ�ţ�+��Ϣ���ͣ�1��+���ݳ���+����
	�������ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��QQ��+����ϢԱ��QQ�ţ�ȺQQ�ţ�+��Ϣ���ͣ�0��+�������+images+��������
	�ļ����ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��QQ��+����ϢԱ��QQ��+��Ϣ���ͣ�2��+�ļ��ֽ���+bytes+�ļ���+data_begin+�ļ�����
	1100012000211���Խ���.txtdata_benginhello���
	Ⱥ�ı�־ռһλ��0��ʾȺ�ģ�1��ʾ����
	��Ϣ����ռһλ��0��ʾ������Ϣ��1��ʾ�ı���Ϣ��2��ʾ�ļ���Ϣ
	QQ��ռλ��ȺQQ��ռ4Ϊ���ݳ���ռ5λ����������ռ3λ
	��ע�⣺��Ⱥ�ı�־λ1ʱ�������ݰ�û������ϢԱ��QQ�ţ���������ϢȺQQ�ţ�
											
	Ⱥ���ı���Ϣ�磺1100012001100005Hello ��ʾQQ��10001��Ⱥ2001�����ı���Ϣ������Ϊ5������ΪHello
	����ͼƬ��Ϣ�磺0100011000201images060		  ��ʾQQ10001��QQ10002����һ������60.png
	Ⱥ���ļ���Ϣ�磺1100052000210bytestest.txtdata_beginhelloworld 
						��ʾQQ10005��Ⱥ2000�����ļ���Ϣ�ļ����ݳ���Ϊ10���ļ���test.txt���ļ�����Ϊhelloworld

	Ⱥ���ļ���Ϣ������1 10001 2001 1 00005 Hello						
	����ͼƬ��Ϣ������0 10001 10002 0 1 images 060
	Ⱥ���ļ���Ϣ������1 10005 20002 10 bytes test.txt data_begin helloworld
************************************************************************************************/
void TalkWindowShell::processPendingData()
{
	//�˿����Ƿ���δ���������
	while (m_udpReceive->hasPendingDatagrams()) 
	{
		const static int groupFlagWidth = 1;//Ⱥ�ı�־ռλ
		const static int groupWidth = 4;	//ȺQQ�ſ��
		const static int employeeWidth = 5;	//Ա��QQ�ſ��
		const static int msgTypeWidth = 1;	//��Ϣ���Ϳ��
		const static int msgLengthWidth = 5;//�ı���Ϣ���ȵĿ��
		const static int pictureWidth = 3;	//����ͼƬ�Ŀ��

		QByteArray btData;
		btData.resize(m_udpReceive->pendingDatagramSize());//���ô�СΪδ�������ݵĴ�С
		m_udpReceive->readDatagram(btData.data(), btData.size());//��ȡudp����
	
		
		
		QString strData = btData.data();
		QString strWindowID;//���촰��ID,Ⱥ������Ⱥ�ţ���������Ա��QQ��
		QString strSendEmployeeID, strReceiveEmployeeID;//���ͼ����ն˵�QQ��
		QString strMsg;//����

		int msgLen; //���ݳ���
		int msgType;//��������

		strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);
		//�Լ�������Ϣ��������
		if (strSendEmployeeID == gLoginEmployeeID) {
			return;
		}

		//Ⱥ�Ĵ���
		if (btData[0] == '1') {
			//ȺQQ��
			strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);//Ⱥ��QQ��
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];
			if (cMsgType == '1')//�ı���Ϣ
			{
				msgType = 1;
				msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();
				strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgType+msgLengthWidth,msgLen);
			}
			else if (cMsgType == '0') //������Ϣ
			{
				msgType = 0;
				int posImages = strData.indexOf("images");
				strMsg = strData.right(strData.length() - posImages - QString("images").length());//��ȡ���ұ�
			}
			else if (cMsgType == '2')//�ļ���Ϣ
			{
				msgType = 2;
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int posData_begin = strData.indexOf("data_begin");

				//�ļ�����
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin -posBytes- bytesWidth);
				gfileName = fileName;
				//�ļ�����
				int dataLengthWidth;
				int posData = posData_begin + QString("data_begin").length();
				strMsg = strData.mid(posData);
				gfileData = strMsg;
				//����employeeID��ȡ����������
				QString sender;
				int employeeID = strSendEmployeeID.toInt();
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1")
					.arg(employeeID));
				querySenderName.exec();
				if (querySenderName.first()) {
					sender = querySenderName.value(0).toString();
				}

				//�����ļ��ĺ�������......................
				ReceiveFile*recvFile = new ReceiveFile(this);
				connect(recvFile, &ReceiveFile::refuseFile, [this]() {
					return;
				});
				QString msgLabel = QString::fromLocal8Bit("�յ�����") + sender + QString::fromLocal8Bit("�������ļ�,�Ƿ����?");
				recvFile->setMsg(msgLabel);
				recvFile->show(); 
			}
		}
		else {//����
			

			strReceiveEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);
			strWindowID = strSendEmployeeID;
			
			//���Ƿ����ҵ���Ϣ�Ҳ�������
			if (strReceiveEmployeeID != gLoginEmployeeID) 
			{
				return;
			}

			//��������
			//��ȡ��������
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
			if (cMsgType == '1') {//�ı���Ϣ
				msgType = 1;
				//�ı����ݳ���
				msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth, msgLengthWidth).toInt();

				//�ı���Ϣ
				strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (cMsgType == '0') {//��������
				msgType = 0;
				int posImage = strData.indexOf("images");
				int imagesWidth = QString("images").length();
				strMsg = strData.mid(posImage + imagesWidth);//��posImage + imagesWidthȫ����ȡ����
			}
			else if (cMsgType == '2') {//�ļ�����
				msgType = 2;
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int data_beginWidth = QString("data_begin").length();
				int posData_begin = strData.indexOf("data_begin");

				//��ȡ�ļ�����
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gfileName = fileName;

				//�ļ�����
				strMsg = strData.mid(posData_begin + data_beginWidth);
				gfileData = strMsg;

				//����employeeID��ȡ����������
				QString sender;
				int employeeID = strSendEmployeeID.toInt();
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1")
					.arg(employeeID));
				querySenderName.exec();
				if (querySenderName.first()) {
					sender = querySenderName.value(0).toString();
				}

				//�����ļ��ĺ�������......................
				ReceiveFile*recvFile = new ReceiveFile(this);
				connect(recvFile, &ReceiveFile::refuseFile, [this]() {
					return;
				});
				QString msgLabel = QString::fromLocal8Bit("�յ�����") + sender + QString::fromLocal8Bit("�������ļ�,�Ƿ����?");
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}

		}
		
		//�����촰������Ϊ��Ĵ���
		QWidget*widget = WindowManager::getInstance()->findWindowName(strWindowID);
		if (widget) {//���촰���Ѵ�
			this->setCurrentWidget(widget);
			//ͬ������������촰��
			QListWidgetItem *item=m_talkwindowItemMap.key(widget);
			item->setSelected(true);
		}
		else {		 //���촰��δ��
			return;
		}
		//������ӵ����촰���в���(׷����ҳ��Ϣ)(�ļ���Ϣ��������)
		if (msgType != 2)
		{
			int senderEmployeeID = strSendEmployeeID.toInt();
			handleReceiveMsg(senderEmployeeID, msgType, strMsg);
		}
		
	}
}

void TalkWindowShell::onEmotionBtnClicked(bool) 
{
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//����ǰ�ؼ������λ��ת��Ϊ��Ļ�ľ���λ��

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 220);
	m_emotionWindow->move(emotionPoint);

}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem*item)
{
	QWidget*talkwindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkwindowWidget);//���ò���
}