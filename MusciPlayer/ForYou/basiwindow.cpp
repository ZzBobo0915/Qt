#include "ForYou/basiwindow.h"
#include <QVBoxLayout>
#include <QPainter>
//#include <QtMath>
#include "ForYou/gaussianblur.h"

Basiwindow::Basiwindow(QWidget *parent,int width,int height)
    :QWidget(parent)
{
    BackgroundState  = false;
    createTitlew();
    setWindowEffect(width,height);
}

void Basiwindow::createTitlew()
{
    //创建窗口标题栏窗口
    m_Titlew = new TitleWindow(this);
    //窗口垂直布局
    m_VmainLayout = new QVBoxLayout();
    //将标题栏添加进窗口当中
    m_VmainLayout->addWidget(m_Titlew);
    //设置要在布局周围使用的左、上、右和下边距。
    m_VmainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_VmainLayout);
}
void Basiwindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);  // QUNUSED用来告诉编译器其中的变量没有使用，不需要提示警告
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);  // 线性插值
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.save();
    QRect rect = this->rect();
    painter.setPen(Qt::transparent);
    if(BackgroundState == false)
    {
        painter.setBrush(QBrush(QColor(230,230,230)));
    }
    else {
        QPixmap blurImage;
        // 如果读取到图片
        if(blurImage.load(BackgroundPath))
        {
            m_blurImage     =   blurImage.toImage();

            QBrush brush;
            brush.setTextureImage(QImage(m_blurImage));
            painter.setBrush(brush);
            //painter.drawImage(-150,-150,m_blurImage,0,0,this->width()*2,this->height()*2);
        }
        // 没有读取到则默认黑色
        else {
            painter.setBrush(QBrush(QColor(230,230,230)));
        }
    }
    rect.setWidth(rect.width());
    rect.setHeight(rect.height());
    painter.drawRoundedRect(rect, 8, 8);  // 绘制带有圆角的矩形，8为圆度，0时为直角，99时为最大圆度
    QWidget::paintEvent(event);           // 转发到基类的事件处理
}
void Basiwindow::setWindowEffect(int w,int h)
{
    m_EffectWindow = new QWidget();

    m_EffectWindow->resize(w,h);
    //窗口垂直布局
    QVBoxLayout *pLayout = new QVBoxLayout();

    pLayout->addWidget(this);
    m_EffectWindow->setLayout(pLayout);

    //设置透明
    m_EffectWindow->setAttribute(Qt::WA_TranslucentBackground, true);
    //设置无边框
    m_EffectWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //实例阴影shadow
    m_shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    m_shadow->setOffset(0, 0);
    //设置阴影颜色
    m_shadow->setColor(QColor("#444444"));
    //设置阴影圆角
    m_shadow->setBlurRadius(30);
    //给嵌套QWidget设置阴影
    m_EffectWindow->setGraphicsEffect(m_shadow);
    //给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)
    pLayout->setMargin(24);

    m_EffectWindow->show();

    return;
}

Basiwindow::~Basiwindow()
{

}

void Basiwindow::BSwitchBackground(QString path)
{
    BackgroundState = true;
    BackgroundPath = path;
    this->update();
}

