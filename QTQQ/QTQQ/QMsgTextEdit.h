#pragma once

#include <QTextEdit>

class QMsgTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	QMsgTextEdit(QWidget *parent=nullptr);
	~QMsgTextEdit();
private slots:
	void onEmotionImageFrameChange(int frame);//动画改变时相应的槽函数
public:
	void addEmotionUrl(int emotionNum);//添加表情，传入表情ID
	void deleteAllEmotionImage();//当点击发送信息时，输入框中的表情，要清空掉，
private:
	QList<QString>m_listEmotionUrl;//用于保存选中的表情
	QMap<QMovie*, QString>m_emotionMap;//表情和QString之间的映射
};
