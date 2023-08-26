#pragma once
#include <QWidget>
#include<QLabel>
#include<QPushButton>
#include<QMouseEvent>

//ö��ֵ
enum ButtonType {
	MIN_BUTTON = 0,		//��С�����رհ�ť
	MIN_MAX_BUTTON,		//��С������󻯼��رհ�ť
	ONLY_CLOSE_BUTTON	//ֻ�йرհ�ť
};


//�Զ��������
class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();

	void setTitleIcon(const QString& filePath);						//���ñ�����ͼ��(����Ϊ�ļ�·��)
	void setTitleContent(const QString& titleContent);				//���ñ��������� 
	void setTitleWidth(int width);									//���ñ���������
	void setButtonType(ButtonType buttontype);						//���ñ�������ť����


	//���洰�����"ǰ"���ڵ�λ�ü���С(λ�ã���С)
	void saveRestoreInfo(const QPoint& point, const QSize& size);
	//��ȡ�������ǰ���ڵ�λ�ü���С
	void getRestoreInfo(QPoint& point, QSize& size);

private:
	//����������д
	void paintEvent(QPaintEvent *event);			//��ͼ�¼�
	void mouseDoubleClickEvent(QMouseEvent* event);	//���˫���¼�
	void mousePressEvent(QMouseEvent* event);		//������¼�
	void mouseMoveEvent(QMouseEvent* event);		//����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent* event);		//����ɿ��¼�


	void initControl();								//��ʼ���������ؼ�
	void initConnections();							//��ʼ���ź��������
	void loadStyleSheet(const QString& sheetName);	//������ʽ�������

signals:
	//��ť���͵��ź�	
	void signalButtonMinClicked();					//��������ť���ʱ�������ź�
	void signalButtonRestoreClicked();				//��󻯻�ԭ��ť
	void signalButtonMaxClicked();					//��󻯰�ť
	void signalButtonCloseClicked();				//�رհ�ť
private slots:
	//��ť��Ӧ�Ĳ�
	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();
private:
	QLabel* m_pIcon;				//������ͼ��member_ptrIcon;
	QLabel* m_pTitleContent;		//����������
	QPushButton* m_pButtonMin;		//��С����ť
	QPushButton* m_pButtonMax;		//��󻯰�ť
	QPushButton* m_pButtonClose;	//�رհ�ť
	QPushButton* m_pButtonRestore;	//��ԭ��ť


	//��С��ʱ��λ�úʹ�С
	QPoint m_restorePos;
	QSize m_restoreSize;

	//�ƶ����ڵı�����������ڵı������϶�
	bool m_isPressed;        //�Ƿ���
	QPoint m_startMovePos;	 //�Ƿ��ƶ�

	QString m_titleContent;		//����������
	ButtonType m_buttonType;	//���������Ͻǰ�ť����
};
