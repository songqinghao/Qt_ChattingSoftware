#include "RootContatItem.h"
#include<QPainter>

RootContatItem::RootContatItem(bool hasArrow, QWidget * parent):QLabel(parent),m_rotation(0),m_hasArrow(hasArrow)
{
	setFixedHeight(32);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	//属性动画进行初始化
	m_animation = new QPropertyAnimation(this, "rotation");
	//设置动画时间
	m_animation->setDuration(50);//设置单次动画时长50毫秒
	m_animation->setEasingCurve(QEasingCurve::InQuad);//动画设置变化曲线类型（动画缓和类型）x^2


}

RootContatItem::~RootContatItem()
{
}

void RootContatItem::setText(const QString & title)
{
	m_titleText = title;
	update();//更新
}

void RootContatItem::setExpanded(bool expand)
{
	if (expand) 
	{
		m_animation->setEndValue(90);//设置属性动画结束值,向下旋转90度
	}
	else
	{
		m_animation ->setEndValue(0);
	}
	m_animation->start();
}

int RootContatItem::rotation()
{
	return m_rotation;
}

void RootContatItem::setRotation(int rotation)
{
	m_rotation = rotation;
	update();//重绘
}

void RootContatItem::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::TextAntialiasing,true);//文本防止此设置为true
	QFont font;
	font.setPointSize(10);
	painter.setFont(font);
	painter.drawText(24, 0, width() - 24, height(), Qt::AlignLeft | Qt::AlignVCenter, m_titleText);//左对齐，上下居中对齐
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);//图片保真
	painter.save();//保存
	if (m_hasArrow) {
		QPixmap pixmap;
		pixmap.load(":/Resources/MainWindow/arrow.png");
		
		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);//设置为透明

		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform,true);//图片保真
		p.translate(pixmap.width() / 2, pixmap.height() / 2); //坐标系偏移（x方向偏移，y方向反向偏移）
		//坐标系进行旋转（顺时针旋转）
		p.rotate(m_rotation);
		//图片绘制
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);
		
		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);
		painter.restore();//恢复画家设置


	}

	QLabel::paintEvent(event);
}
