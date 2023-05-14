#ifndef TITLEWINDOW_H
#define TITLEWINDOW_H

/*
  TODO: 标题栏
  Author: ZzBoAYU
  Date: 2023.05.06
*/

#include <QWidget>
#include <QStyle>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
class QLabel;
class QPushButton;
class TitleWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TitleWindow(QWidget *parent);
    ~TitleWindow();
private slots:
    // 进行最小化、最大化/还原、关闭操作
    void onClicked();
    // 消息框的操作
    void onbuttonClicked(QAbstractButton *btnClicked);

    //重载QWidget函数
protected:
    // 双击标题栏进行界面的最大化/还原
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    // 进行鼠界面的拖动
    virtual void mousePressEvent(QMouseEvent *event);
private:
    //标题栏图标
    QLabel *m_IconLabel;
    //标题栏名称
    QString m_Name;
public:
    //标题名称
    QLabel *m_TitleLabel;
    //标题栏最小按钮
    QPushButton *m_MinimizeButton;
    //标题栏最大按钮
    QPushButton *m_MaximizeButton;
    //标题栏关闭按钮
    QPushButton *m_CloseButton;
    //关闭消息框
    QMessageBox *m_MessageBox;
    //消息框最小化按钮
    QPushButton *m_MessageMinimize;
    //消息框关闭按钮
    QPushButton *m_MessageQuit;
    //消息框阴影
    QGraphicsDropShadowEffect* m_shadow;
private:
    //创建标题栏控件
    void createTitleComponent();
    //创建布局管理器
    void createLayout();
    // 最大化/还原刷新窗口
    void updateMaximize();
    //设置窗口
    void setWindow(QWidget*window);
    //设置UI风格
    void setUIstyle();
public:
    //设置窗口图标
    void SetWindowIcon(QString str);
};

#endif // TITLEWINDOW_H
