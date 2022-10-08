#pragma once

#include <QWidget>
#include "ui_TalkWindowItem.h"
#include"CommonUtils.h"
class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = Q_NULLPTR);
	~TalkWindowItem();


	void setHeadPixmap(const QPixmap&pixmap);
	void setMsgLabelContent(const QString&msg);//设置标签内容
	QString getMsgLabelText();//获取你在和谁聊天的文本
private:
	//重写事件
	void enterEvent(QEvent*event);
	void leaveEvent(QEvent*event);
	void resizeEvent(QResizeEvent*event);

private:
	void initControl();

signals:
	void signalCloseClicked();
private:
	Ui::TalkWindowItem ui;
};
