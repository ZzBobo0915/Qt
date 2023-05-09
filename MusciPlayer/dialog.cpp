#include "dialog.h"
#include <QPropertyAnimation>
#include <QEvent>
#include <QMouseEvent>
#include <QDomDocument>
#include <QApplication>
#include <QDateTime>
#include <QAction>
#include <QMenu>
#include <io.h>
#include <QPainter>
#include "mylovewindow.h"
#include "ForYou/cachethread.h"

Dialog::Dialog(QWidget *parent) :
    Basiwindow(parent,450,650)
{
    m_UpDataThread = new MyThread;
    createwindow();
    setUIStyleSheet();
    Datainitialization();
    createTrayIcon();
    createCachethread();
    QObject::connect(m_Titlew,SIGNAL(CloseMainWindow()),this,SLOT(Storedata()));
    QObject::connect(m_PlayModButton,&QPushButton::clicked,this,&Dialog::On_PlayMod);
    QObject::connect(m_PlayButton,&QPushButton::clicked,this,&Dialog::On_playBtn);
    QObject::connect(m_FastForwardButton,&QPushButton::clicked,this,&Dialog::On_Fast_ForwardBtn);
    QObject::connect(m_RewindButton,&QPushButton::clicked,this,&Dialog::On_RewindBtn);
    QObject::connect(m_MusicSlider,&QSlider::sliderMoved,this,&Dialog::On_sliderMoved);
    QObject::connect(m_VoiceSlider,&QSlider::sliderMoved,this,&Dialog::On_VoiceMoved);
    QObject::connect(m_UpDataThread,&MyThread::intit,this,&Dialog::intitD);
    QObject::connect(m_LyricsButton,&QPushButton::clicked,this,&Dialog::On_Lyrics_Switch);
    QObject::connect(m_UpDataThread,&MyThread::SwitchBackground,this,&Dialog::SwitchBackground);
}

Dialog::~Dialog()
{

}

//----------------------------------------------------------------------UI----------------------------------------------------------------------//

void Dialog::createwindow()
{
    //设置应用程序图标
    m_Titlew->SetWindowIcon("Icon/Sys.png");
    //设置最大化按钮不可用
    m_Titlew->m_MaximizeButton->setVisible(false);
    //主要垂直布局
    QVBoxLayout *mainLayout = this->getWindowLayout();

    //添加UI第一图层
    addFirstLayer(mainLayout);
    //添加歌词展示区
    addMusiclyric(mainLayout);
    //添加音乐区UI
    addMusicLayer(mainLayout);
    //添加功能区UI图层
    addWorkLayer(mainLayout);
    //添加音量区UI图层
    addvoiceLayer(mainLayout);

    createThread();
}

void Dialog::createTrayIcon()
{
    m_SysTrayIcon = new QSystemTrayIcon(this);
    m_SysTrayIcon->setIcon(QIcon("Icon/Sys.png"));

    //创建菜单动作
    QAction* restoreAction = new QAction(tr("&显示"), this);
    connect(restoreAction, &QAction::triggered, this, &Dialog::clickSysShow);

    //创建菜单动作
    QAction* CloseAction = new QAction(tr("&关闭"), this);
    connect(CloseAction, &QAction::triggered, this, &Dialog::Storedata);

    //创建菜单
    QMenu* trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    //创建菜单栏分隔符
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(CloseAction);

    //设置点击呼出菜单
    m_SysTrayIcon->setContextMenu(trayIconMenu);
    connect(m_SysTrayIcon, &QSystemTrayIcon::activated, this, &Dialog::iconActivated);
    m_SysTrayIcon->show();
    if(m_GreetingMap.size())
    {
        ShowTheFirstMeeting();
    }
}

//添加UI第一图层
void Dialog::addFirstLayer(QVBoxLayout *MainLayout)
{
    //获取主要窗口对象
    QWidget *Window = this->window();
    //初始化白胶底图
    m_DiscBackground = new QLabel(this);
    QImage Image;
    //加载图片
    Image.load("res/ditu.png");
    QPixmap pixmap = QPixmap::fromImage(Image);
    //设置QLabel控件的大小
    m_DiscBackground->setMaximumSize(QSize(Window->width()/1.6,Window->width()/1.6));
    //将图片导入到控件当中
    m_DiscBackground->setPixmap(pixmap);
    //设置图片填充饱满
    m_DiscBackground->setScaledContents(true);

    //初始化黑胶唱片
    m_BlackGlueBackground = new QLabel(this);
    QImage BImage;
    //加载图片
    BImage.load("res/heijiao.png");
    QPixmap Bpixmap = QPixmap::fromImage(BImage);
    //设置QLabel控件的大小
    m_BlackGlueBackground->setMaximumSize(QSize(Window->width()/1.8,Window->width()/1.8));
    //将图片导入到控件当中
    m_BlackGlueBackground->setPixmap(Bpixmap);
    //设置图片填充饱满
    m_BlackGlueBackground->setScaledContents(true);

    //初始化音乐封面
    m_Love = new MyLoveWindow(this->window(),Window->width()/2.3,Window->width()/2.3);

    //碟片UI网格布局
    QGridLayout* DiscLayout = new QGridLayout(this);
    //设置控件与窗体的上下空间为0
    DiscLayout->setSpacing(0);
    //设置控件在布局中的位置 1个像素点
    DiscLayout->setContentsMargins(1,1,1,1);
    //添加碟盘白底
    DiscLayout->addWidget(m_DiscBackground,0,1,3,4,Qt::AlignHCenter);
    //添加黑胶唱片
    DiscLayout->addWidget(m_BlackGlueBackground,0,1,3,4,Qt::AlignHCenter);
    //添加音乐封面
    DiscLayout->addWidget(m_Love,0,1,3,4,Qt::AlignBaseline|Qt::AlignHCenter);
    MainLayout->addLayout(DiscLayout);
}

//添加歌词展示区
void Dialog::addMusiclyric(QVBoxLayout *MainLayout)
{
    //存放歌词的垂直布局
    QVBoxLayout*lyricLayer = new QVBoxLayout;
    m_OverLyric = new QLabel(this);
    m_OnLyric   = new QLabel(this);
    m_NextLyric = new QLabel(this);

    QFont Tft("Microsoft YaHei", 8, QFont::Light);
    m_OverLyric->setFont(Tft);
    m_NextLyric->setFont(Tft);
    m_OnLyric->setStyleSheet("color:#F8F8FF;");

    QFont Tft2("Microsoft YaHei", 9, QFont::Normal);
    m_OnLyric->setFont(Tft2);

    lyricLayer->addStretch(5);
    lyricLayer->addWidget(m_OverLyric,1,Qt::AlignCenter);
    lyricLayer->addStretch(5);
    lyricLayer->addWidget(m_OnLyric,1,Qt::AlignCenter);
    lyricLayer->addStretch(5);
    lyricLayer->addWidget(m_NextLyric,1,Qt::AlignCenter);

    MainLayout->addLayout(lyricLayer);
}

//添加功能区UI图层
void Dialog::addWorkLayer(QVBoxLayout *MainLayout)
{
    //初始化功能区水平布局
    QHBoxLayout* WorkLayout = new QHBoxLayout;

    //添加功能按钮

    //1.播放模式按钮
    WorkLayout->addStretch();
    m_PlayModButton = new QPushButton(this);
    m_PlayModButton->setMinimumSize(30,30);
    m_PlayModButton->setMaximumSize(30,30);
    m_PlayModButton->setFlat(true);
    QIcon ModIcon;
    SetPlayMod(Dialog::LP);
    WorkLayout->addWidget(m_PlayModButton);


    //2.快退按钮
    m_RewindButton = new QPushButton(this);
    m_RewindButton->setMinimumSize(35,35);
    m_RewindButton->setMaximumSize(35,35);
    m_RewindButton->setFlat(true);
    QIcon RewindIcon;
    RewindIcon.addFile(tr("res/kuaitui.png"));
    m_RewindButton->setIcon(RewindIcon);
    m_RewindButton->setIconSize(QSize(35,35));
    WorkLayout->addWidget(m_RewindButton);

    //3.播放按钮
    m_PlayButton = new QPushButton(this);
    m_PlayButton->setMinimumSize(45,45);
    m_PlayButton->setMaximumSize(45,45);
    m_PlayButton->setFlat(true);
    QIcon PlayIcon;
    PlayIcon.addFile(tr("res/bofang.png"));
    m_MusicState = Dialog::suspend;
    m_PlayButton->setIcon(PlayIcon);
    m_PlayButton->setIconSize(QSize(45,45));
    WorkLayout->addWidget(m_PlayButton);

    //4.快进按钮
    m_FastForwardButton  = new QPushButton(this);
    m_FastForwardButton->setMinimumSize(35,35);
    m_FastForwardButton->setMaximumSize(35,35);
    m_FastForwardButton->setFlat(true);
    QIcon FastForwardIcon;
    FastForwardIcon.addFile(tr("res/kuaijin.png"));
    m_FastForwardButton->setIcon(FastForwardIcon);
    m_FastForwardButton->setIconSize(QSize(35,35));
    WorkLayout->addWidget(m_FastForwardButton);

    //5.歌词显示按钮
    m_LyricsButton  =   new QPushButton(this);
    m_LyricsButton  ->setMinimumSize(30,30);
    m_LyricsButton  ->setMaximumSize(30,30);
    m_LyricsButton  ->setFlat(true);
    SetLyricsState();
    WorkLayout->addWidget(m_LyricsButton);


    WorkLayout->addStretch();
    WorkLayout->setSpacing(30);
    MainLayout->addLayout(WorkLayout);
    MainLayout->addStretch(0);
}

// 添加音乐区UI
void Dialog::addMusicLayer(QVBoxLayout *MainLayout)
{
    QString space = "     ";
    QVBoxLayout* MusiLayout =   new QVBoxLayout(this);
    //音乐名称水平布局
    QHBoxLayout* NameLayout =   new QHBoxLayout(this);
    m_MusicName             =   new QLabel(QString("未在播放"),this);
    QFont ft("Microsoft YaHei", 12, QFont::Normal);
    m_MusicName             ->setFont(ft);
    m_MusicName             ->setMaximumSize(300,30);
    m_MusicName             ->setMaximumSize(300,30);
    m_MusicName             ->setStyleSheet("color:#F8F8FF;");

    QLabel* Namespace       =   new QLabel(space,this);
    NameLayout              ->addWidget(Namespace);
    NameLayout              ->addWidget(m_MusicName);
    NameLayout              ->addStretch(0);
    //初始化音乐滑块区水平布局
    QHBoxLayout* SliderLayout   =   new QHBoxLayout(this);
    m_MusicSlider               =   new QSlider(Qt::Horizontal,this);
    //装载事件筛选器
    m_MusicSlider               ->installEventFilter(this);

    //设置滑动条控件的最小值
    m_MusicSlider           ->setMinimum(0);
    //设置滑动条控件的最大值
    m_MusicSlider           ->setMaximum(100);
    //设置滑动条控件的值
    m_MusicSlider           ->setValue(0);
    m_MusicSlider           ->setMaximumSize(330,20);
    m_MusicSlider           ->setMaximumSize(330,20);
    SliderLayout            ->addWidget(m_MusicSlider);

    //存放音乐播放时间和剩余时间的布
    QHBoxLayout* TimeLayout =   new QHBoxLayout(this);
    QLabel* TimespaceBeing  =   new QLabel(space,this);
    QLabel* Timespaceend    =   new QLabel(space,this);
    QFont Tft("Microsoft YaHei", 8, QFont::Normal);
    m_MusicBeginTime        =   new QLabel(QString("00:00"),this);
    m_MusicBeginTime        ->setFont(Tft);
    m_MusicEndTime          =   new QLabel(QString("00:00"),this);
    m_MusicEndTime          ->setFont(Tft);
    m_MusicBeginTime        ->setStyleSheet("color:#FFFFF0;");
    m_MusicEndTime          ->setStyleSheet("color:#FFFFF0;");

    m_MusicBeginTime        ->setMaximumSize(50,15);
    m_MusicEndTime          ->setMaximumSize(50,15);
    TimeLayout              ->addWidget(TimespaceBeing);
    TimeLayout              ->addWidget(m_MusicBeginTime);
    TimeLayout              ->addStretch(0);
    TimeLayout              ->addWidget(m_MusicEndTime);
    TimeLayout              ->addWidget(Timespaceend);

    MusiLayout              ->addLayout(NameLayout);
    MusiLayout              ->addStretch(0);
    MusiLayout              ->addLayout(SliderLayout);
    MusiLayout              ->addLayout(TimeLayout);
    MusiLayout              ->addStretch(0);
    MainLayout              ->addLayout(MusiLayout);
}

// 添加音量区UI
void Dialog::addvoiceLayer(QVBoxLayout *MainLayout)
{
    QHBoxLayout*lyricLayer  =   new QHBoxLayout;
    m_VoiceSlider           =   new QSlider(Qt::Horizontal,this);
    m_VoiceSlider           ->setMaximum(100);
    m_VoiceSlider           ->setMinimum(0);
    //装载事件筛选器
    m_VoiceSlider           ->installEventFilter(this);

    QLabel*  reduce = new QLabel(this);
    QImage BImage;
    //加载图片
    BImage.load("res/yingliang.png");
    QPixmap Bpixmap = QPixmap::fromImage(BImage);
    //设置QLabel控件的大小
    reduce->setMaximumSize(QSize(20,20));
    //将图片导入到控件当中
    reduce->setPixmap(Bpixmap);
    //设置图片填充饱满
    reduce->setScaledContents(true);

    lyricLayer->addWidget(reduce);

    //设置滑动条控件的最小值
    m_VoiceSlider           ->setMinimum(0);
    //设置滑动条控件的最大值
    m_VoiceSlider           ->setMaximum(100);

    //设置滑动条控件的值
    m_VoiceSlider           ->setValue(DefaultVolume);
    On_VoiceMoved(m_VoiceSlider->value());
    m_VoiceSlider           ->setMaximumSize(330,20);
    m_VoiceSlider           ->setMaximumSize(330,20);
    lyricLayer              ->addWidget(m_VoiceSlider);

    QLabel*  increase = new QLabel(this);
    QImage BImage2;
    //加载图片
    BImage2.load("res/yingliang1.png");
    QPixmap Bpixmap2 = QPixmap::fromImage(BImage2);
    //设置QLabel控件的大小
    increase->setMaximumSize(QSize(20,20));
    //将图片导入到控件当中
    increase->setPixmap(Bpixmap2);
    //设置图片填充饱满
    increase->setScaledContents(true);

    lyricLayer->addWidget(increase);
    lyricLayer              ->setSpacing(40);
    lyricLayer              ->setMargin(20);
    MainLayout              ->addLayout(lyricLayer);
    MainLayout              ->addStretch(1);
}

// 设置UI的样式
void Dialog::setUIStyleSheet()
{
    // 创建列表并将功能区的五个按钮添加到列表中
    QList<QPushButton*> StyleButton;
    StyleButton.append(m_PlayModButton);
    StyleButton.append(m_FastForwardButton);
    StyleButton.append(m_RewindButton);
    StyleButton.append(m_PlayButton);
    StyleButton.append(m_LyricsButton);

    // 设置悬停注释消息
    m_PlayModButton->setToolTip(GetPlayModStr());
    m_FastForwardButton->setToolTip(tr("下一首"));
    m_RewindButton->setToolTip(tr("上一首"));
    m_PlayButton->setToolTip(GetMusicStateStr());
    m_LyricsButton->setToolTip(GetLyricsStateStr());

    QList<QPushButton*>::iterator pr = StyleButton.begin();
    // 循环遍历按钮设置通用样式,格式类似CSS
    for(;pr!=StyleButton.end();++pr)
    {
        (*pr)->setStyleSheet(
                    //正常状态样式
                    "QPushButton{"
                                       "color:rgba(0,0,0,0);"              //字体颜色
                                       "background-color:transparent;"
                    "}"
                    //鼠标悬停样式
                    "QPushButton:hover{"
                                       "color:rgba(0,0,0,0);"
                    "}"
                    //鼠标按下样式
                    "QPushButton:pressed{"
                                      "color:rgba(0,0,0,0);"
                    "}"
                    );
        //鼠标停靠在按钮上变成手形
        (*pr)->setCursor(QCursor(Qt::PointingHandCursor));
    }

    // 创建列表并将音乐区的两个滑动条添加到列表中
    QList<QSlider*> MusicButton;
    MusicButton.append(m_MusicSlider);
    MusicButton.append(m_VoiceSlider);
    //设置滑块样式
    QList<QSlider*>::iterator dr = MusicButton.begin();
    // 遍历循环滑动条设置通用样式
    for(;dr!=MusicButton.end();++dr)
    {
        (*dr)->setStyleSheet("QSlider::groove:horizontal {"
                             "border: 1px solid #aba1a1;"
                             "background: #aba1a1;"
                             "height: 3px;"
                             "border-radius: 1px;"
                             "padding-left:0px;"
                             "padding-right:0px;"
                             "}"

                             /*滑块经过的颜色:前面的颜色*/
                             "QSlider::sub-page:horizontal {"
                             "background: #e01616;"
                             "border: 1px solid #e01616;"
                             "height: 3px;"
                             "border-radius: 3px;"
                             "}"

                             "QSlider::handle:horizontal {"
                                 "background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,"
                                 "stop:0.6 #e01616,stop:0.98409 rgba(0,0,0,0));"

                                 "width: 15px;"
                                 "margin-top: -6px;"
                                 "margin-bottom: -6px;"
                                 "border-radius: 5px;"
                             "}"

                             "QSlider::handle:horizontal:hover {"
                                 "background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,"
                                 "stop:0.6 #e01616,stop:0.98409 rgba(0,0,0,0));"

                                 "width: 15px;"
                                 "margin-top: -6px;"
                                 "margin-bottom: -6px;"
                                 "border-radius: 5px;"
                             "}"
                            );
    }
}
void Dialog::ShowWindow()
{
    QWidget *Window = this->window();
    // 设置动画效果
    QPropertyAnimation *animation = new QPropertyAnimation(Window, "windowOpacity");
    animation->setDuration(1500);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    connect(animation, SIGNAL(finished()), Window, SLOT(show()));
}

void Dialog::ShowTheFirstMeeting()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString day = current_date_time.toString("ddd");
    QString hour = current_date_time.toString("hh");

    int timeSlot;
    if(hour.toInt() >= 0 && hour.toInt() < 6)
    {
        //凌晨
        timeSlot = Dialog::thesmallhoursTime;
    }
    else if(hour.toInt() >= 6 && hour.toInt() < 11)
    {
        //早晨
        timeSlot = Dialog::morningTime;
    }
    else if(hour.toInt() >= 11 && hour.toInt() < 15)
    {
        //中午
        timeSlot = Dialog::noonTime;
    }
    else if(hour.toInt() >= 15 && hour.toInt() < 18)
    {
        //下午
        timeSlot = Dialog::afternoonTime;
    }
    else if(hour.toInt() >= 18 && hour.toInt() <= 24)
    {
        //晚上
        timeSlot = Dialog::nightTime;
    }
    if(!m_GreetingMap.contains(timeSlot))
    {
        return;
    }
    QString TheFirstMeeting = m_GreetingMap.find(timeSlot).value();
    QStringList Data = TheFirstMeeting.split("|");

    QString png = "Icon/Sys.png";
    if(Data.size() == 2)
    {
        QString name = Data.at(0) + "";
        QString love = day+Data.at(1);
        QIcon Icon(png);
        m_SysTrayIcon->showMessage(name, love, Icon,
                          80 * 1000);
    }
}


//----------------------------------------------------------------------logic----------------------------------------------------------------------//

// 更改播放模式槽函数
void Dialog::On_PlayMod(bool checked)
{
    ++m_PlayMod;
    if(m_PlayMod > Dialog::MusicMode::PR)
        m_PlayMod = Dialog::MusicMode::STC;

    SetPlayMod(m_PlayMod);
    m_PlayModButton->setToolTip(GetPlayModStr());
}

void Dialog::createThread()
{
    /*线程初始化*/
    m_UpDataThread->setMusicSlider(m_MusicSlider);
    m_UpDataThread->setBeginTime(m_MusicBeginTime);
    m_UpDataThread->setEndTime(m_MusicEndTime);
    m_UpDataThread->setMusicName(m_MusicName);
    m_UpDataThread->setUpdateDisc(m_Love);
    m_UpDataThread->SetLyrics(m_OverLyric,m_OnLyric,m_NextLyric);
    m_UpDataThread->setLyricsState(m_LyricsState);
    m_UpDataThread->start();
    return;
}

void Dialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:break;
    case QSystemTrayIcon::DoubleClick:
        this->ShowWindow();
        break;
        //鼠标中击显示消息
    case QSystemTrayIcon::MiddleClick:
        //showMessage();
        break;
    default:
        ;
    }
}

// 设置点击托盘图标槽函数
void Dialog::clickSysShow()
{
    if(!this->window()->isHidden())
    {
        return;
    }
    this->ShowWindow();
    return;
}

bool Dialog::eventFilter(QObject *watched, QEvent *event)
{
    QVector<QSlider*>SliderVector;
    SliderVector.push_back(m_MusicSlider);
    SliderVector.push_back(m_VoiceSlider);

    for(int i = 0; i < SliderVector.size();++i)
    {
        //如果选中的QObject对象是滑块控件
        if(watched == SliderVector.at(i))
        {
            if(!SliderVector.at(i)->isEnabled())
            {
                //再次交给Qt内核处理
                return QObject::eventFilter(watched,event);
            }
            //如果动作的类型是按下鼠标按钮
            if(event->type() == QEvent::MouseButtonPress)
            {
                //强制转换为鼠标对象
                QMouseEvent*mouseEvent = static_cast<QMouseEvent *>(event);
                //如果鼠标是左键点击的话，设置点击长度条任意处即可跳到相应位置并可拖动
                if(mouseEvent->button() == Qt::LeftButton)
                {
                    int value = QStyle::sliderValueFromPosition(SliderVector.at(i)->minimum(),SliderVector.at(i)->maximum(),mouseEvent->pos().x(),SliderVector.at(i)->width());
                    SliderVector.at(i)->setValue(value);
                    if(SliderVector.at(i) == m_MusicSlider)
                    {
                        m_UpDataThread->m_player->setPosition(value);
                    }
                    else if(SliderVector.at(i) == m_VoiceSlider)
                    {
                        m_UpDataThread->m_player->setVolume(value);
                    }
                    if(m_MusicState == Dialog::State::suspend)
                    {
                        int minute = int(value / 1000 / 60);
                        int second = int(value / 1000 % 60);

                        std::string Time = std::to_string(minute) + ":" + std::to_string(second);
                        QString strQ = QString::fromLocal8Bit(Time.c_str());
                        m_MusicBeginTime->setText(strQ);
                    }
                }
            }
        }
    }
    //再次交给Qt内核处理
    return QObject::eventFilter(watched,event);
}

// 关闭窗口存储缓存槽函数
void Dialog::Storedata()
{
    SaveCache();
    m_UpDataThread->closeThread();
    qApp->quit();
    return;
}

void Dialog::SaveCache()
{
    QString FilePath= QApplication::applicationDirPath() +
            "/Config/initializationConfig.xml";

    QFile XmlFile(FilePath);
    //使用只读的方式打开XML文件并且判断是否成功读取
    if(!XmlFile.open(QFile::ReadWrite))
    {
        return;
    }

    // QDomDocument表示一个XML文档类
    //QDomDocument类表示整个XML文档。从概念上讲，它是文档树的根，并提供对文档数据的主要访问。
    QDomDocument docXML;


    if(!docXML.setContent(&XmlFile))
    {
         XmlFile.close();
         return;
    }
    //QdomeElement类表示DOM树中的一个元素。
    QDomElement xmlconfig = docXML.documentElement(); //documentElement 返回文档的根元素。

    QString Time = QString::number(m_UpDataThread->m_player->position());
    QString Name = m_UpDataThread->GetMusicByIndex(m_UpDataThread->GetMusicIndx());
    QString volume = QString::number(m_UpDataThread->m_player->volume());
    for(QDomElement Format = xmlconfig.firstChildElement("Data");!Format.isNull();Format = Format.nextSiblingElement("Data"))
    {
        for(QDomElement node = Format.firstChildElement("Lastplay");!node.isNull();node = node.nextSiblingElement("Lastplay"))
        {
            node.toElement().setAttribute("Name",Name);
            node.toElement().setAttribute("Timer",Time);
            node.toElement().setAttribute("volume",volume);
        }
    }
    XmlFile.close();
    if(!XmlFile.open(QIODevice::Truncate | QIODevice::WriteOnly))
                  return ;
    QTextStream out(&XmlFile);
    docXML.save(out,1);
    XmlFile.close();
    return;
}

void Dialog::Datainitialization()
{
    LoadConfig();
    LoadUI();
}

void Dialog::LoadConfig()
{
    m_GreetingMap.clear();
    QString FilePath= QApplication::applicationDirPath() +
            "/Config/initializationConfig.xml";

    QFile XmlFile(FilePath);
    //使用只读的方式打开XML文件并且判断是否成功打开
    if(!XmlFile.open(QFile::ReadOnly))
    {
        return;
    }

    // QDomDocument表示一个XML文档类
    //QDomDocument类表示整个XML文档。从概念上讲，它是文档树的根，并提供对文档数据的主要访问。
    QDomDocument docXML;


    if(!docXML.setContent(&XmlFile))
    {
         XmlFile.close();
         return;
    }

    // docXML初始化成功  不在需要XmlFile
    XmlFile.close();
    //QdomeElement类表示DOM树中的一个元素。
    QDomElement xmlconfig = docXML.documentElement(); //documentElement 返回文档的根元素。

    QString Time;
    QString Data;
    //title节点
    for(QDomElement Format = xmlconfig.firstChildElement("title");!Format.isNull();Format = Format.nextSiblingElement("title"))
    {
        //标题
        QString Title = Format.toElement().attribute("word");
        //循环node节点
        for(QDomElement node = Format.firstChildElement("node");!node.isNull();node = node.nextSiblingElement("node"))
        {
            //保存见面文字
            QString word = node.toElement().attribute("word");
            Data = word + "|";
            //循环greetings节点
            for(QDomElement greetings = node.firstChildElement("greetings");!greetings.isNull();greetings = greetings.nextSiblingElement("greetings"))
            {
                QString WORD = greetings.toElement().attribute("word");
                Data+=WORD;
                QString TIME = greetings.toElement().attribute("Timer");
                Time = TIME;
            }
           m_GreetingMap.insert(Time.toInt(),Data);
           Time.clear();
           Data.clear();
        }
    }
    //缓存数据
    for(QDomElement Format = xmlconfig.firstChildElement("Data");!Format.isNull();Format = Format.nextSiblingElement("Data"))
    {
        QMap<QString,QString>CacheData;
        QString Name;
        //Lastplay节点
        for(QDomElement node = Format.firstChildElement("Lastplay");!node.isNull();node = node.nextSiblingElement("Lastplay"))
        {
            //保存音乐名称
            Name            =   node.toElement().attribute("Name");
            //保存播放时间
            QString Timer   =   node.toElement().attribute("Timer");
            //保存播放音量
            QString volume  =   node.toElement().attribute("volume");
            CacheData.insert(Timer,volume);
        }
        m_CacheData.insert(Name,CacheData);
        CacheData.clear();
    }
    return;
}

void Dialog::LoadUI()
{
    m_MusicState = Dialog::suspend;
    //如果没有找到任何音乐
    if(!m_UpDataThread->GetMusiList().size())
    {
        m_MusicSlider->setEnabled(false);
        return;
    }
    if(m_CacheData.size())
    {
        bool cache = false;
        for(int i = 0;  i < m_UpDataThread->GetMusiList().size();   i++)
        {
            if(m_CacheData.find(m_UpDataThread->GetMusiList().at(i)) != m_CacheData.end())
            {
                m_UpDataThread->setMusicIndx(i);
                //设置缓存音量
                QString volume = m_CacheData.value(m_UpDataThread->GetMusiList().at(i)).first();
                m_UpDataThread->m_player->setVolume(volume.toInt());
                //设置缓存时间
                QString Time = m_CacheData.value(m_UpDataThread->GetMusiList().at(i)).lastKey();
                m_MusicTimeNow = Time.toLongLong();
                m_UpDataThread->m_player->setMedia(QUrl::fromLocalFile(m_UpDataThread->GetMusicByIndex(m_UpDataThread->GetMusicIndx())));
                m_VoiceSlider->setValue(volume.toInt());
                //暂停线程
                m_UpDataThread->threadPause();
                cache = true;
                break;
            }
        }
        if(cache == false)
        {
            m_UpDataThread->setMusicIndx(0);
            m_UpDataThread->m_player->setMedia(QUrl::fromLocalFile(m_UpDataThread->GetMusicByIndex(m_UpDataThread->GetMusicIndx())));
            //暂停线程
            m_UpDataThread->threadPause();
        }

        m_UpDataThread->UpDataName();
    }
}

void Dialog::intitD(qint64 duration)
{
    m_MusicSlider->setRange(0,duration);
    m_MusicSlider->setValue(m_MusicTimeNow);
    m_UpDataThread->m_player->setPosition(m_MusicTimeNow);

    int minute = int(m_MusicTimeNow / 1000 / 60);
    int second = int(m_MusicTimeNow / 1000 % 60);

    std::string strminute;
    std::string strsecond;
    if(minute < 10)
    {
        strminute = "0" + std::to_string(minute);
    }
    else {
        strminute = std::to_string(minute);
    }
    if(second < 10)
    {
        strsecond = "0" + std::to_string(second);
    }
    else {
        strsecond = std::to_string(second);
    }
    std::string Time = strminute + ":" + strsecond;
    QString strQ = QString::fromLocal8Bit(Time.c_str());
    m_MusicBeginTime->setText(strQ);
}

// 播放按钮槽函数
void Dialog::On_playBtn(bool checked)
{
    if(m_MusicState == Dialog::suspend)
    {
        m_MusicState = Dialog::conduct;
    }
    else {
        m_MusicState = Dialog::suspend;
    }
    m_PlayButton->setToolTip(GetMusicStateStr());
    PlayMusic(m_UpDataThread->GetMusicIndx(),m_MusicState);
}

void Dialog::PlayMusic(const int&indx,bool state)
{
    m_MusicState = state;
    if(m_MusicState == Dialog::suspend)
    {
        if(m_UpDataThread->m_player->state() == QMediaPlayer::PlayingState)
        {
            QIcon icon("res/bofang.png");
            QString str = icon.name();
            m_PlayButton->setIcon(icon);
            //暂停线程
            m_UpDataThread->threadPause();
        }
    }
    else if(m_MusicState == Dialog::conduct){
        m_MusicState = Dialog::conduct;
        QIcon icon("res/tingzhi.png");
        m_PlayButton->setIcon(icon);
        //开启线程
        m_UpDataThread->threadResume();
    }
    m_UpDataThread->UpDataName();
}
void Dialog::SetPlayMod(int Mod)
{
    QString Pix;
    switch(Mod)
    {
    case Dialog::LP:Pix = "res/congfu.png";        break;
    case Dialog::PR:Pix = "res/suiji.png";          break;
    case Dialog::STC:Pix = "res/jiantou.png";       break;
        default:
        Pix = "res/liebiao.png";
    }
    QIcon icon;
    icon.addFile(Pix);
    //qDebug() << icon.name();
    m_PlayModButton->setIcon(icon);
    m_PlayModButton->setIconSize(QSize(30,30));
    m_PlayMod = Mod;
    m_UpDataThread->setPlayMod(m_PlayMod);
    return;
}

void Dialog::SetLyricsState(int State)
{
    QString Pix;
    switch(State)
    {
    case Dialog::CloseLyrics:Pix    =   "res/yinfu.png";        break;
    case Dialog::OpenLyrics:Pix     =   "res/yinfu.png";        break;
    }
    QIcon icon;
    icon.addFile(Pix);
    m_LyricsButton->setIcon(icon);
    m_LyricsButton->setIconSize(QSize(30,30));
    m_LyricsState = State;
    return;
}

QString Dialog::GetPlayModStr()
{
    QString PlayMod;
    switch(m_PlayMod)
    {
    case Dialog::LP:PlayMod = tr("列表循环");           break;
    case Dialog::PR:PlayMod = tr("随机播放");           break;
    case Dialog::STC:PlayMod = tr("单曲循环");          break;
        default:
        PlayMod = tr("错误");
    }
    return PlayMod;
}
QString Dialog::GetLyricsStateStr()
{
    QString LyricsState;
    switch(m_LyricsState)
    {
    case Dialog::CloseLyrics:LyricsState    =   tr("打开歌词");        break;
    case Dialog::OpenLyrics:LyricsState     =   tr("关闭歌词");        break;
        default:
        LyricsState = tr("错误");
    }
    return LyricsState;
}

QString Dialog::GetMusicStateStr()
{
    QString MusicState;
    switch(m_MusicState)
    {
    case Dialog::suspend:MusicState    =   tr("播放");        break;
    case Dialog::conduct:MusicState    =   tr("暂停");        break;
        default:
        MusicState = tr("错误");
    }
    return MusicState;
}

void Dialog::On_Fast_ForwardBtn(bool checked)
{
    int index = m_UpDataThread->GetMusicIndx();
    m_UpDataThread->setMusicIndx(++index);
    if(m_UpDataThread->GetMusicIndx() >= m_UpDataThread->GetMusiList().size())
        m_UpDataThread->setMusicIndx(0);

    m_UpDataThread->SwitchSongs(m_MusicState);
    m_MusicState = Dialog::State::conduct;
    if(m_UpDataThread->m_player->state() == QMediaPlayer::PlayingState)
    {
        QIcon icon("res/tingzhi.png");
        m_PlayButton->setIcon(icon);
    }
}

// 上一首歌曲
void Dialog::On_RewindBtn(bool checked)
{
    int index = m_UpDataThread->GetMusicIndx();
    m_UpDataThread->setMusicIndx(--index);
    // 第一首的上一首是最后一首
    if(m_UpDataThread->GetMusicIndx() < 0)
    {
        int newindex = m_UpDataThread->GetMusiList().size() - 1;
        m_UpDataThread->setMusicIndx(newindex);
    }
    m_UpDataThread->SwitchSongs(m_MusicState);
    m_MusicState = Dialog::State::conduct;
    if(m_UpDataThread->m_player->state() == QMediaPlayer::PlayingState)
    {
        QIcon icon("res/tingzhi.png");
        m_PlayButton->setIcon(icon);
    }
}

void Dialog::On_sliderMoved(int position)
{
    m_UpDataThread->m_player->setPosition(position);

    int minute = int(position / 1000 / 60);
    int second = int(position / 1000 % 60);

    std::string Time = std::to_string(minute) + ":" + std::to_string(second);
    QString strQ = QString::fromLocal8Bit(Time.c_str());
    m_MusicBeginTime->setText(strQ);
}

void Dialog::On_VoiceMoved(int position)
{
    m_UpDataThread->m_player->setVolume(position);
}

//歌词开关按钮
void Dialog::On_Lyrics_Switch(bool checked)
{
    bool LyricsSwitch = false;
    if(m_LyricsState == false)
    {
        LyricsSwitch = true;
        if(m_MusicState == Dialog::suspend)
        {
            m_UpDataThread->UpDataLyrics();
        }
    }
    else {
        m_OnLyric->setText(" ");
        m_OverLyric->setText(" ");
        m_NextLyric->setText(" ");
    }
    SetLyricsState(LyricsSwitch);
    m_UpDataThread->setLyricsState(LyricsSwitch);

    m_LyricsButton->setToolTip(GetLyricsStateStr());
}
//切换播放背景
void Dialog::SwitchBackground(QString name)
{
    QString path    =   m_cachethread->GetCacheFilePath()   +   "/"     +   name    +   ".png";
    BSwitchBackground(path);
}
void Dialog::createCachethread()
{
    QString resPath =  "MyLove";
    m_cachethread = new Cachethread(resPath,m_UpDataThread->GetMusiNameList(),this);
    m_cachethread->start();
}
