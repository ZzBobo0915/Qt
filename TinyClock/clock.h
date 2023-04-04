#ifndef CLOCK_H
#define CLOCK_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QPen>
#include <QFont>

class Clock : public QWidget
{
    Q_OBJECT

public:
    Clock(QWidget *parent = nullptr);
    ~Clock();
private:
    // 三个数组定义表针的四个顶点，方便后面填充
    static const QPoint hourHand[4];
    static const QPoint minuteHand[4];
    static const QPoint secondHand[4];
    QPen hourHandPen;
    QPen minuteHandPen;
    QPen secondHanPen;
    QFont font;
protected:
    void paintEvent(QPaintEvent*);
    void drawHourHand(QPainter*);
    void drawMinuteHand(QPainter*);
    void drawSecondHand(QPainter*);
    void drawClockDial(QPainter*);

};
#endif // CLOCK_H
