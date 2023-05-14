#ifndef BASIWINDOW_H_
#define BASIWINDOW_H_

/*
  TODO: 主窗口
  Author: ZzBoAYU
  Date: 2023.05.07
*/

#include <QMainWindow>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>  // 提供类似投影的效果
#include "ForYou/titlewindow.h"
#include "ForYou/gaussianblur.h"
#include "ForYou/cachethread.h"

#include <QListWidget>

class Basiwindow :public QWidget
{
    Q_OBJECT
public:
    explicit Basiwindow(QWidget *parent = nullptr,int width = 400,int height = 300);
    ~Basiwindow();
private:
    //阴影窗口
    QWidget* m_EffectWindow;
    //窗口阴影实例
    QGraphicsDropShadowEffect * m_shadow;
    //窗口主要布局
    QVBoxLayout*m_VmainLayout;
    //自定义背景
    bool BackgroundState;
    //自定义背景路径
    QString BackgroundPath;
protected:
    //设置窗口圆角;
    virtual void paintEvent(QPaintEvent *event);
private:
    //设置窗口阴影
    void setWindowEffect(int w,int h);
    //创建标题栏
    void createTitlew();
    //高斯背景图
    QImage          m_blurImage;
public:
    //标题栏
    TitleWindow * m_Titlew;
    //获取窗口布局器
    inline QVBoxLayout* getWindowLayout()
    {return m_VmainLayout;}
    //切换背景
    void BSwitchBackground(QString path);

    QListWidget* list;
};

#endif // DEMOWINDOW_H
