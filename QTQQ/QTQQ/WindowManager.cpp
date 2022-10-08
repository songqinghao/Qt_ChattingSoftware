#include "WindowManager.h"
#include"TalkWindow.h"
#include"TalkWindowShell.h"
#include"TalkWindowItem.h"
#include<QSqlQueryModel>
//����ģʽ������ȫ�־�̬����

Q_GLOBAL_STATIC(WindowManager, theInstance);


WindowManager::WindowManager():QObject(nullptr),m_talkwindowshell(nullptr)
{

}

WindowManager::~WindowManager()
{
}

QWidget * WindowManager::findWindowName(const QString & qsWindowName)
{
	if (m_windowMap.contains(qsWindowName)) {
		return m_windowMap.value(qsWindowName);
	}
	return nullptr;
}

void WindowManager::deleteWindowName(const QString & qsWindowName)
{
	m_windowMap.remove(qsWindowName);
}
//��Ӵ�����
void WindowManager::addWindowName(const QString & qsWindowName, QWidget * qWidget)
{
	if (!m_windowMap.contains(qsWindowName)) {
		m_windowMap.insert(qsWindowName, qWidget);
	}
}

WindowManager * WindowManager::getInstance()
{
	return theInstance();
}

TalkWindowShell * WindowManager::getTalkWindowShell()
{
	return m_talkwindowshell;
}

QString WindowManager::getCreatingTalkId()
{
	return m_strCreateingTalkId;
}

void WindowManager::addNewTalkWindow(const QString & uid/*, GroupType groupType, const QString & strPeople*/)
{
	if (m_talkwindowshell == nullptr){//����������û�д�������

		m_talkwindowshell = new TalkWindowShell;
		connect(m_talkwindowshell, &TalkWindowShell::destroyed, [this](QObject*obj) {
			m_talkwindowshell = nullptr;
		});
	}

	QWidget*widget=findWindowName(uid);
	if (!widget) {
		m_strCreateingTalkId = uid;
		TalkWindow*talkwindow = new TalkWindow(m_talkwindowshell, uid/*, groupType*/);
		TalkWindowItem*talkwindowItem = new TalkWindowItem(talkwindow);
		
		//���������������Ϊ��""
		m_strCreateingTalkId = "";
		
		//�ж���Ⱥ�Ļ��Ƕ���
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID=%1").arg(uid);
		//���ò�ѯ
		sqlDepModel.setQuery(strSql);
		int rows = sqlDepModel.rowCount();//����ѯ����м���
		QString strWindowName, strMsgLabel;
		if (rows == 0) {//Ϊ����
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID=%1").arg(uid);
			sqlDepModel.setQuery(sql);
		}
		QModelIndex indexDepIndex, signIndex;
		//��������
		indexDepIndex = sqlDepModel.index(0, 0);//ģ�������еĵ�0�е�0��(��employee_name)
		signIndex = sqlDepModel.index(0, 1);//��employee_sign
		strWindowName = sqlDepModel.data(signIndex).toString();
		strMsgLabel = sqlDepModel.data(indexDepIndex).toString();

		talkwindow->setWindowName(strWindowName);  //��������
		talkwindowItem->setMsgLabelContent(strMsgLabel);//�����ϵ���ı���ʾ
		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem, uid);
	}
	else {
		//��������б����Ѿ�����������Ϊѡ��
		QListWidgetItem*item=m_talkwindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);//���ô�������Ϊѡ��

		//�����Ҳ����촰��
		m_talkwindowshell->setCurrentWidget(widget);
	}
	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();
}
