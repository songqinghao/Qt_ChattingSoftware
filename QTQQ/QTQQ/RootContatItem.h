#pragma once

#include <QLabel>
#include<qpropertyanimation.h>
class RootContatItem : public QLabel
{
	Q_OBJECT

		//��ͷ�Ƕ�
	Q_PROPERTY(int rotation READ rotation WRITE setRotation)
public:
	RootContatItem(bool hasArrow=true,QWidget *parent=nullptr);
	~RootContatItem();
public:
	void setText(const QString&title);	//�����ı�
	void setExpanded(bool expand);		//����չ��
private:
	int rotation();//��ȡ�Ƕ�
	void setRotation(int rotation);//���ýǶ�

protected:
	void paintEvent(QPaintEvent*event);
private:
	QPropertyAnimation*m_animation;
	QString m_titleText;	//��ʾ�ı�
	int m_rotation;			//��ͷ�ĽǶ�
	bool m_hasArrow;		//�Ƿ��м�ͷ
};
