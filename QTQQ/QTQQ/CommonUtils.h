#pragma once
//用于常见的一些操作
#include<QPixmap>
#include<QSize>
#include<QProxyStyle>

//改变默认的部件风格
class CustomProxyStyle:public QProxyStyle {
public:
	CustomProxyStyle(QObject* parent) {
		setParent(parent);
	}
	//重写drawPrimitive
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget = 0)const {
		if (PE_FrameFocusRect == element) {
			//去掉windows中部件默认的边框或者虚线框，部件获取焦点时直接返回，不进行绘制
			return;
		}
		else {
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}

};



class CommonUtils
{
public:
	CommonUtils();
public:
	//获取圆头像
	/*
	*  mask：空的圆头像
	*  src： 图片
	*  masksize: 大小
	*/
	static QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize = QSize(0, 0));
	
	//加载样式表
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);
	
	//设置默认皮肤颜色
	static void setDefaultSkinColor(const QColor& color);

	//获取默认皮肤颜色
	static QColor getDefaultSkinColor();
	
};

