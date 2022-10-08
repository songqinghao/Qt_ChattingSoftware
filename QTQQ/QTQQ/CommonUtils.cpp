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
		//圆头像进行圆滑的缩放
		mask = mask.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	QImage resultImage(masksize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(),Qt::transparent);//填充矩形,透明
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(masksize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();
	return QPixmap::fromImage(resultImage);
}


void CommonUtils::loadStyleSheet(QWidget* widget, const QString& sheetName) {
	QFile file(":/Resources/QSS/" + sheetName + ".css");//:/是打开一个资源文件
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		widget->setStyleSheet("");							 //样式设置为空
		QString qsstyleSheet = QLatin1String(file.readAll());//全部读取到qsstyleSheet中去
		widget->setStyleSheet(qsstyleSheet);				 //部件进行样式设置
	}
	file.close();
}

//重写设置默认皮肤颜色
void CommonUtils::setDefaultSkinColor(const QColor& color) {
	//将颜色配置文件进行重写
	//获取路径
	const QString &&path = QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	//操作配置文件（对配置文件进行读写操作IniFormat）
	QSettings settings(path, QSettings::IniFormat);
	//【结/key,要设置的值】
	settings.setValue("DefaultSkin/red", color.red());
	settings.setValue("DefaultSkin/green", color.green());
	settings.setValue("DefaultSkin/blue", color.blue());
}

//获取默认颜色值
QColor CommonUtils::getDefaultSkinColor() {
	QColor color;
	const QString&& path= QApplication::applicationDirPath() + "/" + QString("tradeprintinfo.ini");
	//不存在文件
	if (!QFile::exists(path)) {//如果不存在该路径就设置默认颜色
		setDefaultSkinColor(QColor(22, 154, 218));
	}
	//存在文件进行文件配置操作
	QSettings settings(path, QSettings::IniFormat);
	QSettings getting(path, QSettings::IniFormat);
	color.setRed(settings.value("DefaultSkin/red").toInt());
	color.setGreen(settings.value("DefaultSkin/green").toInt());
	color.setBlue(settings.value("DefaultSkin/blue").toInt());
	return color;
}