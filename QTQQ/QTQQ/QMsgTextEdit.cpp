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
	if (m_listEmotionUrl.contains(imageName)) {//如果已经存在
		return;
	}
	else {
		m_listEmotionUrl.append(imageName);
	}
	QMovie*apngMovie = new QMovie(imageName, "apng", this);
	m_emotionMap.insert(apngMovie, flagName);
	//数据帧改变时(动图动起来)发射信号
	connect(apngMovie, SIGNAL(frameChanged(int)), this, SLOT(onEmotionImageFrameChange(int)));
	
	apngMovie->start();//启动动图
	updateGeometry();  //动图改变，文本编辑器几何发生改变
	
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
		QUrl(m_emotionMap.value(movie)), movie->currentPixmap());//添加资源->图片资源+对应url+模式是当前的图片


}
