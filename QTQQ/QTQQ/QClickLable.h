#pragma once

#include <QLabel>

class QClickLable : public QLabel
{
	Q_OBJECT

public:
	QClickLable(QWidget *parent);
	~QClickLable();
protected:
	void mousePressEvent(QMouseEvent*event);
signals:
	void clicked();

};
