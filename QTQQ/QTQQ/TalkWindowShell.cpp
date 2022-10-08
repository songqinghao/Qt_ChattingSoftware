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
QString gfileName;	//文件名称
QString gfileData;	//文件数据

extern QString gLoginEmployeeID;
TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//当窗体关闭时资源进行时回收
	initControl();
	//初始化tcp
	initTcpSocket();
	initUdpSocket();
	
	//如果没有添加到资源文件中那么就不要:/
	QFile file("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size()) {
		//写入js文件
		QStringList employeesIDList;
		getEmployeesID(employeesIDList);
		if (!createJSFile(employeesIDList)) {
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("更新js数据失败"));

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
	//加载样式表
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("奇奇聊天窗口"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();	//隐藏表情窗口
	QList<int>leftWidgetSize;//分裂器尺寸
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);//分裂器设置尺寸

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));

}

void TalkWindowShell::initTcpSocket()
{
	m_tcpClientSocket = new QTcpSocket(this);
	//客户端要与服务器端进行连接(环回地址是本机,端口号为gtcpPort)
	m_tcpClientSocket->connectToHost("127.0.0.1",gtcpPort);

}
void TalkWindowShell::initUdpSocket()
{
	m_udpReceive = new QUdpSocket(this);
	for (quint16 port = gudpPort; port < gudpPort + 200; port++)
	{
		//绑定端口号
		if (m_udpReceive->bind(port, QUdpSocket::ShareAddress))
		{
			break;
		}
	}
	connect(m_udpReceive, &QUdpSocket::readyRead, this, &TalkWindowShell::processPendingData);
}
//获取员工的qq号
void  TalkWindowShell::getEmployeesID(QStringList&employeesList)
{
	QSqlQueryModel queryModel;
	queryModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status=1"));
	
	//所有记录的数量（员工的总数）
	int employeesNum = queryModel.rowCount();
	QModelIndex index;
	for (int i = 0; i < employeesNum; i++) {
		index = queryModel.index(i, 0);//行+列
		employeesList << queryModel.data(index).toString();
	}
}

bool TalkWindowShell::createJSFile(QStringList & employeesList)
{
	//读取txt文件数据
	QString strFileTxt = "Resources/MainWindow/MsgHtml/msgtmpl.txt";
	QFile fileRead(strFileTxt);
	QString strFile;
	if (fileRead.open(QIODevice::ReadOnly)) {
		strFile = fileRead.readAll();
		fileRead.close();
	}
	else {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("读取 msgtmpl.txt 失败!"));
		return false;
	}

	//替换 （external0,appendHtml0用于自己发信息使用）
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		//更新空值
		QString strSourceInitNull = "var external = null;";
		//更新初始化
		QString strSourceInit = "external = channel.objects.external;";
		//更新new QWebChannel
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

		//更新追加recvHtml,脚本中有双引号无法直接进行赋值，采用文件读取
		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly))
		{
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		}
		else {
			QMessageBox::information(this,
				QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("读取 recvHtml.txt 失败!"));
			return false;
		}
		//保存替换后的脚本
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;
		//有多少员工就替换多少次
		for (int i = 0; i < employeesList.length(); i++) {
			//编辑替换后的值
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";
			strReplaceInitNull += "\n";

			//编辑替换后的初始值
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";
			strReplaceInit += "\n";
			//编辑替换后的newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";
			strReplaceNew += "\n";
			//编辑替换后的recvHtml
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
		QString::fromLocal8Bit("提示"),
		QString::fromLocal8Bit("写 msgtmpl.js 失败!"));
	return false;
}

void TalkWindowShell::handleReceiveMsg(int senderEmployeeID, int msgType, QString strMsg)
{
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setText(strMsg);

	if (msgType == 1) {//文本信息
		msgTextEdit.document()->toHtml();
	}
	else if (msgType == 0) {//表情信息
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;//表情个数
		for (int i = 0; i < emotionNum; i++) {
			msgTextEdit.addEmotionUrl(strMsg.mid(i * 3, emotionWidth).toInt());//加载表情
		}
	}

	QString&& html = msgTextEdit.document()->toHtml();
	if (!html.contains(".png") && !html.contains("</span>")) {
		QString fontHtml;
		QString text = strMsg;  //保存用户输入的文本（图片也会转化为文本）
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", text);
			file.close();
		}
		else {
			QMessageBox::information(this, QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("未找到文件msgFont.txt 不存在！"));
			return;
		}
		if (!html.contains(fontHtml)) {
			html.replace(text, fontHtml);
		}
	}
	//添加到网页中去
	TalkWindow*talkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	talkWindow->ui.msgWidget->appendMsg(html, QString::number(senderEmployeeID));
}

void TalkWindowShell::addTalkWindow(TalkWindow * talkWindow, TalkWindowItem * talkWindowItem,const QString&uid/*, GroupType grouptype*/)
{
	ui.rightStackedWidget->addWidget(talkWindow);
	//connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()), talkWindow, SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem*aItem = new QListWidgetItem(ui.listWidget);
	m_talkwindowItemMap.insert(aItem, talkWindow);

	aItem->setSelected(true);//设置为已选中
	
	//判断群聊还是单聊
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture FROM tab_department WHERE departmentID=%1").arg(uid);
	sqlDepModel.setQuery(strQuery);
	int rows = sqlDepModel.rowCount();

	if (rows == 0) {//单聊
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID=%1").arg(uid);
		sqlDepModel.setQuery(strQuery);//设置模型
	}

	QModelIndex index;
	index = sqlDepModel.index(0, 0);
	QImage img;
	img.load(sqlDepModel.data(index).toString());


	//左边列表中的一列列设置头像
	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));
	
	//添加项
	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);

	connect(talkWindowItem,&TalkWindowItem::signalCloseClicked,
		[talkWindowItem, talkWindow, aItem, this]() {
		m_talkwindowItemMap.remove(aItem);//对应聊天窗口进行移除
		talkWindow->close();
		ui.listWidget->takeItem(ui.listWidget->row(aItem));//移除列表中的对应项（传入行号）
		delete talkWindowItem;
		ui.rightStackedWidget->removeWidget(talkWindow);//多页面的那里也要将对应聊天窗口项移除
		if (ui.rightStackedWidget->count() < 1) {  //假如整个都没有聊天项的话
			close();  //整个聊天页面全部关闭
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

//文本数据包格式：群聊标志+发信息员工QQ号+收消息员工QQ号(群qq号)+消息类型+数据长度(5)+数据（hello）
//表情数据包格式：群聊标志+发信息员工qq号+收消息员工qq号（群号）+消息类型+表情个数+images+表情数据
//msgType 0表情消息  1文本消息 2文件消息
void TalkWindowShell::updateSendTcpMsg(QString & strData, int & msgType, QString filename)
{
	TalkWindow*curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	QString talkId = curTalkWindow->getTalkId();

	QString strGroupFlag;
	QString strSend;
	if (talkId.length() == 4) {//群qq长度
		strGroupFlag = "1";
	}
	else {
		strGroupFlag = "0";
	}
	int nstrDataLength=strData.length();
	int dataLength = QString::number(nstrDataLength).length();
	//const int sourceDataLength = dataLength;
	QString strdataLength;//用于表示数据的长度

	if (msgType == 1) {//发送文本消息
		//文本消息的长度约定为5位
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
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("不合理的数据长度！"));
		}

		//文本数据包格式：群聊标志+发信息员工QQ号+收消息员工QQ号(群qq号)+消息类型+数据长度(5)+数据（hello）
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1"+strdataLength+strData;
	}
	else if (msgType == 0) {//表情消息
		//表情数据包格式：群聊标志+发信息员工qq号+收消息员工qq号（群号）+消息类型+表情个数+images+表情数据
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0"+ strData;
	}
	else if (msgType == 2) {//文件消息
		//文件数据包格式：群聊标志+发信息员工qq号+收消息员工qq号+消息类型（2）+文件长度+"bytes"+文件名称+"data_begin"+文件内容
		QString strLength = QString::number(strData.toUtf8().length());//获取字节数组的长度
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2" + strLength+"bytes" + filename + "data_begin" + strData;

	}

	QByteArray dataBt;
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8();
	m_tcpClientSocket->write(dataBt);
}
//表情被点击
void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	TalkWindow*curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());//返回多页面部件的当前部件
	if (curTalkWindow)
	{
		curTalkWindow->addEmotionImage(emotionNum);
	}
}
/***********************************************************************************************
	数据包格式：
	文本数据包格式：群聊标志+发信息员工QQ号+收消息员工QQ号（群QQ号）+消息类型（1）+数据长度+数据
	表情数据包格式：群聊标志+发信息员工QQ号+收消息员工QQ号（群QQ号）+消息类型（0）+表情个数+images+表情名称
	文件数据包格式：群聊标志+发信息员工QQ号+收消息员工QQ号+消息类型（2）+文件字节数+bytes+文件名+data_begin+文件数据
	1100012000211测试接收.txtdata_benginhello你好
	群聊标志占一位，0表示群聊，1表示单聊
	消息类型占一位，0表示表情信息，1表示文本信息，2表示文件信息
	QQ号占位，群QQ号占4为数据长度占5位，表情名称占3位
	（注意：当群聊标志位1时，择数据包没有收消息员工QQ号，而是收消息群QQ号）
											
	群聊文本信息如：1100012001100005Hello 表示QQ号10001向群2001发送文本信息，长度为5，数据为Hello
	单聊图片信息如：0100011000201images060		  表示QQ10001向QQ10002发送一个表情60.png
	群聊文件信息如：1100052000210bytestest.txtdata_beginhelloworld 
						表示QQ10005向群2000发送文件信息文件内容长度为10，文件是test.txt，文件数据为helloworld

	群聊文件信息解析：1 10001 2001 1 00005 Hello						
	单聊图片信息解析：0 10001 10002 0 1 images 060
	群聊文件信息解析：1 10005 20002 10 bytes test.txt data_begin helloworld
************************************************************************************************/
void TalkWindowShell::processPendingData()
{
	//端口中是否有未处理的数据
	while (m_udpReceive->hasPendingDatagrams()) 
	{
		const static int groupFlagWidth = 1;//群聊标志占位
		const static int groupWidth = 4;	//群QQ号宽度
		const static int employeeWidth = 5;	//员工QQ号宽度
		const static int msgTypeWidth = 1;	//消息类型宽度
		const static int msgLengthWidth = 5;//文本信息长度的宽度
		const static int pictureWidth = 3;	//表情图片的宽度

		QByteArray btData;
		btData.resize(m_udpReceive->pendingDatagramSize());//设置大小为未处理数据的大小
		m_udpReceive->readDatagram(btData.data(), btData.size());//读取udp数据
	
		
		
		QString strData = btData.data();
		QString strWindowID;//聊天窗口ID,群聊则是群号，单聊则是员工QQ号
		QString strSendEmployeeID, strReceiveEmployeeID;//发送及接收端的QQ号
		QString strMsg;//数据

		int msgLen; //数据长度
		int msgType;//数据类型

		strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);
		//自己发的信息不做处理
		if (strSendEmployeeID == gLoginEmployeeID) {
			return;
		}

		//群聊窗口
		if (btData[0] == '1') {
			//群QQ号
			strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);//群聊QQ号
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + groupWidth];
			if (cMsgType == '1')//文本信息
			{
				msgType = 1;
				msgLen = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgTypeWidth, msgLengthWidth).toInt();
				strMsg = strData.mid(groupFlagWidth + employeeWidth + groupWidth + msgType+msgLengthWidth,msgLen);
			}
			else if (cMsgType == '0') //表情信息
			{
				msgType = 0;
				int posImages = strData.indexOf("images");
				strMsg = strData.right(strData.length() - posImages - QString("images").length());//截取到右边
			}
			else if (cMsgType == '2')//文件信息
			{
				msgType = 2;
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int posData_begin = strData.indexOf("data_begin");

				//文件名称
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin -posBytes- bytesWidth);
				gfileName = fileName;
				//文件内容
				int dataLengthWidth;
				int posData = posData_begin + QString("data_begin").length();
				strMsg = strData.mid(posData);
				gfileData = strMsg;
				//根据employeeID获取发送者姓名
				QString sender;
				int employeeID = strSendEmployeeID.toInt();
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1")
					.arg(employeeID));
				querySenderName.exec();
				if (querySenderName.first()) {
					sender = querySenderName.value(0).toString();
				}

				//接受文件的后续操作......................
				ReceiveFile*recvFile = new ReceiveFile(this);
				connect(recvFile, &ReceiveFile::refuseFile, [this]() {
					return;
				});
				QString msgLabel = QString::fromLocal8Bit("收到来自") + sender + QString::fromLocal8Bit("发来的文件,是否接收?");
				recvFile->setMsg(msgLabel);
				recvFile->show(); 
			}
		}
		else {//单聊
			

			strReceiveEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);
			strWindowID = strSendEmployeeID;
			
			//不是发给我的信息我不做处理
			if (strReceiveEmployeeID != gLoginEmployeeID) 
			{
				return;
			}

			//解析数据
			//获取数据类型
			QChar cMsgType = btData[groupFlagWidth + employeeWidth + employeeWidth];
			if (cMsgType == '1') {//文本信息
				msgType = 1;
				//文本数据长度
				msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth, msgLengthWidth).toInt();

				//文本信息
				strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (cMsgType == '0') {//表情数据
				msgType = 0;
				int posImage = strData.indexOf("images");
				int imagesWidth = QString("images").length();
				strMsg = strData.mid(posImage + imagesWidth);//从posImage + imagesWidth全部截取出来
			}
			else if (cMsgType == '2') {//文件数据
				msgType = 2;
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int data_beginWidth = QString("data_begin").length();
				int posData_begin = strData.indexOf("data_begin");

				//获取文件名称
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gfileName = fileName;

				//文件内容
				strMsg = strData.mid(posData_begin + data_beginWidth);
				gfileData = strMsg;

				//根据employeeID获取发送者姓名
				QString sender;
				int employeeID = strSendEmployeeID.toInt();
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1")
					.arg(employeeID));
				querySenderName.exec();
				if (querySenderName.first()) {
					sender = querySenderName.value(0).toString();
				}

				//接受文件的后续操作......................
				ReceiveFile*recvFile = new ReceiveFile(this);
				connect(recvFile, &ReceiveFile::refuseFile, [this]() {
					return;
				});
				QString msgLabel = QString::fromLocal8Bit("收到来自") + sender + QString::fromLocal8Bit("发来的文件,是否接收?");
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}

		}
		
		//将聊天窗口设置为活动的窗口
		QWidget*widget = WindowManager::getInstance()->findWindowName(strWindowID);
		if (widget) {//聊天窗口已打开
			this->setCurrentWidget(widget);
			//同步激活左侧聊天窗口
			QListWidgetItem *item=m_talkwindowItemMap.key(widget);
			item->setSelected(true);
		}
		else {		 //聊天窗口未打开
			return;
		}
		//数据添加到聊天窗口中操作(追加网页信息)(文件信息另作处理)
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
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//将当前控件的相对位置转换为屏幕的绝对位置

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 220);
	m_emotionWindow->move(emotionPoint);

}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem*item)
{
	QWidget*talkwindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkwindowWidget);//设置部件
}