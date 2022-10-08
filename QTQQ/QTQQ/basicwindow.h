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
	//������ʽ���������۷��
	void loadStyleSheet(const QString& sheetName);

	//��ȡԲͷ��
	QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize = QSize(0, 0));
private:
	void initBackGroundColor();//��ʼ������
protected:
	void paintEvent(QPaintEvent* enent);	   //�����¼�
	void mousePressEvent(QMouseEvent* event);  //��갴ѹ�¼�
	void mouseMoveEvent(QMouseEvent* event);   //����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent* event);//����ɿ��¼�

protected:
	//��ʼ��������
	void initTitleBar(ButtonType buttontype = MIN_BUTTON);
	//���ñ���������
	void setTitleBarTitle(const QString&title, const QString& icon = "");
public slots:
	void onShowClose(bool);
	void onShowMin(bool);
	void onShowHide(bool);	//��������
	void onShowNormal(bool);//�����״̬
	void onShowQuit(bool);
	void onSignalSkinChanged(const QColor& color);//����Ƥ���ı��������Ĳ۷���

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();
protected:
	QPoint m_mousePoint;		//���λ��
	bool m_mousePressed;		//����Ƿ���
	QColor m_colorBackGround;	//����ɫ
	QString m_styleName;		//��ʽ�ļ�����
	TitleBar* _titleBar;		//������
};
