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
	m_apngMovie = new QMovie(imageName, "apng", this);//��ͼ·��+��ʽ+����
	m_apngMovie->start();
	m_apngMovie->stop();
	setMovie(m_apngMovie);
}

void EmotionLabelItem::initControl()
{
	setAlignment(Qt::AlignCenter);//���ж���
	setObjectName("emotionLabelItem");
	setFixedSize(32, 32);
}
