#ifndef THREAD_H
#define THREAD_H

#endif // THREAD_H
#ifndef MYTHREAD_H
#define MYTHREAD_H

/*
  TODO: 子线程 用来处理歌词、播放时间等
  Author: ZzBoAYU
  Date: 2023.05.07
*/

#include <QThread>
#include <QtMultimedia/QMediaPlayer>
#include <QSlider>
#include <QLabel>
#include <QMutex>
#include "mylovewindow.h"

class MyThread : public QThread
{
    Q_OBJECT
signals:
    void intit(qint64 duration);
    void SwitchBackground(QString name);
public:
    MyThread();
    void closeThread();

    // STC == Single tune circulation   单曲循环
    // LP == List loop                  列表循环
    // PR == Random play                随机播放
    enum MusicMode {STC, LP, PR};
    enum State {suspend,conduct};

    //关闭歌词，打开歌词
    enum{CloseLyrics,OpenLyrics};
protected:
    virtual void run();

private:
    volatile bool isStop;       //isStop是易失性变量，需要用volatile进行申明
    QMutex m_mutex;//互斥量
    //音乐列表
    typedef std::vector<QString> MusicList;
    MusicList m_MyMusic;
public:
    void setMusicSlider(QSlider*Slider){m_SubSlider = Slider;}
    void setBeginTime(QLabel*BeginTime){m_SubBeginTime = BeginTime;}
    void setEndTime(QLabel*EndTime){m_SubEndTime = EndTime;}
    void setMusicName(QLabel*MusicName){m_SubMusicName = MusicName;}
    void setUpdateDisc(MyLoveWindow* SubLove) {m_SubMyLoveWindow = SubLove;}
    void setLyricsState(bool LyricsState) {m_SubLyricsState = LyricsState;}
    void SetLyrics(QLabel*Over,QLabel*On,QLabel*Next) {m_SubOverLyric = Over;m_SubOnLyric = On;m_SubNextLyric = Next;}
    void UpDataName();
    void setPlayMod(int mod);
    void setMusicIndx(int index){m_MusicIndx = index;}
    int  GetMusicIndx() {return m_MusicIndx;}
    void SwitchSongs(bool State);
    bool GetThreadState(){return m_SubMusicState;}
    QString GetMusicByIndex(const int&indx);
    std::vector<QString> GetMusiList(){return m_MyMusic;}
    std::vector<QString>  GetMusiNameList();
    void UpDataLyrics();

    //线程暂停和开启
    void threadPause();
    void threadResume();
private:
    //上一句歌词
    QLabel*             m_SubOverLyric;
    //当前歌词
    QLabel*             m_SubOnLyric;
    //一下句歌词
    QLabel*             m_SubNextLyric;

    QSlider*            m_SubSlider;
    QLabel*             m_SubBeginTime;
    QLabel*             m_SubEndTime;
    QLabel*             m_SubMusicName;
    MyLoveWindow*       m_SubMyLoveWindow;
    //当前播放进度ms
    qint64              m_SubMusicTimeNow;
    //播放总时间
    qint64              m_SubMusicTime;
    int                 m_SubPlayMod;
    //播放状态
    bool                m_SubMusicState;
    //歌词状态
    bool                m_SubLyricsState;

    //歌词容器 QMap<音乐名称,QMap<时间,歌词>>
    QMap<QString,QMap<QString,QString>>     m_LyricMap;
    bool                m_initialization = true;
public:
    QMediaPlayer *      m_player;
    //当前播放索引
    int                 m_MusicIndx;
private slots:
    void UpdateMusic(qint64 duration);
private:
    int  LoadMusic();
    bool LoadLyric();
};

#endif // MYTHREAD_H
