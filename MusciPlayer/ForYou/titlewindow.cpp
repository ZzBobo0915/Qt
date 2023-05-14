#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QFileIconProvider>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPropertyAnimation>
#include "titlewindow.h"
#include <QMessageBox>

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

TitleWindow::TitleWindow(QWidget *parent)
    : QWidget(parent),
      m_Name("By For You")
{
    //设置窗口的最大最小高度
    setFixedHeight(30);
    createTitleComponent();
    createLayout();
    setUIstyle();
    setWindow(parent);

    connect(m_MinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_MaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_CloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    QObject::connect(m_MessageBox, &QMessageBox::buttonClicked, this, &TitleWindow::onbuttonClicked);
}

void TitleWindow::createTitleComponent()
{
    m_IconLabel = new QLabel(this);
    m_TitleLabel = new QLabel(this);
    m_MinimizeButton = new QPushButton(this);
    m_MaximizeButton = new QPushButton(this);
    m_CloseButton = new QPushButton(this);

    m_MessageBox = new QMessageBox(this);


    //设置图标的大小
    m_IconLabel->setFixedSize(20, 20);
    //设置可填充控件，用于放置软件图标
    m_IconLabel->setScaledContents(true);
    //设置标题布局行为
    m_TitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //设置缩小按钮的大小
    m_MinimizeButton->setFixedSize(32, 30);
    //设置放大按钮的大小
    m_MaximizeButton->setFixedSize(32, 30);
    //设置关闭按钮的大小
    m_CloseButton->setFixedSize(32, 30);

    //设置控件类名
    m_TitleLabel->setObjectName(tr("whiteLabel"));
    m_MinimizeButton->setObjectName(tr("minimizeButton"));
    m_MaximizeButton->setObjectName(tr("maximizeButton"));
    m_CloseButton->setObjectName(tr("closeButton"));

    //设置控件的注释消息
    m_MinimizeButton->setToolTip(tr("最小化"));
    m_MaximizeButton->setToolTip(tr("最大化"));
    m_CloseButton->setToolTip(tr("关闭"));

    //按钮图标设置
    //关闭按钮
    QPixmap close_pix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    m_CloseButton->setIcon(close_pix);
    //最小化按钮
    QPixmap Min_pix = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    m_MinimizeButton->setIcon(Min_pix);
    //最大化按钮
    QPixmap Max_pix = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
    m_MaximizeButton->setIcon(Max_pix);

    m_TitleLabel->setText(m_Name);

    //鼠标停靠在按钮上变成手形
    m_MinimizeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_MaximizeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_CloseButton->setCursor(QCursor(Qt::PointingHandCursor));

    //m_MessageBox->setWindowTitle("关闭提示");
    m_MessageBox->setText("<p align=center>关闭提示   </p>");
    m_MessageBox->setWindowIcon(QIcon("Icon/Sys.png"));
    /*
    m_shadow = new QGraphicsDropShadowEffect(m_MessageBox);
    //设置阴影距离
    m_shadow->setOffset(10, 10);
    //设置阴影颜色
    m_shadow->setColor(QColor("#444444"));
    //设置阴影圆角
    m_shadow->setBlurRadius(30);
    //给嵌套QWidget设置阴影
    m_MessageBox->setGraphicsEffect(m_shadow);
    */
    //设置无边框
    m_MessageBox->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //m_MessageBox->setMinimumSize(200, 100);
    m_MessageMinimize = new QPushButton();
    m_MessageQuit = new QPushButton();
    //设置消息框最小化
    m_MessageMinimize = m_MessageBox->addButton("最小化到托盘", QMessageBox::AcceptRole);
    //设置消息框退出
    m_MessageQuit = m_MessageBox->addButton("退出", QMessageBox::AcceptRole);
}

void TitleWindow::createLayout()
{
    //创建水平布局管理器
    QHBoxLayout *HLayout = new QHBoxLayout(this);
    HLayout->addWidget(m_IconLabel);
    HLayout->addSpacing(5);
    HLayout->addWidget(m_TitleLabel);
    HLayout->addWidget(m_MinimizeButton);
    HLayout->addWidget(m_MaximizeButton);
    HLayout->addWidget(m_CloseButton);
    HLayout->setSpacing(0);
    HLayout->setContentsMargins(5, 0, 5, 0);

    //设置布局管理器
    setLayout(HLayout);
}

TitleWindow::~TitleWindow()
{

}

void TitleWindow::onClicked()
{
    //获取用户点击的按钮
    QPushButton *Button = qobject_cast<QPushButton *>(sender());
    //获取当前窗口指针
    QWidget *Window = this->window();

    //如果小部件是独立的窗口那么返回true不然的话返回false
    if (Window->isWindow())
    {
        //如果点击的按钮是最小化
        if (Button == m_MinimizeButton)
        {
            //以图标的形式显示窗口
            //Window->showMinimized();
            Window->hide();
        }//如果点击的是最大化
        else if (Button == m_MaximizeButton)
        {
            //放大窗口
            Window->isMaximized() ? Window->showNormal() : Window->showMaximized();
        }
        else if (Button == m_CloseButton)
        {
            m_MessageBox->exec();
        }
    }
}

void TitleWindow::onbuttonClicked(QAbstractButton *btnClicked)
{
    if (btnClicked == m_MessageMinimize) {
        QPropertyAnimation *animation = new QPropertyAnimation(this->window(), "windowOpacity");
        animation->setDuration(1000);
        animation->setStartValue(1);
        animation->setEndValue(0);
        animation->start();
        //动画结束时发出finished信号触发槽函数关闭主窗口
        connect(animation, SIGNAL(finished()), this->window(), SLOT(hide()));
    } else if (btnClicked == m_MessageQuit){
        qApp->closeAllWindows();
        qApp->quit();
    }
}

void TitleWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(m_MaximizeButton->isEnabled() && m_MaximizeButton->isVisible())
    {
        //双击以放大窗口
        Q_UNUSED(event);
        //发送放大按钮点击消息
        emit m_MaximizeButton->clicked();
    }
}

//整个窗口随标题栏移动
void TitleWindow::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    //释放对鼠标的捕获消息
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel())
        {
           SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
       event->ignore();
#else
#endif
}

void TitleWindow::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        //如果窗口是最大化
        if (bMaximize)
        {
            m_MaximizeButton->setToolTip(tr("恢复"));
            m_MaximizeButton->setProperty("maximizeProperty", "restore");
        }
        else
        {
            m_MaximizeButton->setProperty("maximizeProperty", "maximize");
            m_MaximizeButton->setToolTip(tr("最大化"));
        }

        m_MaximizeButton->setStyle(QApplication::style());
    }
}

void TitleWindow::setWindow(QWidget*window)
{
    //设置窗口标题图标
    QFileIconProvider fileIP;
    QString filePath = QString("exe");
    QIcon icon = fileIP.icon(QFileInfo(filePath));
    window->setWindowIcon(icon);
}

void TitleWindow::setUIstyle()
{
    //设置按钮为透明风格
    m_MinimizeButton->setStyleSheet("QPushButton{background:transparent;}");
    m_MaximizeButton->setStyleSheet("QPushButton{background:transparent;}");
    m_CloseButton->setStyleSheet("QPushButton{background:transparent;}");
}
void TitleWindow::SetWindowIcon(QString str)
{
    QImage Image;
    Image.load(str);
    QPixmap pixmap = QPixmap::fromImage(Image);
    m_IconLabel->setPixmap(pixmap);
}
