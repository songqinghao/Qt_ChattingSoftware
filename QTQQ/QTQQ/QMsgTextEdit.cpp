#include "QMsgTextEdit.h"
#include<QMovie>
#include<QUrl>
QMsgTextEdit::QMsgTextEdit(QWidget *parent)
	: QTextEdit(parent)
{
}

QMsgTextEdit::~QMsgTextEdit()
{
	deleteAllEmotionImage();
}

void QMsgTextEdit::addEmotionUrl(int emotionNum)
{
	const QString& imageName = QString("qrc:/Resources/MainWindow/emotion/%1.png").arg(emotionNum);
	const QString&flagName = QString("%1").arg(imageName);
	insertHtml(QString("<img src='%1'/>").arg(flagName));
	if (m_listEmotionUrl.contains(imageName)) {//����Ѿ�����
		return;
	}
	else {
		m_listEmotionUrl.append(imageName);
	}
	QMovie*apngMovie = new QMovie(imageName, "apng", this);
	m_emotionMap.insert(apngMovie, flagName);
	//����֡�ı�ʱ(��ͼ������)�����ź�
	connect(apngMovie, SIGNAL(frameChanged(int)), this, SLOT(onEmotionImageFrameChange(int)));
	
	apngMovie->start();//������ͼ
	updateGeometry();  //��ͼ�ı䣬�ı��༭�����η����ı�
	
}


void QMsgTextEdit::deleteAllEmotionImage()
{
	for (auto itor = m_emotionMap.constBegin(); itor != m_emotionMap.constEnd(); itor++) {
		delete itor.key();
	}

	m_emotionMap.clear();
}
void QMsgTextEdit::onEmotionImageFrameChange(int frame) 
{
	QMovie*movie = qobject_cast<QMovie*>(sender());
	document()->addResource(QTextDocument::ImageResource,
		QUrl(m_emotionMap.value(movie)), movie->currentPixmap());//�����Դ->ͼƬ��Դ+��Ӧurl+ģʽ�ǵ�ǰ��ͼƬ


}
