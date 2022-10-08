#include "EmotionLabelItem.h"
#include<QMovie>
EmotionLabelItem::EmotionLabelItem(QWidget *parent)
	: QClickLable(parent)
{
	initControl();
	connect(this, &QClickLable::clicked, [this] {
		emit emotionClicked(m_emotionName);
	});
}

EmotionLabelItem::~EmotionLabelItem()
{
}

void EmotionLabelItem::setEmotionName(int emotionName)
{
	m_emotionName = emotionName;
	QString imageName = QString(":/Resources/MainWindow/emotion/%1.png").arg(emotionName);
	m_apngMovie = new QMovie(imageName, "apng", this);//动图路径+格式+父类
	m_apngMovie->start();
	m_apngMovie->stop();
	setMovie(m_apngMovie);
}

void EmotionLabelItem::initControl()
{
	setAlignment(Qt::AlignCenter);//居中对齐
	setObjectName("emotionLabelItem");
	setFixedSize(32, 32);
}
