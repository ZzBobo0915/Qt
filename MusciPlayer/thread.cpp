#include "thread.h"
#include <QDebug>
#include <QMutex>
#include <io.h>
#include <stdio.h>
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QDomDocument>

MyThread::MyThread()
{
    m_player = new QMediaPlayer();
    LoadMusic();
    LoadLyric();
    QObject::connect(m_player,
                      &QMediaPlayer::durationChanged,
                      this,
                      &MyThread::UpdateMusic);

    isStop = false;
}
int MyThread::LoadMusic()
{
    char buff[1000];
    _getcwd(buff, 1000);
    //文件句柄
    long  hFile = 0;
    //文件信息
    /*
    struct _finddata_t
        {
            unsigned attrib;        // 文件属性的存储位置
            time_t time_create;
            time_t time_access;
            time_t time_write;
            _fsize_t size;
            char name[_MAX_FNAME];  // 文件名
        };
    */
    struct _finddata_t fileinfo;
    std::string p;
    // _findfirst遍历所有文件夹以及文件
    if ((hFile = _findfirst(p.assign(buff).append("\\music\\*.mp3").c_str(), &fileinfo)) != -1)   //_findfirst遍历所有文件夹以及文件
    {
        do
        {
            //如果是目录,迭代之
            //如果不是,加入列表
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    LoadMusic();              }
                else{
                    std::string str;
                    str = (p.assign(buff).append("\\music\\").append(fileinfo.name));
                    QString strQ = QString::fromLocal8Bit(str.c_str());
                    m_MyMusic.push_back(strQ);
                }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
    else {
        QDir *photo = new QDir;
        photo->mkdir("music");
    }


    return m_MyMusic.size();
}
void MyThread::threadPause()//暂停线程
{
    m_SubMusicState = MyThread::State::suspend;
    m_player->pause();
    m_SubMyLoveWindow->m_RestoreCover->stop();
    m_SubMyLoveWindow->writeThe_j(30);
    m_SubMyLoveWindow->m_RecoveryRod->stop();
    m_SubMyLoveWindow->update();
    this->m_mutex.lock();
}
void MyThread::threadResume()//继续线程
{
    m_SubMusicState = MyThread::State::conduct;
    m_player->play();
    if(!m_SubMyLoveWindow->m_RestoreCover->isActive())
    {
       m_SubMyLoveWindow->m_RestoreCover->start();
    }
    if(!m_SubMyLoveWindow->m_RecoveryRod->isActive())
    {
       m_SubMyLoveWindow->m_RecoveryRod->start();
    }
    this->m_mutex.unlock();
}
//关闭线程调用
void MyThread::closeThread()
{
    return;
}
void MyThread::UpdateMusic(qint64 duration)
{
    m_SubMusicTime = m_player->duration();

    int minute = int(m_SubMusicTime / 1000 / 60);
    int second = int(m_SubMusicTime / 1000 % 60);

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
    m_SubEndTime->setText(strQ);
    m_SubSlider->setRange(0,m_SubMusicTime);
    m_SubOverLyric->setText("");
    m_SubOnLyric->setText("");
    m_SubNextLyric->setText("");
    m_SubMyLoveWindow->writeThe_i(1);
    UpDataName();
    if(m_initialization)
    {
        emit intit(m_SubMusicTime);
        m_initialization = false;
    }
}
//开启线程调用
void MyThread::run()
{
    while (1)
    {
        //互斥锁
        m_mutex.lock();
        m_SubMusicTimeNow = m_player->position();
        m_SubSlider->setValue(m_SubMusicTimeNow);
        int minute = int(m_SubMusicTimeNow / 1000 / 60);
        int second = int(m_SubMusicTimeNow / 1000 % 60);

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
        m_SubBeginTime->setText(strQ);

        if(m_SubLyricsState == MyThread::OpenLyrics)
        {
            if(m_LyricMap.find(m_SubMusicName->text())!=m_LyricMap.end())
            {
                QMap<QString,QString> Lyric = m_LyricMap[m_SubMusicName->text()];

                //如果没有找到当前时间的歌词
                while(Lyric.find(strQ) == Lyric.end())
                {
                    if(second <= 0)
                    {
                        if(minute>0)
                        {
                            minute--;
                            second = 59;
                        }
                        else {
                            break;
                        }
                    }
                    else {
                        second--;
                    }

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
                    strQ = QString::fromLocal8Bit(Time.c_str());
                }

                if(Lyric.find(strQ) != Lyric.end())
                {
                    m_SubOnLyric->setText(Lyric[strQ]);
                    for(QMap<QString,QString>::iterator pr = Lyric.begin();pr != Lyric.end();++pr)
                    {
                        QMap<QString,QString>::iterator Dr;
                        if(pr.key() == strQ && pr != Lyric.begin())
                        {
                            m_SubOverLyric->setText((pr - 1).value());
                        }
                        if(pr.key() == strQ && (pr + 1) != Lyric.end())
                        {
                            m_SubNextLyric->setText((pr + 1).value());
                        }
                    }
                }
            }
        }
        else {
            m_SubOnLyric->setText(" ");
            m_SubOverLyric->setText(" ");
            m_SubNextLyric->setText(" ");
        }
        if(m_SubMusicTimeNow == m_SubMusicTime && m_player->mediaStatus() == QMediaPlayer::MediaStatus::EndOfMedia)
        {
            int i = m_MusicIndx;
            switch(m_SubPlayMod){
                //单曲循环
                case MyThread::MusicMode::STC:
                m_player->setMedia(QUrl::fromLocalFile(this->GetMusicByIndex(m_MusicIndx)));
                threadResume();
                sleep(1);
                continue;
                //列表循环
                case MyThread::MusicMode::LP:
                ++m_MusicIndx;
                if(m_MusicIndx >= m_MyMusic.size())
                    m_MusicIndx = 0;
                m_player->setMedia(QUrl::fromLocalFile(this->GetMusicByIndex(m_MusicIndx)));
                threadResume();
                sleep(1);
                continue;
                //随机播放
                case MyThread::MusicMode::PR:
                m_MusicIndx = rand() % (m_MyMusic.size());
                while(i == m_MusicIndx)
                    m_MusicIndx = rand() % (m_MyMusic.size());
                m_player->setMedia(QUrl::fromLocalFile(this->GetMusicByIndex(m_MusicIndx)));
                threadResume();
                sleep(1);
                continue;
                ;
            }
        }
        m_mutex.unlock();
        sleep(1);
    }
}

QString MyThread::GetMusicByIndex(const int&indx)
{
    QString str = "";
    if(indx < 0 || indx >= m_MyMusic.size())return str;
    return m_MyMusic.at(indx);
}

void MyThread::setPlayMod(int mod)
{
    m_SubPlayMod = mod;
}
void MyThread::SwitchSongs(bool State)
{
    if(m_MusicIndx >= m_MyMusic.size())
        m_MusicIndx = 0;
    if(State == MyThread::State::suspend)
    {
        m_player->setMedia(QUrl::fromLocalFile(this->GetMusicByIndex(m_MusicIndx)));
        threadResume();
    }
    else {
        threadPause();
        m_player->setMedia(QUrl::fromLocalFile(this->GetMusicByIndex(m_MusicIndx)));
        threadResume();
    }
}
void MyThread::UpDataName()
{
    if(m_MyMusic.size()>0)
    {
        QString Name = m_MyMusic[m_MusicIndx];
        std::string str = std::string((const char *)Name.toLocal8Bit());
        long  hFile = 0;
        struct _finddata_t fileinfo;
        std::string p;

        if ((hFile = _findfirst(p.assign(str).c_str(), &fileinfo)) != -1)
        {
            if (fileinfo.name)
            {
               std::string NAME = fileinfo.name;
               NAME = NAME.substr(0,NAME.find(".",0));
               QString name = QString::fromLocal8Bit(NAME.c_str());
               m_SubMusicName->setText(name);
               m_SubMyLoveWindow->setLove("MyLove/" + name);
            }
        }
       emit SwitchBackground(m_SubMusicName->text());
    }
}

bool MyThread::LoadLyric()
{
    QVector<QString> Iname;

    if(m_MyMusic.size()>0)
    {
        for(int i = 0; i < m_MyMusic.size();++i)
        {
            QString Name = m_MyMusic.at(i);

            std::string str = std::string((const char *)Name.toLocal8Bit());
            long  hFile = 0;
            struct _finddata_t fileinfo;
            std::string p;

            if ((hFile = _findfirst(p.assign(str).c_str(), &fileinfo)) != -1)
            {
                if (fileinfo.name)
                {
                    std::string NAME = fileinfo.name;
                    NAME = NAME.substr(0,NAME.find(".",0));
                    QString name = QString::fromLocal8Bit(NAME.c_str());
                    Iname.append(name);
                }
            }
        }
    }

    m_LyricMap.clear();
    for(int i = 0; i < Iname.size();++i)
    {
        QString FilePath= QApplication::applicationDirPath() +
                "/lyric/" + Iname.at(i) + ".xml";

        QFile XmlFile(FilePath);
        //使用只读的方式打开XML文件并且判断是否成功打开
        if(!XmlFile.open(QFile::ReadOnly))
        {
            continue;
        }

        // QDomDocument表示一个XML文档类
        //QDomDocument类表示整个XML文档。从概念上讲，它是文档树的根，并提供对文档数据的主要访问。
        QDomDocument docXML;


        if(!docXML.setContent(&XmlFile))
        {
             XmlFile.close();
             continue;
        }

        // docXML初始化成功  不在需要XmlFile
        XmlFile.close();
        //QdomeElement类表示DOM树中的一个元素。
        QDomElement xmlconfig = docXML.documentElement(); //documentElement 返回文档的根元素。

        QString XmlParameterList;
        QString Time;
        QString Data;

        QMap<QString,QString> LLyric;
        //Node节点
        for(QDomElement Format = xmlconfig.firstChildElement("Node");!Format.isNull();Format = Format.nextSiblingElement("Node"))
        {
            QString lyric = Format.toElement().attribute("lyric");
            QString Timer = Format.toElement().attribute("Timer");
            LLyric.insert(Timer,lyric);
        }
        m_LyricMap.insert(Iname.at(i),LLyric);
        LLyric.clear();
    }
    return true;
}
void MyThread::UpDataLyrics()
{
    int minute = int(m_SubMusicTimeNow / 1000 / 60);
    int second = int(m_SubMusicTimeNow / 1000 % 60);

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


    if(m_LyricMap.find(m_SubMusicName->text())!=m_LyricMap.end())
    {
        QMap<QString,QString> Lyric = m_LyricMap[m_SubMusicName->text()];
        //如果没有找到当前时间的歌词
        while(Lyric.find(strQ) == Lyric.end())
        {
            if(second <= 0)
            {
                if(minute>0)
                {
                    minute--;
                    second = 59;
                }
                else {
                    break;
                }
            }
            else {
                second--;
            }

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
            strQ = QString::fromLocal8Bit(Time.c_str());
        }

        if(Lyric.find(strQ) != Lyric.end())
        {
            m_SubOnLyric->setText(Lyric[strQ]);
            for(QMap<QString,QString>::iterator pr = Lyric.begin();pr != Lyric.end();++pr)
            {
                QMap<QString,QString>::iterator Dr;
                if(pr.key() == strQ && pr != Lyric.begin())
                {
                    m_SubOverLyric->setText((pr - 1).value());
                }
                if(pr.key() == strQ && (pr + 1) != Lyric.end())
                {
                    m_SubNextLyric->setText((pr + 1).value());
                }
            }
        }
    }
}

std::vector<QString>MyThread::GetMusiNameList()
{
    std::vector<QString> MusiNameList;
    for(int i = 0;i < m_MyMusic.size();i++)
    {
        QString Name = m_MyMusic.at(i);
        std::string str = std::string((const char *)Name.toLocal8Bit());
        long  hFile = 0;
        struct _finddata_t fileinfo;
        std::string p;

        if ((hFile = _findfirst(p.assign(str).c_str(), &fileinfo)) != -1)
        {
            if (fileinfo.name)
            {
               std::string NAME = fileinfo.name;
               NAME = NAME.substr(0,NAME.find(".",0));
               QString name = QString::fromLocal8Bit(NAME.c_str());
               MusiNameList.push_back(name);
            }
        }
    }
    return MusiNameList;
}
