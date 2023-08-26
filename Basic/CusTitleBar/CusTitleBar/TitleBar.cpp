#include "TitleBar.h"
#include<QHBoxLayout>
#include<QPainter>
#include<QMouseEvent>
#include<QFile>
#include<QDebug>
#include "NotifyManager.h"
//Ԥ���룬Ԥ����
#define BUTTON_HEIGHT 27	//��ť�߶�
#define BUTTON_WIDTH  27	//��ť���
#define TITLE_HEIGHT  27	//�������߶�




//�Զ��������
TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
	, m_isPressed(false)
	, m_buttonType(MIN_MAX_BUTTON)
{
	initControl();
	initConnections();
	loadStyleSheet("Title");	//��������ʽ��

}

TitleBar::~TitleBar()
{
}

//��ʼ���ؼ�
void TitleBar::initControl() {
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonMin = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);

	//��ť���ù̶���С
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	//���ö�����
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");

	//���ò���
	QHBoxLayout* mylayout = new QHBoxLayout(this);//ˮƽ����
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);
	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);

	mylayout->setContentsMargins(5, 0, 0, 0);	//���ò��ּ�϶���ʹ��ڱ߿�
	mylayout->setSpacing(0);					//���ò�����ļ�϶ 
	//�������ݵĳߴ���ԣ��ͺ�������������ʹ��Ӧ��һ����ˮƽ������չ����ֱ����̶���
	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setFixedHeight(TITLE_HEIGHT);			//���ñ������߶�

	setWindowFlags(Qt::FramelessWindowHint);//����������ޱ߿�
}


//��ʼ���ź���۵�����
void TitleBar::initConnections() {
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	
}

//���ñ�����ͼ��
void TitleBar::setTitleIcon(const QString& filePath) {

	QPixmap titleIcon(filePath);
	m_pIcon->setFixedSize(titleIcon.size());//����ͼ���С
	m_pIcon->setPixmap(titleIcon);
}

//���ñ���������
void TitleBar::setTitleContent(const QString& titleContent) {
	m_pTitleContent->setText(titleContent);//���ñ�����������
	m_titleContent = titleContent;
}

//���ñ������ĳ���
void TitleBar::setTitleWidth(int width) {
	setFixedWidth(width);
}

void TitleBar::setButtonType(ButtonType buttontype) {
	m_buttonType = buttontype;
	switch (buttontype) {
	case MIN_BUTTON://ֻ����С����ť�͹رհ�ť	
	{
		m_pButtonRestore->setVisible(false);//�Ƿ�ɼ�
		m_pButtonMax->setVisible(false);
	}
	break;

	case MIN_MAX_BUTTON://�������С���йرհ�ť
	{
		m_pButtonRestore->setVisible(false);
	}
	break;

	case ONLY_CLOSE_BUTTON://ֻ�йرհ�ť
	{
		m_pButtonRestore->setVisible(false);
		m_pButtonMax->setVisible(false);
		m_pButtonMin->setVisible(false);
	}
	break;

	default:
		break;
	}
}


//���洰�����ǰ���ڵ�λ�ü���С
void TitleBar::saveRestoreInfo(const QPoint& point, const QSize& size) {
	m_restorePos = point;
	m_restoreSize = size;
}

//��ȡ�������ǰ���ڵ�λ�ü���С
void TitleBar::getRestoreInfo(QPoint& point, QSize& size) {
	point = m_restorePos;
	size = m_restoreSize;
}

//���Ʊ�����
void TitleBar::paintEvent(QPaintEvent* event) {
	//���ñ���ɫ
	QPainter painter(this);
	QPainterPath pathBack;					//��ͼ·��
	pathBack.setFillRule(Qt::WindingFill);	//����������
	//xб�ʺ�yб��Ϊ3
	pathBack.addRoundedRect(QRect(0, 0, width(), height()), 3, 3);  //���Բ�Ǿ��ε���ͼ·��
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);   //����ת������ƽ��������ת������Ⱦ��

	//��������󻯻�ԭ�󣬴��ڳ��ȸı䣬��������Ӧ�����ı�
	//parentWidth()���ظ�����
	//����������Ŀ�Ⱥ͸������Ŀ�Ȳ�һ��
	if (width() != parentWidget()->width()) {
		setFixedWidth(parentWidget()->width());
	}
	QWidget::paintEvent(event);

}


//˫����Ӧ�¼�����Ҫʵ��˫��������������󻯺���С���Ĳ���
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event) {

	//���������󻯺���С���İ�ťʱ����Ч
	if (m_buttonType == MIN_MAX_BUTTON) {
		//�����󻯼��ɼ��Ļ��ͷŴ�˵����С���ľͿ��ԷŴ�
		if (m_pButtonMax->isVisible()) {
			onButtonMaxClicked();
		}
		else {
			//���ɼ��Ļ����ǻ�ԭ��������С��
			onButtonRestoreClicked();
		}
	}
	return QWidget::mouseDoubleClickEvent(event);
}

//�ƶ������¼���ͨ����갴������ƶ�����ͷ�ʵ���϶�������ʵ���ƶ�����Ч����
void TitleBar::mousePressEvent(QMouseEvent*event) {
	if (m_buttonType == MIN_MAX_BUTTON) {
		//�ڴ������ʱ��ֹ�϶�
		if (m_pButtonMax->isVisible()) {
			m_isPressed = true;
			m_startMovePos = event->globalPos();  //globalPos();�����¼�����ʱ����ȫ��λ��
		}
	}
	else {
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);
}

//����ƶ��¼�
void TitleBar::mouseMoveEvent(QMouseEvent* event) {
	if (m_isPressed) {
		//���ȫ�����������ԭ��ƫ���˶���
		QPoint movePoint = event->globalPos() - m_startMovePos;//����ֵ�����ƶ�����
		//��ȡ��������ȫ������
		QPoint widgetPos = parentWidget()->pos();
		m_startMovePos = event->globalPos();
		parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());//�����������ƶ�
		//parentWidget()->move(movePoint.x(), movePoint.y());//�����������ƶ�
		//qDebug() << "parentWidget()->pos();" << widgetPos.x() << " " << widgetPos.y();
		//qDebug() << "global;" << event->globalPos().x() << " " << event->globalPos().y();
	}
	return QWidget::mouseMoveEvent(event);
}

//����ͷ��¼�
void TitleBar::mouseReleaseEvent(QMouseEvent* event) {
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

//������ʽ��
void TitleBar::loadStyleSheet(const QString& sheetName) {
	QFile file(":/Resources/QSS/" + sheetName + ".css");	//sheetName������Ǳ�ʾtitlebar
	if (file.open(QFile::ReadOnly)) {//ֻ��
		QString styleSheet = this->styleSheet();		//��ǰ��ʽ��
		//QLatin1Stringֻ�����˵�ַ�ͳ��ȣ���QString����С
		styleSheet += QLatin1String(file.readAll());	//���ļ������ݺ����е���ʽ����кϲ�
		setStyleSheet(styleSheet);						//������ʽ��
	}
}


void TitleBar::onButtonMinClicked() {
	emit signalButtonMinClicked();
}

void TitleBar::onButtonRestoreClicked() {
	m_pButtonRestore->setVisible(false);  //�ָ���ť���ɼ�
	m_pButtonMax->setVisible(true);		  //��󻯰�ť�ɼ�
	emit signalButtonRestoreClicked();
}

void TitleBar::onButtonCloseClicked() {
	emit signalButtonCloseClicked();
}


void TitleBar::onButtonMaxClicked() {
	m_pButtonMax->setVisible(false);	//�Ŵ�����ɼ�
	m_pButtonRestore->setVisible(true);	//��ԭ���ɼ�
	emit signalButtonMaxClicked();
}

