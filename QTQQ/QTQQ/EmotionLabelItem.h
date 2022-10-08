#pragma once
#include"QClickLable.h"
class QMovie;
class EmotionLabelItem : public QClickLable
{
	Q_OBJECT

public:
	EmotionLabelItem(QWidget *parent);
	~EmotionLabelItem();
	void setEmotionName(int emotionName);
private:
	void initControl();
signals:
	void emotionClicked(int emotionNum);
private:
	int m_emotionName;
	QMovie*m_apngMovie;
};
