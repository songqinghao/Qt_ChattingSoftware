#include "Userlogin.h"
#include"CCMainWindow.h"
#include<QMessageBox>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDebug>
#include<QSqlError>
QString gLoginEmployeeID;//登陆者QQ号
Userlogin::Userlogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);//设置窗口属性
	this->initTitleBar();//初始化标题栏
	//this->setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");//设置标题
	this->setTitleBarTitle("", ":/Resources/MainWindow/aio_toobar_report.png");//设置标题
	loadStyleSheet("UserLogin");
	initControl();
}

Userlogin::~Userlogin()
{
}

//初始化控件
void Userlogin::initControl() {
	QLabel*headlabel = new QLabel(this);//头像
	headlabel->setFixedSize(68, 68);
	//获取空的圆头像
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"),pix,headlabel->size()));//圆头像设置成标签的外表
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &Userlogin::onLoginBtnClicked);
	if (!connectMySql()) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("连接数据库失败"));
		close();
	}
}
bool Userlogin::connectMySql()
{

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");		 //数据库名称（和你设置的保持一致）
	db.setHostName("localhost");	 //主机名
	db.setUserName("root");			 //用户名
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
bool Userlogin::veryfyAccountCode(bool& isAccountLogin, QString& strAccount)
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();
	//输入员工号（QQ号登录）
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID=%1").arg(strAccountInput);
	QSqlQuery queryEmployeeID(strSqlCode);//查询
	queryEmployeeID.exec();//执行

	//如果是用员工号登录的话
	if (queryEmployeeID.first()) {//指向结果集第一条(如果确实是emplyeeID来登录即能找到对应密码)
		//数据库中qq号对应的密码
		QString strCode=queryEmployeeID.value(0).toString();//因为我们只是查一个code所以列为0，（"SELECT code，emoployeeID FROM tab_accounts WHERE employeeID=%1"）在这employeeID就是列为1
		if (strCode == strCodeInput) {
			gLoginEmployeeID = strAccountInput;
			isAccountLogin = false;
			strAccount = strAccountInput;
			return true;
		}
		else {
			return false;
		}
	}

	//账号登录
	strSqlCode = QString("SELECT code,employeeID FROM tab_accounts WHERE account ='%1'")
		.arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();
	if (queryAccount.first()) {
		QString strCode = queryAccount.value(0).toString();
		//验证密码
		if (strCode == strCodeInput) {
			gLoginEmployeeID = queryAccount.value(1).toString();
			isAccountLogin = true;
			strAccount = strAccountInput;
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}
void Userlogin::onLoginBtnClicked() {
	bool isAccountLogin;
	QString strAccount;//有可能是账号或者qq号
	if (!veryfyAccountCode(isAccountLogin,strAccount)) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("你输入的账号或密码有误请重新输入"));
		//ui.editPassword->setText("");
		//ui.editPassword->setText("");
		return;
	}

	//更新登陆状态为登录
	QString strSqlStatus = QString("UPDATE tab_employees SET online=2 WHERE employeeID=%1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	close();
	CCMainWindow*mainwindow = new CCMainWindow(strAccount,isAccountLogin);
	mainwindow->show();
}