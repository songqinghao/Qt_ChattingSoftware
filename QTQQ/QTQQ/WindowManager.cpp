#include "WindowManager.h"
#include"TalkWindow.h"
#include"TalkWindowShell.h"
#include"TalkWindowItem.h"
#include<QSqlQueryModel>
//单例模式，创建全局静态对象

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
//添加窗口名
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
	if (m_talkwindowshell == nullptr){//看父窗口有没有创建出来

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
		
		//用完就又重新设置为空""
		m_strCreateingTalkId = "";
		
		//判断是群聊还是独聊
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID=%1").arg(uid);
		//设置查询
		sqlDepModel.setQuery(strSql);
		int rows = sqlDepModel.rowCount();//看查询结果有几行
		QString strWindowName, strMsgLabel;
		if (rows == 0) {//为单聊
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID=%1").arg(uid);
			sqlDepModel.setQuery(sql);
		}
		QModelIndex indexDepIndex, signIndex;
		//部门索引
		indexDepIndex = sqlDepModel.index(0, 0);//模型索引中的第0行第0列(即employee_name)
		signIndex = sqlDepModel.index(0, 1);//即employee_sign
		strWindowName = sqlDepModel.data(signIndex).toString();
		strMsgLabel = sqlDepModel.data(indexDepIndex).toString();

		talkwindow->setWindowName(strWindowName);  //窗口名称
		talkwindowItem->setMsgLabelContent(strMsgLabel);//左侧联系人文本显示
		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem, uid);
	}
	else {
		//左侧聊天列表中已经存在则设置为选中
		QListWidgetItem*item=m_talkwindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);//将该窗口设置为选中

		//设置右侧聊天窗口
		m_talkwindowshell->setCurrentWidget(widget);
	}
	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();
}
