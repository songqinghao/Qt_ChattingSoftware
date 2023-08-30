#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFrame>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
    //创建边框
    void createCtrlFrame();
    //创建右边内容区域
    void createContentFrame();


    //给组合框添加颜色列表
    void fillColorList(QComboBox*comboBox);
public slots:
    void ShowWindow(int index);
    void ShowWindowText(int index);
    void ShowButton(int index);
    void ShowButtonText(int index);
    void ShowBase(int index);

private:
    QFrame* m_ctrlFrame;
    QLabel* m_windowLabel;
    QComboBox* m_windowComboBox;
    QLabel* m_windowTextLabel;
    QComboBox* m_windowTextComboBox;
    QLabel* m_buttonLabel;
    QComboBox* m_buttonComboBox;
    QLabel* m_buttonTextLabel;
    QComboBox* m_buttonTextComboBox;
    QLabel* m_baseLabel;
    QComboBox* m_baseComboBox;
    QFrame* m_contentFrame;
    QLabel* m_label1;
    QLabel* m_label2;
    QComboBox* m_comboBox1;
    QLineEdit* m_lineEdit;
    QTextEdit* m_textEdit;
    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;
};

#endif // DIALOG_H
