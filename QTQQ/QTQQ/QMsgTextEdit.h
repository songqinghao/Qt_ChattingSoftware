#pragma once

#include <QTextEdit>

class QMsgTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	QMsgTextEdit(QWidget *parent=nullptr);
	~QMsgTextEdit();
private slots:
	void onEmotionImageFrameChange(int frame);//�����ı�ʱ��Ӧ�Ĳۺ���
public:
	void addEmotionUrl(int emotionNum);//��ӱ��飬�������ID
	void deleteAllEmotionImage();//�����������Ϣʱ��������еı��飬Ҫ��յ���
private:
	QList<QString>m_listEmotionUrl;//���ڱ���ѡ�еı���
	QMap<QMovie*, QString>m_emotionMap;//�����QString֮���ӳ��
};
