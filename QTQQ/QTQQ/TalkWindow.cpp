#include "TalkWindow.h"
#include"RootContatItem.h"
#include"ContactItem.h"
#include"CommonUtils.h"
#include"WindowManager.h"
#include<QToolTip>
#include<QMessageBox>
#include<QSqlQueryModel>
#include<QSqlQuery>
#include"SendFile.h"
extern QString gLoginEmployeeID;
TalkWindow::TalkWindow(QWidget *parent, const QString&uid/*, GroupType groupType*/)
	: QWidget(parent),m_talkId(uid)//,m_groupType(groupType)
{
	ui.setupUi(this);
	WindowManager::getInstance()->addWindowName(m_talkId, this);
	setAttribute(Qt::WA_DeleteOnClose);

	initGroupTalkStatus();
	initControl();
}

TalkWindow::~TalkWindow()
{
	WindowManager::getInstance()->deleteWindowName(m_talkId);
}

void TalkWindow::addEmotionImage(int emotionNum)
{
	ui.textEdit->setFocus();
	//添加表情操作
	ui.textEdit->addEmotionUrl(emotionNum);

}

void TalkWindow::setWindowName(const QString & name)
{
	ui.nameLabel->setText(name);
}

QString TalkWindow::getTalkId()
{
	return m_talkId;
}

void TalkWindow::initControl()
{
	QList<int>rightWidgetSize;
	rightWidgetSize << 600 << 138;
	ui.bodySplitter->setSizes(rightWidgetSize);
	ui.textEdit->setFontPointSize(10);
	ui.textEdit->setFocus();
	connect(ui.sysmin, SIGNAL(clicked(bool)), parent(), SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));

	connect(ui.faceBtn, SIGNAL(clicked(bool)), parent(), SLOT(onEmotionBtnClicked(bool)));
	connect(ui.sendBtn, SIGNAL(clicked(bool)), this, SLOT(onSendBtnClicked(bool)));
	connect(ui.fileopenBtn, SIGNAL(clicked(bool)), this, SLOT(onFileOpenBtnClicked(bool)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));
	
	if (m_isGroupTalk) {
		//初始化群聊
		initTalkWindow();
	}
	else {
		initPtoPtalk();
	}
	/*
	switch (m_groupType) 
	{
	case COMPANY:
	{
		initCompanyTalk();
		break;
	}
	case PERSONELGROUP:
	{
		initPersonelTalk();
		break;
	}
	case MARKETGROUP:
	{
		initMarketTalk();
		break;
	}
	case DEVELOPMENTGROUP:
	{
		initDevelopTalk();
		break;
	}
	case PTOP:
	{
		initPtoPtalk();
		break;
	}
	default:
		break;
	}
	*/
}
void TalkWindow::initGroupTalkStatus()
{
	QSqlQueryModel sqlDepModel;
	QString strSql = QString("SELECT * FROM tab_department WHERE departmentID=%1")
					.arg(m_talkId);
	sqlDepModel.setQuery(strSql);

	int rows = sqlDepModel.rowCount();
	if (rows == 0) {//单聊
		m_isGroupTalk = false;
	}
	else {
		m_isGroupTalk = true;
	}
}
int TalkWindow::getCompDepID()
{
	QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'")
					.arg(QString::fromLocal8Bit("公司群")));

	queryDepID.exec();
	queryDepID.first();
	return queryDepID.value(0).toInt();
}

void TalkWindow::initTalkWindow()
{
	//构建一个根项
	QTreeWidgetItem*pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//设置孩子指示器的策略(无论是否有孩子他的治时期都能展开)

	//设置data，用于区别根项以及子项
	pRootItem->setData(0, Qt::UserRole, 0);//第0列，角色为UserRole,值为0
	RootContatItem*pItemName = new RootContatItem(false, ui.treeWidget);//箭头为false,父类为treeWidget
	ui.treeWidget->setFixedHeight(646);//shell的高度-shell头高（talkwindow titleWidget高度）
	
	//当前聊天群组名或人名
	QString strGroupName;
	QSqlQuery queryGroupName(QString("SELECT department_name FROM tab_department WHERE departmentID=%1").arg(m_talkId));
	queryGroupName.exec();
	if (queryGroupName.first()) {//有记录
		strGroupName = queryGroupName.value(0).toString();
	}
	
	
	QSqlQueryModel queryEmployeeModel;
	
	//如果是公司群
	if (getCompDepID() == m_talkId.toInt()) {
		queryEmployeeModel.setQuery("SELECT employeeID FROM tab_employees WHERE status=1");
	}
	else {
		queryEmployeeModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status=1 AND departmentID=%1").arg(m_talkId));

	}
	int nEmployeeNum = queryEmployeeModel.rowCount();//群人数
	
	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3")
		.arg(strGroupName)
		.arg(0).
		arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem,0,pItemName);//设置部件pRootItem在第0列,部件为pItemName

	//展开
	pRootItem->setExpanded(true);
	for (int i = 0; i < nEmployeeNum; i++) 
	{
		QModelIndex modelIndex = queryEmployeeModel.index(i,0);//行号为i列好为0----》获取模式索引
		int employeeID=queryEmployeeModel.data(modelIndex).toInt();//获取员工的员工号
		

		////过滤掉自己
		//if (employeeID == gLoginEmployeeID.toInt())
		//{
		//	continue;
		//}
		//添加子节点
		addPeopInfo(pRootItem, employeeID);
	}

}
//void TalkWindow::initCompanyTalk()
//{
//	//构建一个根项
//	QTreeWidgetItem*pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//设置孩子指示器的策略(无论是否有孩子他的治时期都能展开)
//
//	//设置data，用于区别根项以及子项
//	pRootItem->setData(0, Qt::UserRole, 0);//第0列，角色为UserRole,值为0
//	RootContatItem*pItemName = new RootContatItem(false, ui.treeWidget);//箭头为false,父类为treeWidget
//	ui.treeWidget->setFixedHeight(646);//shell的高度-shell头高（talkwindow titleWidget高度）
//	int nEmployeeNum = 50;//在线人数
//	QString qsGroupName = QString::fromLocal8Bit("公司群 %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	//插入分组节点
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem,0,pItemName);//设置部件pRootItem在第0列,部件为pItemName
//
//	//展开
//	pRootItem->setExpanded(true);
//	for (int i = 0; i < nEmployeeNum; i++) 
//	{
//		addPeopInfo(pRootItem);
//	}
//}
//void TalkWindow::initPersonelTalk()
//{
//	//构建一个根项
//	QTreeWidgetItem*pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//设置孩子指示器的策略(无论是否有孩子他的治时期都能展开)
//
//	//设置data，用于区别根项以及子项
//	pRootItem->setData(0, Qt::UserRole, 0);//第0列，角色为UserRole,值为0
//	RootContatItem*pItemName = new RootContatItem(false, ui.treeWidget);//箭头为false,父类为treeWidget
//	ui.treeWidget->setFixedHeight(646);//shell的高度-shell头高（talkwindow titleWidget高度）
//	int nEmployeeNum = 5;//在线人数
//	QString qsGroupName = QString::fromLocal8Bit("人事部 %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	//插入分组节点
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);//设置部件pRootItem在第0列,部件为pItemName
//
//	//展开
//	pRootItem->setExpanded(true);
//	for (int i = 0; i < nEmployeeNum; i++)
//	{
//		addPeopInfo(pRootItem);
//	}
//}
//void TalkWindow::initMarketTalk()
//{
//	//构建一个根项
//	QTreeWidgetItem*pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//设置孩子指示器的策略(无论是否有孩子他的治时期都能展开)
//
//	//设置data，用于区别根项以及子项
//	pRootItem->setData(0, Qt::UserRole, 0);//第0列，角色为UserRole,值为0
//	RootContatItem*pItemName = new RootContatItem(false, ui.treeWidget);//箭头为false,父类为treeWidget
//	ui.treeWidget->setFixedHeight(646);//shell的高度-shell头高（talkwindow titleWidget高度）
//	int nEmployeeNum = 8;//在线人数
//	QString qsGroupName = QString::fromLocal8Bit("市场部 %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	//插入分组节点
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);//设置部件pRootItem在第0列,部件为pItemName
//
//	//展开
//	pRootItem->setExpanded(true);
//	for (int i = 0; i < nEmployeeNum; i++)
//	{
//		addPeopInfo(pRootItem);
//	}
//}
//void TalkWindow::initDevelopTalk()
//{
//	//构建一个根项
//	QTreeWidgetItem*pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//设置孩子指示器的策略(无论是否有孩子他的治时期都能展开)
//
//	//设置data，用于区别根项以及子项
//	pRootItem->setData(0, Qt::UserRole, 0);//第0列，角色为UserRole,值为0
//	RootContatItem*pItemName = new RootContatItem(false, ui.treeWidget);//箭头为false,父类为treeWidget
//	ui.treeWidget->setFixedHeight(646);//shell的高度-shell头高（talkwindow titleWidget高度）
//	int nEmployeeNum = 32;//在线人数
//	QString qsGroupName = QString::fromLocal8Bit("研发部 %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	//插入分组节点
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);//设置部件pRootItem在第0列,部件为pItemName
//
//	//展开
//	pRootItem->setExpanded(true);
//	for (int i = 0; i < nEmployeeNum; i++)
//	{
//		addPeopInfo(pRootItem);
//	}
//}
void TalkWindow::initPtoPtalk()
{
	QPixmap pixSkin;
	pixSkin.load(":/Resources/MainWindow/skin.png");
	ui.widget->setFixedSize(pixSkin.size());
	QLabel*skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());
}
void TalkWindow::addPeopInfo(QTreeWidgetItem * pRootGroupItem, int employeeID)
{
	QTreeWidgetItem*pChild = new QTreeWidgetItem();
	
	
	//添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1,employeeID);
	ContactItem*pContactItem = new ContactItem(ui.treeWidget);
	
	

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	
	//获取名，签名，头像
	QString strName, strSign, strPicturePath;
	QSqlQueryModel queryInfoModel;
	queryInfoModel.setQuery(QString("SELECT employee_name,employee_sign,picture FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	
	//创建模型索引
	QModelIndex nameIndex, signIndex, pictureIndex;
	nameIndex = queryInfoModel.index(0, 0);//行列
	signIndex = queryInfoModel.index(0, 1);
	pictureIndex = queryInfoModel.index(0, 2);

	strName = queryInfoModel.data(nameIndex).toString();
	strSign = queryInfoModel.data(signIndex).toString();
	strPicturePath = queryInfoModel.data(pictureIndex).toString();

	//设置头像
	QImage imageHead;
	imageHead.load(strPicturePath);
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(QPixmap::fromImage(imageHead), pix1, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(strName);
	pContactItem->setSignName(strSign);

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);//类似将pChild和pContactItem绑定起来

	//窗体(联系人)和窗体（联系人）名称一一对应的映射
	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);

}
void TalkWindow::onFileOpenBtnClicked(bool) 
{
	SendFile*sendFile = new SendFile(this);
	sendFile->show();

}
void TalkWindow::onItemDoubleClicked(QTreeWidgetItem*item, int column)
{
	bool bIsChild=item->data(0, Qt::UserRole).toBool();
	if (bIsChild)
	{
		QString talkId = item->data(0, Qt::UserRole + 1).toString();
		if (talkId == gLoginEmployeeID)
		{
			return;
		}
		//获取人名的名字
		QString strPeopleName = m_groupPeopleMap.value(item);

		//添加窗口
		WindowManager::getInstance()->addNewTalkWindow(item->data(0,Qt::UserRole+1).toString()/*, PTOP, strPeopleName*/);
	}
}

//发送消息按钮触发的槽方法
void TalkWindow::onSendBtnClicked(bool) 
{
	if (ui.textEdit->toPlainText().isEmpty()) {
		//坐标(相对于聊天窗体)转换为全局(相对于桌面左上角)坐标+文本消息+父窗口+大小（120宽100高）+显示时间为2000毫秒
		QToolTip::showText(this->mapToGlobal(QPoint(630,660)),QString::fromLocal8Bit("发送消息不可以为空！"),this,QRect(0,0,120,100),2000);
		return;
	}

	QString html=ui.textEdit->document()->toHtml();//转化为html
	
	//文本html如果没有字体则添加字体msgFont.txt
	if (!html.contains(".png") && !html.contains("</span>")) {
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();  //保存用户输入的文本（图片也会转化为文本）
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

	ui.textEdit->clear();//清空消息
	ui.textEdit->deleteAllEmotionImage();//删除所有表情
	ui.msgWidget->appendMsg(html);//追加消息（信息窗口添加消息）
	
	
}