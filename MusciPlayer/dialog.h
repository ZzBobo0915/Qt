#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QIcon>
#include <QThread>
#include <QPushButton>
#include "ForYou/basiwindow.h"
#include "ForYou/cachethread.h"
#include <QSlider>
#include <QtMultimedia/QMediaPlayer>
#include <QSystemTrayIcon>
#include "thread.h"
#include "mylovewindow.h"
namespace Ui {
class Dialog;
}

class Dialog : public Basiwindow
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
public:
    void Datainitialization();
    void createwindow();
    void ShowWindow();
private:
    void addFirstLayer(QVBoxLayout *MainLayout);
    void addWorkLayer (QVBoxLayout *MainLayout);
    void addMusicLayer(QVBoxLayout *MainLayout);
    void addMusiclyric(QVBoxLayout *MainLayout);
    void addvoiceLayer(QVBoxLayout *MainLayout);
    //创建系统托盘
    void createTrayIcon();
    void createThread();
    void ShowTheFirstMeeting();
    void setUIStyleSheet();
    void LoadConfig();
    void LoadUI();
    void PlayMusic(const int&indx,bool state = true);
    void SaveCache();
    //初始化缓存线程对象
    void createCachethread();
    //设置播放模式
    void SetPlayMod(int Mod);
    //设置歌词显示状态
    void SetLyricsState(int State = Dialog::OpenLyrics);
    QString GetPlayModStr();
    QString GetLyricsStateStr();
    QString GetMusicStateStr();
private slots:
    void Storedata();
    void clickSysShow();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void On_PlayMod(bool checked);
    void On_playBtn(bool checked);
    void On_Fast_ForwardBtn(bool checked);
    void On_RewindBtn(bool checked);
    void On_sliderMoved(int);
    void On_VoiceMoved(int);
    void intitD(qint64);
    void On_Lyrics_Switch(bool checked);
    void SwitchBackground(QString name);
private:
    //初始化音量值
    const int static DefaultVolume = 30;
    //更新精度
    const int static Update_Accuracy = 100;

    // STC == Single tune circulation   单曲循环
    // LP == List loop                  列表循环
    // PR == Random play                随机播放
    enum MusicMode {STC, LP, PR};
    enum State {suspend,conduct};
    //所有，早上，中午，下午，晚上，凌晨
    enum{AllTime,morningTime,noonTime,afternoonTime,nightTime,thesmallhoursTime};
    //关闭歌词，打开歌词
    enum{CloseLyrics,OpenLyrics};
protected:
    //从写事件过滤器函数
    bool eventFilter(QObject *watched, QEvent *event);
private:
    //系统托盘对象
    QSystemTrayIcon*    m_SysTrayIcon;

    //音乐名称，播放时间，播放音量
    typedef QMap<QString,QMap<QString,QString>>LastTime;
    //用户缓存数据
    LastTime m_CacheData;

    QLabel*             m_MicroChatUI;
    QGroupBox *         m_dataGroupBox;
    //播放模式按钮
    QPushButton*        m_PlayModButton;
    //播放按钮
    QPushButton*        m_PlayButton;
    //快进按钮
    QPushButton*        m_FastForwardButton;
    //快退按钮
    QPushButton*        m_RewindButton;
    //歌词显示按钮
    QPushButton*        m_LyricsButton;
    //白底背景图
    QLabel*             m_DiscBackground;
    //黑胶底图
    QLabel*             m_BlackGlueBackground;
    //音乐碟片
    MyLoveWindow*       m_Love;
    //音乐名称
    QLabel*             m_MusicName;
    //进度条控件
    QSlider*            m_MusicSlider;
    //音量条控件
    QSlider*            m_VoiceSlider;
    //当前播放时间
    QLabel*             m_MusicBeginTime;
    //剩余播放时间
    QLabel*             m_MusicEndTime;
    //歌词当前播放容器
    QMap<QString,QString> m_Lyric;
    //上一句歌词
    QLabel*             m_OverLyric;
    //当前歌词
    QLabel*             m_OnLyric;
    //一下句歌词
    QLabel*             m_NextLyric;
    //当前播放进度ms
    qint64              m_MusicTimeNow;
    //播放总时间
    qint64              m_MusicTime;
    //播放状态
    bool                m_MusicState;
    //歌词状态
    bool                m_LyricsState;
    //数据更新线程
    MyThread*           m_UpDataThread;
    //缓存线程
    Cachethread*        m_cachethread;
    //播放模式
    int                 m_PlayMod;
private:
    //问候语字典
    QMap<int,QString>   m_GreetingMap;
private:
    //播放开始时间
    qint64              m_TimeBeing;
};

#endif // DIALOG_H
