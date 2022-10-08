#include "QtQQ_Server.h"
#include<QSqlDatabase>
#include<QMessageBox>
#include<qheaderview.h>
#include<QTableWidgetItem>
#include<QDebug>
#include<qsqlerror.h>
#include<QSqlRecord>
#include<QSqlQuery>
#include<QFileDialog>
QtQQ_Server::QtQQ_Server(QWidget *parent)
	: QDialog(parent),m_pixPath("")
{
	ui.setupUi(this);
	if (!connectMySql()) {
		QMessageBox::warning(NULL, QString::fromLocal8Bit("��ʾ"), 
			QString::fromLocal8Bit("�������ݿ�ʧ��"));
		close();
		return;
	}

	setDepNameMap();
	setStatusMap();
	setOnlineMap();
	initComboBoxData();

	m_queryInfoModel.setQuery("SELECT * FROM tab_employees");
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	//��ʼ����ѯ��˾Ⱥ������Ա����Ϣ
	m_employeeID = 0;
	m_depID = getComDepID();
	m_compDepID = m_depID;

	
	updateTableData();//���±������
	//���ü�ʱ������ʱˢ������
	m_timer = new QTimer(this);
	m_timer->setInterval(200);//200���봥��һ��
	m_timer->start();
	connect(m_timer, &QTimer::timeout, this, &QtQQ_Server::onRefresh);

	initTcpSocket();
	initUdpSocket();
}
//��ʼ����Ͽ�Ա������+Ա����ѯ����
void QtQQ_Server::initComboBoxData()
{
	QString itemText;//��Ͽ����Ŀ�ı�
	//��ȡ��˾�ܵĲ��Ÿ���
	QSqlQueryModel queryDepModel;
	queryDepModel.setQuery("SELECT * FROM tab_department");
	int depCounts = queryDepModel.rowCount() - 1;//�������������㹫˾Ⱥ��

	for (int i = 0; i < depCounts; i++) 
	{
		itemText = ui.employeeDepBox->itemText(i);
		//�Ҳ������ƶ�Ӧ�Ĳ���ID
		QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'").arg(itemText));
		queryDepID.exec();
		queryDepID.first();

		//����Ա������������Ͽ������Ϊ��Ӧ�Ĳ���QQ��
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());
	}
	//��һ����˾Ⱥ����
	for (int i = 0; i < depCounts+1; i++) {
		itemText = ui.departmentBox->itemText(i);
		QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'").arg(itemText));
		queryDepID.exec();
		queryDepID.first();

		//���ò�����Ͽ������Ϊ��Ӧ�Ĳ���qq��
		ui.departmentBox->setItemData(i, queryDepID.value(0).toInt());
		
	}
}

void QtQQ_Server::initTcpSocket()
{
	m_tcpServer = new TcpServer(gtcpPort);
	m_tcpServer->run();

	//�յ�tcp�ͻ��˷�������Ϣ����е�UDP�㲥
	connect(m_tcpServer, &TcpServer::signalTcpMsgComes, this, &QtQQ_Server::onUDPbroadMsg);
}

void QtQQ_Server::initUdpSocket()
{
	m_udpSend = new QUdpSocket(this);

}

void QtQQ_Server::setDepNameMap()
{
	m_depNameMap.insert(QStringLiteral("2001"), QStringLiteral("���²�"));
	m_depNameMap.insert(QStringLiteral("2002"), QStringLiteral("�з���"));
	m_depNameMap.insert(QStringLiteral("2003"), QStringLiteral("�г���"));
}

void QtQQ_Server::setStatusMap()
{
	m_statusMap.insert(QStringLiteral("1"), QStringLiteral("��Ч"));
	m_statusMap.insert(QStringLiteral("0"), QStringLiteral("��ע��"));

}

void QtQQ_Server::setOnlineMap()
{
	m_onlineMap.insert(QStringLiteral("1"), QStringLiteral("����"));
	m_onlineMap.insert(QStringLiteral("2"), QStringLiteral("����"));
	m_onlineMap.insert(QStringLiteral("3"), QStringLiteral("����"));
}

void QtQQ_Server::onUDPbroadMsg(QByteArray & btData)
{
	for (quint16 port = gudpPort; port < gudpPort + 200; port++)
	{
		//����Щ�˿�д������(�㲥д������)
		m_udpSend->writeDatagram(btData, btData.size(), QHostAddress::Broadcast,port);
	}
}


bool QtQQ_Server::connectMySql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");//���ݿ����ƣ��������õı���һ�£�
	db.setHostName("localhost");//������
	db.setUserName("root");//�û���
	db.setPassword("123456");//���������Mysql���˺�����һ��
	db.setPort(3306);//���ö˿ں�
	if (db.open()) {
		return true;
	}
	else {
		qDebug() << "��ʧ��" << db.lastError().text();
		return false;
	}
}

int QtQQ_Server::getComDepID()
{
	QSqlQuery queryComDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'")
		.arg(QString::fromLocal8Bit("��˾Ⱥ")));
	queryComDepID.exec();
	queryComDepID.first();
	int comDepID = queryComDepID.value(0).toInt();

	return comDepID;
}

void QtQQ_Server::updateTableData(int depID, int employeeID)
{
	//����
	ui.tableWidget->clear();

	if (depID&&depID!=m_compDepID) //���ǹ�˾Ⱥ
	{		
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees WHERE departmentID=%1").arg(depID));
	}
	else if (employeeID) {
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	}
	else//�ǹ�˾Ⱥ
	{
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees"));
	}
	int rows = m_queryInfoModel.rowCount();//������
	int columns = m_queryInfoModel.columnCount();//�����������ֶ�����
	QModelIndex index;//ģ������
	//���ñ�������������
	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);
	
	//���ñ�ͷ
	QStringList headers;
	headers << QStringLiteral("����") << QStringLiteral("����")
		<< QStringLiteral("Ա������") << QStringLiteral("Ա��ǩ��")
		<< QStringLiteral("Ա��״̬") << QStringLiteral("Ա��ͷ��") << QStringLiteral("����״̬");
	ui.tableWidget->setHorizontalHeaderLabels(headers);

	//�����еȿ�
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			index = m_queryInfoModel.index(i, j);//�У���
			QString strData = m_queryInfoModel.data(index).toString();//��ȡi��j�е�����

			//��ȡ�ֶ�����
			QSqlRecord record = m_queryInfoModel.record(i);//��ȡ��ǰ�еļ�¼
			QString strRecordName = record.fieldName(j);//�У���ȡ����¼�����ƣ�
			if (strRecordName == QLatin1String("departmentID")) {
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_depNameMap.value(strData)));
				continue;
			}
			else if (strRecordName == QLatin1String("status")) {
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_statusMap.value(strData)));
				continue;
			}
			else if (strRecordName == QLatin1String("online")) {
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_onlineMap.value(strData)));
				continue;
			}
			ui.tableWidget->setItem(i, j, new QTableWidgetItem(strData));
		}
	}
}

void QtQQ_Server::onRefresh()
{
	updateTableData(m_depID, m_employeeID);
}

void QtQQ_Server::on_queryDepartmentBtn_clicked()
{
	ui.queryIDLineEdit->clear();
	m_employeeID = 0;
	m_depID = ui.departmentBox->currentData().toInt();
	updateTableData(m_depID,m_employeeID);
}

void QtQQ_Server::on_queryIDBtn_clicked()
{
	ui.departmentBox->setCurrentIndex(0);
	m_depID = m_compDepID;

	//���Ա��qq���Ƿ�����
	if (!ui.queryIDLineEdit->text().length()) {
		QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("������Ա��qq��"));
		ui.queryIDLineEdit->setFocus();
		return;
	}
	//��ȡԱ��qq��
	int employeeID = ui.queryIDLineEdit->text().toInt();

	QSqlQuery queryInfo(QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	queryInfo.exec();

	//���Ա�����Ƿ�������ȷ
	if (!queryInfo.first()) {
		QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("��������ȷ��Ա��qq��"));
		ui.queryIDLineEdit->setFocus();
		return;
	}
	else {
		m_employeeID = employeeID;
		//updateTableData(m_depID, m_employeeID);
	}
}

//ע��
void QtQQ_Server::on_logoutBtn_clicked()
{
	ui.departmentBox->setCurrentIndex(0);
	ui.queryIDLineEdit->clear();

	//���Ա��qq���Ƿ�����
	if (!ui.logoutIDLineEdit->text().length()) {
		QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("������Ա��qq��"));
		ui.queryIDLineEdit->setFocus();
		return;
	}

	//��ȡԱ��qq��
	int employeeID = ui.logoutIDLineEdit->text().toInt();

	QSqlQuery queryInfo(QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	queryInfo.exec();

	//���Ա�����Ƿ�������ȷ
	if (!queryInfo.first()) {
		QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("��������ȷ��Ա��qq��"));
		ui.queryIDLineEdit->setFocus();
		return;
	}
	else {
		//ע���������������ݿ��е����ݣ���Ա����status����Ϊ0
		QSqlQuery sqlUpdate(QString("UPDATE tab_employees SET status=0 WHERE employeeID=%1").arg(employeeID));
		sqlUpdate.exec();

		//��ȡע��Ա������
		QString strName = queryInfo.value(0).toString();
		QSqlQuery sqlQueryName(QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1").arg(employeeID));
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("Ա�� %1 ����ҵqq:%2 �ѱ�ע������")
			.arg(strName).arg(employeeID));
		ui.logoutIDLineEdit->clear();
	}
}

void QtQQ_Server::on_selectPictureBtn_clicked()
{
	m_pixPath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ��ͷ��"),
		".","*.png;;*.jpg");
	//���Ƿ���Ч
	if (!m_pixPath.size()) {
		return;
	}
	//��ʾ����ǩ
	QPixmap pixmap;
	pixmap.load(m_pixPath);
	//��ı���
	qreal widthRatio = (qreal)ui.headLabel->width() / (qreal)pixmap.width();
	qreal heightRatio = (qreal)ui.headLabel->height() / (qreal)pixmap.height();
	
	QSize size(pixmap.width()*widthRatio, pixmap.height()*heightRatio);
	//QSize size(ui.headLabel->width(), ui.headLabel->height());
	ui.headLabel->setPixmap(pixmap.scaled(size));
}

void QtQQ_Server::on_addBtn_clicked()
{
	//���Ա������������
	QString strName = ui.nameLineEdit->text();
	if (!strName.size()) {
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), 
			QString::fromLocal8Bit("������Ա��������"));
		ui.nameLineEdit->clear();
		return;
	}

	//���Ա��ͷ���Ƿ�����
	if (!m_pixPath.size()) {
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("��ѡ��Ա��ͷ��·����"));
		return;
	}

	//���ݿ�����µ�Ա������
	//��ȡԱ��qq��
	QSqlQuery maxEmployeeID("SELECT MAX(employeeID) FROM tab_employees");
	maxEmployeeID.exec();
	maxEmployeeID.first();
	int employeeID = maxEmployeeID.value(0).toInt() + 1;

	//��ȡԱ������qq��
	int depID = ui.employeeDepBox->currentData().toInt();

	//ͼƬ·������Ϊxxx\xxx\xxx.png
	m_pixPath.replace("/", "\\\\");//��/���滻Ϊ��\��
	QSqlQuery insertSql(QString("INSERT INTO tab_employees(departmentID,employeeID,employee_name,picture)\
						VALUES(%1,%2,'%3','%4')").arg(depID).arg(employeeID).arg(strName).arg(m_pixPath));
	insertSql.exec();
	/*******************�޸�����2022/5/19*******************************************/
	QSqlQuery insertAccount(QString("INSERT INTO tab_accounts(employeeID,account,code)VALUES(%1,'%2','%3')")
							.arg(employeeID).arg(strName).arg(strName));
	insertAccount.exec();
	/**************************************************************/
	QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
		QString::fromLocal8Bit("����Ա���ɹ���"));
	m_pixPath = "";
	ui.headLabel->setText(QString::fromLocal8Bit("Ա������"));
	ui.nameLineEdit->clear();
}	
