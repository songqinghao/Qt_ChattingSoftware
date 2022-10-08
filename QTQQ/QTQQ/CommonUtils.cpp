#include "CommonUtils.h"
#include<QFile>
#include<QPainter>
#include<QWidget>
#include<QApplication>
#include<QSettings>
CommonUtils::CommonUtils() {


}

QPixmap CommonUtils::getRoundImage(const QPixmap &src, QPixmap& mask, QSize masksize) {
	if (masksize == QSize(0, 0)) {
		masksize = mask.size();
	}
	else {
		//Բͷ�����Բ��������
		mask = mask.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	QImage resultImage(masksize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(),Qt::transparent);//������,͸��
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();
	return QPixmap::fromImage(resultImage);
}


void CommonUtils::loadStyleSheet(QWidget* widget, const QString& sheetName) {
	QFile file(":/Resources/QSS/" + sheetName + ".css");//:/�Ǵ�һ����Դ�ļ�
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		widget->setStyleSheet("");							 //��ʽ����Ϊ��
		QString qsstyleSheet = QLatin1String(file.readAll());//ȫ����ȡ��qsstyleSheet��ȥ
		widget->setStyleSheet(qsstyleSheet);				 //����������ʽ����
	}
	file.close();
}

//��д����Ĭ��Ƥ����ɫ
void CommonUtils::setDefaultSkinColor(const QColor& color) {
	//����ɫ�����ļ�������д
	//��ȡ·��
	const QString &&path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	//���������ļ����������ļ����ж�д����IniFormat��
	QSettings settings(path, QSettings::IniFormat);
	//����/key,Ҫ���õ�ֵ��
	settings.setValue("DefaultSkin/red", color.red());
	settings.setValue("DefaultSkin/green", color.green());
	settings.setValue("DefaultSkin/blue", color.blue());
}

//��ȡĬ����ɫֵ
QColor CommonUtils::getDefaultSkinColor() {
	QColor color;
	const QString&& path= QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	//�������ļ�
	if (!QFile::exists(path)) {//��������ڸ�·��������Ĭ����ɫ
		setDefaultSkinColor(QColor(22, 154, 218));
	}
	//�����ļ������ļ����ò���
	QSettings settings(path, QSettings::IniFormat);
	QSettings getting(path, QSettings::IniFormat);
	color.setRed(settings.value("DefaultSkin/red").toInt());
	color.setGreen(settings.value("DefaultSkin/green").toInt());
	color.setBlue(settings.value("DefaultSkin/blue").toInt());
	return color;
}