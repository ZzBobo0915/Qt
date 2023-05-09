#include "mylovewindow.h"
MyLoveWindow::MyLoveWindow(QWidget *parent , int width , int height) : QWidget(parent)
{
    m_Increasing = 210;
    setMaximumSize(width + m_Increasing,height);
    setMinimumSize(width + m_Increasing,height);
    setCoverSeting();
}
//该widget的一些默认参数
void MyLoveWindow::setCoverSeting()
{
    m_RestoreCover  =   new QTimer(this);
    m_RestoreCover  ->setInterval(70);

    m_RecoveryRod   =   new QTimer(this);
    m_RecoveryRod   ->setInterval(14);

    connect(m_RestoreCover,&QTimer::timeout,this,&MyLoveWindow::changeCover);
    connect(m_RecoveryRod,&QTimer::timeout,this,&MyLoveWindow::changeRod);
}
void MyLoveWindow::paintEvent(QPaintEvent *event)//绘制事件
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    QColor color(100,100,100);// 黑胶片下部的一层背景色
    //重新定义了该widget的原点。
    painter.translate((this->width() - (this->width() -20 - m_Increasing))/2,(this->height() - (this->height() -20))/2);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawEllipse(-5,-5,this->width() - 10 - m_Increasing,this->height() -10);
    painter.restore();
    painter.save();
    QPixmap pix1;//黑胶片
    pix1.load(m_Love);
    //为了好看移动的原点
    painter.translate((this->width() - (this->width() -20 - m_Increasing))/2,(this->height() - (this->height() -20))/2);
    //让图片的中心作为旋转的中心
    painter.translate((this->width()-20 - m_Increasing)/2,(this->height()-20)/2);
    //顺时针旋转的角度
    painter.rotate(i);
    //使原点复原
    painter.translate(-((this->width()-20 - m_Increasing)/2),-((this->height()-20)/2));
    painter.drawPixmap(0,0,this->width() -20 - m_Increasing,this->height() -20,pix1);
    painter.restore();
    painter.save();

    QPixmap pix;//转动杆
    pix.load("res/huakua.png");
    //为了好看移动了原点
    painter.translate(20,0);
    //让图片的中心作为旋转的中心
    painter.translate(0,0);
    painter.rotate(j);
    painter.translate(0,0);//使原点复原
    //使用图片原始比例缩小7倍
    painter.drawPixmap(0,0,609/7,800/7,pix);
    painter.restore();
}
void MyLoveWindow::changeCover()//转动黑胶片的函数
{
    i+=1;
    if(i>360)
    {
        i=0;
    }
    update();
}
void MyLoveWindow::changeRod()
{
    j-=1;
    if(j<0)
    {
        m_RecoveryRod->stop();
    }
    update();
}
