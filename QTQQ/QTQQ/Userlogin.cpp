#include "Userlogin.h"
#include"CCMainWindow.h"
#include<QMessageBox>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDebug>
#include<QSqlError>
QString gLoginEmployeeID;//��½��QQ��
Userlogin::Userlogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);//���ô�������
	this->initTitleBar();//��ʼ��������
	//this->setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");//���ñ���
	this->setTitleBarTitle("", ":/Resources/MainWindow/aio_toobar_report.png");//���ñ���
	loadStyleSheet("UserLogin");
	initControl();
}

Userlogin::~Userlogin()
{
}

//��ʼ���ؼ�
void Userlogin::initControl() {
	QLabel*headlabel = new QLabel(this);//ͷ��
	headlabel->setFixedSize(68, 68);
	//��ȡ�յ�Բͷ��
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"),pix,headlabel->size()));//Բͷ�����óɱ�ǩ�����
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &Userlogin::onLoginBtnClicked);
	if (!connectMySql()) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�������ݿ�ʧ��"));
		close();
	}
}
bool Userlogin::connectMySql()
{

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");		 //���ݿ����ƣ��������õı���һ�£�
	db.setHostName("localhost");	 //������
	db.setUserName("root");			 //�û���
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
bool Userlogin::veryfyAccountCode(bool& isAccountLogin, QString& strAccount)
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();
	//����Ա���ţ�QQ�ŵ�¼��
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID=%1").arg(strAccountInput);
	QSqlQuery queryEmployeeID(strSqlCode);//��ѯ
	queryEmployeeID.exec();//ִ��

	//�������Ա���ŵ�¼�Ļ�
	if (queryEmployeeID.first()) {//ָ��������һ��(���ȷʵ��emplyeeID����¼�����ҵ���Ӧ����)
		//���ݿ���qq�Ŷ�Ӧ������
		QString strCode=queryEmployeeID.value(0).toString();//��Ϊ����ֻ�ǲ�һ��code������Ϊ0����"SELECT code��emoployeeID FROM tab_accounts WHERE employeeID=%1"������employeeID������Ϊ1
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

	//�˺ŵ�¼
	strSqlCode = QString("SELECT code,employeeID FROM tab_accounts WHERE account ='%1'")
		.arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();
	if (queryAccount.first()) {
		QString strCode = queryAccount.value(0).toString();
		//��֤����
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
	QString strAccount;//�п������˺Ż���qq��
	if (!veryfyAccountCode(isAccountLogin,strAccount)) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��������˺Ż�������������������"));
		//ui.editPassword->setText("");
		//ui.editPassword->setText("");
		return;
	}

	//���µ�½״̬Ϊ��¼
	QString strSqlStatus = QString("UPDATE tab_employees SET online=2 WHERE employeeID=%1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	close();
	CCMainWindow*mainwindow = new CCMainWindow(strAccount,isAccountLogin);
	mainwindow->show();
}