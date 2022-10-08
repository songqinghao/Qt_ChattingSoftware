#pragma once

#include <QDialog>
#include"TitleBar.h"
class BasicWindow : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent=nullptr);
	virtual ~BasicWindow();
public:
	//加载样式表（窗体的外观风格）
	void loadStyleSheet(const QString& sheetName);

	//获取圆头像
	QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize = QSize(0, 0));
private:
	void initBackGroundColor();//初始化背景
protected:
	void paintEvent(QPaintEvent* enent);	   //绘制事件
	void mousePressEvent(QMouseEvent* event);  //鼠标按压事件
	void mouseMoveEvent(QMouseEvent* event);   //鼠标移动事件
	void mouseReleaseEvent(QMouseEvent* event);//鼠标松开事件

protected:
	//初始化标题栏
	void initTitleBar(ButtonType buttontype = MIN_BUTTON);
	//设置标题栏标题
	void setTitleBarTitle(const QString&title, const QString& icon = "");
public slots:
	void onShowClose(bool);
	void onShowMin(bool);
	void onShowHide(bool);	//窗口隐藏
	void onShowNormal(bool);//窗体的状态
	void onShowQuit(bool);
	void onSignalSkinChanged(const QColor& color);//窗体皮肤改变所触发的槽方法

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();
protected:
	QPoint m_mousePoint;		//鼠标位置
	bool m_mousePressed;		//鼠标是否按下
	QColor m_colorBackGround;	//背景色
	QString m_styleName;		//样式文件名称
	TitleBar* _titleBar;		//标题栏
};
