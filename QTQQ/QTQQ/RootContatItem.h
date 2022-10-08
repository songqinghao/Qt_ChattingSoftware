#pragma once

#include <QLabel>
#include<qpropertyanimation.h>
class RootContatItem : public QLabel
{
	Q_OBJECT

		//箭头角度
	Q_PROPERTY(int rotation READ rotation WRITE setRotation)
public:
	RootContatItem(bool hasArrow=true,QWidget *parent=nullptr);
	~RootContatItem();
public:
	void setText(const QString&title);	//设置文本
	void setExpanded(bool expand);		//设置展开
private:
	int rotation();//获取角度
	void setRotation(int rotation);//设置角度

protected:
	void paintEvent(QPaintEvent*event);
private:
	QPropertyAnimation*m_animation;
	QString m_titleText;	//显示文本
	int m_rotation;			//箭头的角度
	bool m_hasArrow;		//是否有箭头
};
