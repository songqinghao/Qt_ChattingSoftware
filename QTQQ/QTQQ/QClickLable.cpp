#include "QClickLable.h"
#include<QMouseEvent>
QClickLable::QClickLable(QWidget *parent)
	: QLabel(parent)
{
}

QClickLable::~QClickLable()
{
}

void QClickLable::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		emit clicked();
	}
}
