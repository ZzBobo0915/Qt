#ifndef MYLOVEWINDOW_H
#define MYLOVEWINDOW_H

/*
  TODO: 转盘封面，包括黑胶片和胶片杆
  Author: ZzBoAYU
  Date: 2023.05.08
*/

#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <QWidget>
class MyLoveWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MyLoveWindow(QWidget *parent = 0 , int width = 200 , int height = 200);
    int readThe_i()//读取角度
    {
         return i;
    }
    void  writeThe_i(int x)//写入角度
    {
        this->i=x;
    }
    int readThe_j()//读取杆的角度
    {
        return j;
    }
    void writeThe_j(int x)//写入杆的角度
    {
        this->j=x;
    }
    //设置封面
    void setLove(QString Love){m_Love = Love;}
    //黑胶片的timer
    QTimer *        m_RestoreCover;
    //回复杆的timer
    QTimer *        m_RecoveryRod;
private:
    int             m_Increasing;
    QString         m_Love;
    //黑胶片转动的角度，会有定时器来改变它
    int i=0;
    //转动杆的转动角度，会有定时器来改变它
    int j=0;
    //设置音乐封面
    void setCoverSeting();
    //绘制事件，所有的动态实现就在这里面实现的
    void paintEvent(QPaintEvent *event);

public slots:
    //转动黑胶片
    void changeCover();
    //转动回复杆
    void changeRod();
};

#endif // MYLOVEWINDOW_H
