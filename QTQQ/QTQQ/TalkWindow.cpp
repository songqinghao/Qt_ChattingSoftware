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
	//��ӱ������
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
		//��ʼ��Ⱥ��
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
	if (rows == 0) {//����
		m_isGroupTalk = false;
	}
	else {
		m_isGroupTalk = true;
	}
}
int TalkWindow::getCompDepID()
{
	QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'")
					.arg(QString::fromLocal8Bit("��˾Ⱥ")));

	queryDepID.exec();
	queryDepID.first();
	return queryDepID.value(0).toInt();
}

void TalkWindow::initTalkWindow()
{
	//����һ������
	QTreeWidgetItem*pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//���ú���ָʾ���Ĳ���(�����Ƿ��к���������ʱ�ڶ���չ��)

	//����data��������������Լ�����
	pRootItem->setData(0, Qt::UserRole, 0);//��0�У���ɫΪUserRole,ֵΪ0
	RootContatItem*pItemName = new RootContatItem(false, ui.treeWidget);//��ͷΪfalse,����ΪtreeWidget
	ui.treeWidget->setFixedHeight(646);//shell�ĸ߶�-shellͷ�ߣ�talkwindow titleWidget�߶ȣ�
	
	//��ǰ����Ⱥ����������
	QString strGroupName;
	QSqlQuery queryGroupName(QString("SELECT department_name FROM tab_department WHERE departmentID=%1").arg(m_talkId));
	queryGroupName.exec();
	if (queryGroupName.first()) {//�м�¼
		strGroupName = queryGroupName.value(0).toString();
	}
	
	
	QSqlQueryModel queryEmployeeModel;
	
	//����ǹ�˾Ⱥ
	if (getCompDepID() == m_talkId.toInt()) {
		queryEmployeeModel.setQuery("SELECT employeeID FROM tab_employees WHERE status=1");
	}
	else {
		queryEmployeeModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status=1 AND departmentID=%1").arg(m_talkId));

	}
	int nEmployeeNum = queryEmployeeModel.rowCount();//Ⱥ����
	
	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3")
		.arg(strGroupName)
		.arg(0).
		arg(nEmployeeNum);
	pItemName->setText(qsGroupName);

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem,0,pItemName);//���ò���pRootItem�ڵ�0��,����ΪpItemName

	//չ��
	pRootItem->setExpanded(true);
	for (int i = 0; i < nEmployeeNum; i++) 
	{
		QModelIndex modelIndex = queryEmployeeModel.index(i,0);//�к�Ϊi�к�Ϊ0----����ȡģʽ����
		int employeeID=queryEmployeeModel.data(modelIndex).toInt();//��ȡԱ����Ա����
		

		////���˵��Լ�
		//if (employeeID == gLoginEmployeeID.toInt())
		//{
		//	continue;
		//}
		//����ӽڵ�
		addPeopInfo(pRootItem, employeeID);
	}

}
//void TalkWindow::initCompanyTalk()
//{
//	//����һ������
//	QTreeWidgetItem*pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//���ú���ָʾ���Ĳ���(�����Ƿ��к���������ʱ�ڶ���չ��)
//
//	//����data��������������Լ�����
//	pRootItem->setData(0, Qt::UserRole, 0);//��0�У���ɫΪUserRole,ֵΪ0
//	RootContatItem*pItemName = new RootContatItem(false, ui.treeWidget);//��ͷΪfalse,����ΪtreeWidget
//	ui.treeWidget->setFixedHeight(646);//shell�ĸ߶�-shellͷ�ߣ�talkwindow titleWidget�߶ȣ�
//	int nEmployeeNum = 50;//��������
//	QString qsGroupName = QString::fromLocal8Bit("��˾Ⱥ %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	//�������ڵ�
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem,0,pItemName);//���ò���pRootItem�ڵ�0��,����ΪpItemName
//
//	//չ��
//	pRootItem->setExpanded(true);
//	for (int i = 0; i < nEmployeeNum; i++) 
//	{
//		addPeopInfo(pRootItem);
//	}
//}
//void TalkWindow::initPersonelTalk()
//{
//	//����һ������
//	QTreeWidgetItem*pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//���ú���ָʾ���Ĳ���(�����Ƿ��к���������ʱ�ڶ���չ��)
//
//	//����data��������������Լ�����
//	pRootItem->setData(0, Qt::UserRole, 0);//��0�У���ɫΪUserRole,ֵΪ0
//	RootContatItem*pItemName = new RootContatItem(false, ui.treeWidget);//��ͷΪfalse,����ΪtreeWidget
//	ui.treeWidget->setFixedHeight(646);//shell�ĸ߶�-shellͷ�ߣ�talkwindow titleWidget�߶ȣ�
//	int nEmployeeNum = 5;//��������
//	QString qsGroupName = QString::fromLocal8Bit("���²� %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	//�������ڵ�
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);//���ò���pRootItem�ڵ�0��,����ΪpItemName
//
//	//չ��
//	pRootItem->setExpanded(true);
//	for (int i = 0; i < nEmployeeNum; i++)
//	{
//		addPeopInfo(pRootItem);
//	}
//}
//void TalkWindow::initMarketTalk()
//{
//	//����һ������
//	QTreeWidgetItem*pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//���ú���ָʾ���Ĳ���(�����Ƿ��к���������ʱ�ڶ���չ��)
//
//	//����data��������������Լ�����
//	pRootItem->setData(0, Qt::UserRole, 0);//��0�У���ɫΪUserRole,ֵΪ0
//	RootContatItem*pItemName = new RootContatItem(false, ui.treeWidget);//��ͷΪfalse,����ΪtreeWidget
//	ui.treeWidget->setFixedHeight(646);//shell�ĸ߶�-shellͷ�ߣ�talkwindow titleWidget�߶ȣ�
//	int nEmployeeNum = 8;//��������
//	QString qsGroupName = QString::fromLocal8Bit("�г��� %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	//�������ڵ�
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);//���ò���pRootItem�ڵ�0��,����ΪpItemName
//
//	//չ��
//	pRootItem->setExpanded(true);
//	for (int i = 0; i < nEmployeeNum; i++)
//	{
//		addPeopInfo(pRootItem);
//	}
//}
//void TalkWindow::initDevelopTalk()
//{
//	//����һ������
//	QTreeWidgetItem*pRootItem = new QTreeWidgetItem();
//	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//���ú���ָʾ���Ĳ���(�����Ƿ��к���������ʱ�ڶ���չ��)
//
//	//����data��������������Լ�����
//	pRootItem->setData(0, Qt::UserRole, 0);//��0�У���ɫΪUserRole,ֵΪ0
//	RootContatItem*pItemName = new RootContatItem(false, ui.treeWidget);//��ͷΪfalse,����ΪtreeWidget
//	ui.treeWidget->setFixedHeight(646);//shell�ĸ߶�-shellͷ�ߣ�talkwindow titleWidget�߶ȣ�
//	int nEmployeeNum = 32;//��������
//	QString qsGroupName = QString::fromLocal8Bit("�з��� %1/%2").arg(0).arg(nEmployeeNum);
//	pItemName->setText(qsGroupName);
//
//	//�������ڵ�
//	ui.treeWidget->addTopLevelItem(pRootItem);
//	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);//���ò���pRootItem�ڵ�0��,����ΪpItemName
//
//	//չ��
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
	
	
	//����ӽڵ�
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1,employeeID);
	ContactItem*pContactItem = new ContactItem(ui.treeWidget);
	
	

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	
	//��ȡ����ǩ����ͷ��
	QString strName, strSign, strPicturePath;
	QSqlQueryModel queryInfoModel;
	queryInfoModel.setQuery(QString("SELECT employee_name,employee_sign,picture FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	
	//����ģ������
	QModelIndex nameIndex, signIndex, pictureIndex;
	nameIndex = queryInfoModel.index(0, 0);//����
	signIndex = queryInfoModel.index(0, 1);
	pictureIndex = queryInfoModel.index(0, 2);

	strName = queryInfoModel.data(nameIndex).toString();
	strSign = queryInfoModel.data(signIndex).toString();
	strPicturePath = queryInfoModel.data(pictureIndex).toString();

	//����ͷ��
	QImage imageHead;
	imageHead.load(strPicturePath);
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(QPixmap::fromImage(imageHead), pix1, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(strName);
	pContactItem->setSignName(strSign);

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);//���ƽ�pChild��pContactItem������

	//����(��ϵ��)�ʹ��壨��ϵ�ˣ�����һһ��Ӧ��ӳ��
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
		//��ȡ����������
		QString strPeopleName = m_groupPeopleMap.value(item);

		//��Ӵ���
		WindowManager::getInstance()->addNewTalkWindow(item->data(0,Qt::UserRole+1).toString()/*, PTOP, strPeopleName*/);
	}
}

//������Ϣ��ť�����Ĳ۷���
void TalkWindow::onSendBtnClicked(bool) 
{
	if (ui.textEdit->toPlainText().isEmpty()) {
		//����(��������촰��)ת��Ϊȫ��(������������Ͻ�)����+�ı���Ϣ+������+��С��120��100�ߣ�+��ʾʱ��Ϊ2000����
		QToolTip::showText(this->mapToGlobal(QPoint(630,660)),QString::fromLocal8Bit("������Ϣ������Ϊ�գ�"),this,QRect(0,0,120,100),2000);
		return;
	}

	QString html=ui.textEdit->document()->toHtml();//ת��Ϊhtml
	
	//�ı�html���û���������������msgFont.txt
	if (!html.contains(".png") && !html.contains("</span>")) {
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();  //�����û�������ı���ͼƬҲ��ת��Ϊ�ı���
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

	ui.textEdit->clear();//�����Ϣ
	ui.textEdit->deleteAllEmotionImage();//ɾ�����б���
	ui.msgWidget->appendMsg(html);//׷����Ϣ����Ϣ���������Ϣ��
	
	
}