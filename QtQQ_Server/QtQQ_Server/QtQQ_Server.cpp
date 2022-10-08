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
		QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), 
			QString::fromLocal8Bit("连接数据库失败"));
		close();
		return;
	}

	setDepNameMap();
	setStatusMap();
	setOnlineMap();
	initComboBoxData();

	m_queryInfoModel.setQuery("SELECT * FROM tab_employees");
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	//初始化查询公司群的所有员工信息
	m_employeeID = 0;
	m_depID = getComDepID();
	m_compDepID = m_depID;

	
	updateTableData();//更新表的数据
	//利用计时器来定时刷新数据
	m_timer = new QTimer(this);
	m_timer->setInterval(200);//200毫秒触发一次
	m_timer->start();
	connect(m_timer, &QTimer::timeout, this, &QtQQ_Server::onRefresh);

	initTcpSocket();
	initUdpSocket();
}
//初始化组合框员工部门+员工查询部门
void QtQQ_Server::initComboBoxData()
{
	QString itemText;//组合框的项目文本
	//获取公司总的部门个数
	QSqlQueryModel queryDepModel;
	queryDepModel.setQuery("SELECT * FROM tab_department");
	int depCounts = queryDepModel.rowCount() - 1;//部门总数（不算公司群）

	for (int i = 0; i < depCounts; i++) 
	{
		itemText = ui.employeeDepBox->itemText(i);
		//找部门名称对应的部门ID
		QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'").arg(itemText));
		queryDepID.exec();
		queryDepID.first();

		//设置员工所属部门组合框的数据为相应的部门QQ号
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());
	}
	//多一个公司群部门
	for (int i = 0; i < depCounts+1; i++) {
		itemText = ui.departmentBox->itemText(i);
		QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'").arg(itemText));
		queryDepID.exec();
		queryDepID.first();

		//设置部门组合框的数据为相应的部门qq号
		ui.departmentBox->setItemData(i, queryDepID.value(0).toInt());
		
	}
}

void QtQQ_Server::initTcpSocket()
{
	m_tcpServer = new TcpServer(gtcpPort);
	m_tcpServer->run();

	//收到tcp客户端发来的消息后进行的UDP广播
	connect(m_tcpServer, &TcpServer::signalTcpMsgComes, this, &QtQQ_Server::onUDPbroadMsg);
}

void QtQQ_Server::initUdpSocket()
{
	m_udpSend = new QUdpSocket(this);

}

void QtQQ_Server::setDepNameMap()
{
	m_depNameMap.insert(QStringLiteral("2001"), QStringLiteral("人事部"));
	m_depNameMap.insert(QStringLiteral("2002"), QStringLiteral("研发部"));
	m_depNameMap.insert(QStringLiteral("2003"), QStringLiteral("市场部"));
}

void QtQQ_Server::setStatusMap()
{
	m_statusMap.insert(QStringLiteral("1"), QStringLiteral("有效"));
	m_statusMap.insert(QStringLiteral("0"), QStringLiteral("已注销"));

}

void QtQQ_Server::setOnlineMap()
{
	m_onlineMap.insert(QStringLiteral("1"), QStringLiteral("离线"));
	m_onlineMap.insert(QStringLiteral("2"), QStringLiteral("在线"));
	m_onlineMap.insert(QStringLiteral("3"), QStringLiteral("隐身"));
}

void QtQQ_Server::onUDPbroadMsg(QByteArray & btData)
{
	for (quint16 port = gudpPort; port < gudpPort + 200; port++)
	{
		//对这些端口写入数据(广播写入数据)
		m_udpSend->writeDatagram(btData, btData.size(), QHostAddress::Broadcast,port);
	}
}


bool QtQQ_Server::connectMySql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");//数据库名称（和你设置的保持一致）
	db.setHostName("localhost");//主机名
	db.setUserName("root");//用户名
	db.setPassword("123456");//密码和我们Mysql的账号密码一致
	db.setPort(3306);//设置端口号
	if (db.open()) {
		return true;
	}
	else {
		qDebug() << "打开失败" << db.lastError().text();
		return false;
	}
}

int QtQQ_Server::getComDepID()
{
	QSqlQuery queryComDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'")
		.arg(QString::fromLocal8Bit("公司群")));
	queryComDepID.exec();
	queryComDepID.first();
	int comDepID = queryComDepID.value(0).toInt();

	return comDepID;
}

void QtQQ_Server::updateTableData(int depID, int employeeID)
{
	//清理
	ui.tableWidget->clear();

	if (depID&&depID!=m_compDepID) //不是公司群
	{		
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees WHERE departmentID=%1").arg(depID));
	}
	else if (employeeID) {
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	}
	else//是公司群
	{
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees"));
	}
	int rows = m_queryInfoModel.rowCount();//总行数
	int columns = m_queryInfoModel.columnCount();//总列数（总字段数）
	QModelIndex index;//模型索引
	//设置表格的行数和列数
	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);
	
	//设置表头
	QStringList headers;
	headers << QStringLiteral("部门") << QStringLiteral("工号")
		<< QStringLiteral("员工姓名") << QStringLiteral("员工签名")
		<< QStringLiteral("员工状态") << QStringLiteral("员工头像") << QStringLiteral("在线状态");
	ui.tableWidget->setHorizontalHeaderLabels(headers);

	//设置列等宽
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			index = m_queryInfoModel.index(i, j);//行，列
			QString strData = m_queryInfoModel.data(index).toString();//获取i行j列的数据

			//获取字段名称
			QSqlRecord record = m_queryInfoModel.record(i);//获取当前行的记录
			QString strRecordName = record.fieldName(j);//列（获取到记录的名称）
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

	//检测员工qq号是否输入
	if (!ui.queryIDLineEdit->text().length()) {
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入员工qq号"));
		ui.queryIDLineEdit->setFocus();
		return;
	}
	//获取员工qq号
	int employeeID = ui.queryIDLineEdit->text().toInt();

	QSqlQuery queryInfo(QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	queryInfo.exec();

	//检测员工号是否输入正确
	if (!queryInfo.first()) {
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入正确的员工qq号"));
		ui.queryIDLineEdit->setFocus();
		return;
	}
	else {
		m_employeeID = employeeID;
		//updateTableData(m_depID, m_employeeID);
	}
}

//注销
void QtQQ_Server::on_logoutBtn_clicked()
{
	ui.departmentBox->setCurrentIndex(0);
	ui.queryIDLineEdit->clear();

	//检测员工qq号是否输入
	if (!ui.logoutIDLineEdit->text().length()) {
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入员工qq号"));
		ui.queryIDLineEdit->setFocus();
		return;
	}

	//获取员工qq号
	int employeeID = ui.logoutIDLineEdit->text().toInt();

	QSqlQuery queryInfo(QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	queryInfo.exec();

	//检测员工号是否输入正确
	if (!queryInfo.first()) {
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("请输入正确的员工qq号"));
		ui.queryIDLineEdit->setFocus();
		return;
	}
	else {
		//注销操作，更新数据库中的数据，将员工的status设置为0
		QSqlQuery sqlUpdate(QString("UPDATE tab_employees SET status=0 WHERE employeeID=%1").arg(employeeID));
		sqlUpdate.exec();

		//获取注销员工姓名
		QString strName = queryInfo.value(0).toString();
		QSqlQuery sqlQueryName(QString("SELECT employee_name FROM tab_employees WHERE employeeID=%1").arg(employeeID));
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("员工 %1 的企业qq:%2 已被注销！！")
			.arg(strName).arg(employeeID));
		ui.logoutIDLineEdit->clear();
	}
}

void QtQQ_Server::on_selectPictureBtn_clicked()
{
	m_pixPath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择头像"),
		".","*.png;;*.jpg");
	//看是否有效
	if (!m_pixPath.size()) {
		return;
	}
	//显示到标签
	QPixmap pixmap;
	pixmap.load(m_pixPath);
	//宽的比例
	qreal widthRatio = (qreal)ui.headLabel->width() / (qreal)pixmap.width();
	qreal heightRatio = (qreal)ui.headLabel->height() / (qreal)pixmap.height();
	
	QSize size(pixmap.width()*widthRatio, pixmap.height()*heightRatio);
	//QSize size(ui.headLabel->width(), ui.headLabel->height());
	ui.headLabel->setPixmap(pixmap.scaled(size));
}

void QtQQ_Server::on_addBtn_clicked()
{
	//检测员工姓名的输入
	QString strName = ui.nameLineEdit->text();
	if (!strName.size()) {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), 
			QString::fromLocal8Bit("请输入员工姓名！"));
		ui.nameLineEdit->clear();
		return;
	}

	//检测员工头像是否输入
	if (!m_pixPath.size()) {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("请选择员工头像路径！"));
		return;
	}

	//数据库插入新的员工数据
	//获取员工qq号
	QSqlQuery maxEmployeeID("SELECT MAX(employeeID) FROM tab_employees");
	maxEmployeeID.exec();
	maxEmployeeID.first();
	int employeeID = maxEmployeeID.value(0).toInt() + 1;

	//获取员工部门qq号
	int depID = ui.employeeDepBox->currentData().toInt();

	//图片路径设置为xxx\xxx\xxx.png
	m_pixPath.replace("/", "\\\\");//“/”替换为“\”
	QSqlQuery insertSql(QString("INSERT INTO tab_employees(departmentID,employeeID,employee_name,picture)\
						VALUES(%1,%2,'%3','%4')").arg(depID).arg(employeeID).arg(strName).arg(m_pixPath));
	insertSql.exec();
	/*******************修改日期2022/5/19*******************************************/
	QSqlQuery insertAccount(QString("INSERT INTO tab_accounts(employeeID,account,code)VALUES(%1,'%2','%3')")
							.arg(employeeID).arg(strName).arg(strName));
	insertAccount.exec();
	/**************************************************************/
	QMessageBox::information(this, QString::fromLocal8Bit("提示"),
		QString::fromLocal8Bit("新增员工成功！"));
	m_pixPath = "";
	ui.headLabel->setText(QString::fromLocal8Bit("员工寸照"));
	ui.nameLineEdit->clear();
}	
