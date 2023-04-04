#include "clock.h"

const QPoint Clock::hourHand[4] = {
    QPoint(3, 5),
    QPoint(0, 13),
    QPoint(-3, 5),
    QPoint(0, -40)
};
const QPoint Clock::minuteHand[4] = {
    QPoint(3, 5),
    QPoint(0, 16),
    QPoint(-3, 5),
    QPoint(0, -70)
};
const QPoint Clock::secondHand[4] = {
    QPoint(3, 5),
    QPoint(0, 18),
    QPoint(-3, 5),
    QPoint(0, -90)
};

Clock::Clock(QWidget *parent)
    : QWidget(parent)
{
    hourHandPen = QPen(palette().windowText(), 2.0);//设置小时刻度线为粗黑
    minuteHandPen = QPen(palette().windowText(), 1.0);//设置分钟刻度线为灰
    font.setPointSize(10);//字体大小设置为10
    setFont(font);
    QTimer *timer = new QTimer(this);
    timer->start(1000);  //定时器一秒钟
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    setWindowTitle("The Clock");
    resize(360, 360);
    this->setBackgroundRole(QPalette::WindowText);
}

Clock::~Clock()
{
}

void Clock::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int side = qMin(width(), height());  // 绘制的范围，长和宽中的最小值
    painter.setViewport((width() - side) / 2, (height() - side) / 2,
                        side, side);
    painter.setRenderHint(QPainter::Antialiasing);  // 绘制的图像反锯齿
    painter.setWindow(0, 0, 200, 200);  // 设置窗口初始大小
    painter.translate(100, 100);  // 重新设定坐标原点
    drawClockDial(&painter);
    drawHourHand(&painter);
    drawMinuteHand(&painter);
    drawSecondHand(&painter);
    painter.setBrush(Qt::black);  // 区域填充
    painter.drawEllipse(QPoint(0,0),3,3);  // 绘制一个位于圆心的半径为3的圆
}

void Clock::drawHourHand(QPainter *painter)
{
    QTime time = QTime::currentTime();
    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);
    painter->save();
    painter->rotate(30.0*(time.hour()+time.minute()/60.0));  // 旋转变化
    painter->drawConvexPolygon(hourHand,4);  // 绘制时针
    painter->restore();  // 绘制图形后复位坐标系
}

void Clock::drawMinuteHand(QPainter *painter)
{
    QTime time = QTime::currentTime();
    painter->setBrush(Qt::blue);
    painter->setPen(Qt::blue);
    painter->save();
    painter->rotate(6.0*(time.minute()+time.second()/60.0));  // 旋转变化
    painter->drawConvexPolygon(minuteHand,4);  // 绘制分针
    painter->restore();  // 绘制图形后复位坐标系
}

void Clock::drawSecondHand(QPainter *painter)
{
    QTime time = QTime::currentTime();
    painter->setBrush(Qt::red);
    painter->setPen(Qt::red);
    painter->save();  // 保存坐标系，防止坐标系跑偏了
    painter->rotate(6.0*time.second());  // 注意是6.0，不是6
    painter->drawConvexPolygon(secondHand,4);  // 绘制秒针
    painter->restore();  // 绘制图形后复位坐标系
}

void Clock::drawClockDial(QPainter *painter)
{
    // 绘制钟表刻度盘和数字
    for (int i = 1; i <=60; ++i)
    {
        painter->save();
        painter->rotate(6*i);  // 坐标轴旋转6度
        if (i % 5 == 0)
        {
            painter->setPen(hourHandPen);
            painter->drawLine(0, -98, 0, -82);
            painter->drawText(-20, -82, 40, 40,
                              Qt::AlignHCenter | Qt::AlignTop,
                              QString::number(i/5));
        }
        else
        {
            painter->setPen(minuteHandPen);
            painter->drawLine(0, -98, 0, -88);
        }
        painter->restore(); // 绘制图形后复位坐标系
    }
}

