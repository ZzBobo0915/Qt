#include "ForYou/cachethread.h"
#include<QDir>
#include<QApplication>
#include<QPixmap>
Cachethread::Cachethread(const QString ResourcePath,vector<QString>ResourceName,QWidget* window)
{
    m_MainWindow    =   window;
    m_ResourcePath  =   ResourcePath;
    m_ResourceName  =   ResourceName;
    m_blur  = new GaussianBlur(3,0.84089642f);
    m_blur->setBlurRadius(90);
    m_blur->setSigma(90);
    CreateCacheFile();
}

Cachethread::~Cachethread()
{

}

//开启线程调用
void Cachethread::run()
{
    for(int i = 0;i < m_ResourceName.size();++i)
    {
        QPixmap pixmap;
        QString BackgroundPath;
        BackgroundPath  =   m_ResourcePath    +   "/"     +   m_ResourceName.at(i) + ".png";

        QFileInfo file(QApplication::applicationDirPath()    +   "/Cache"     +
                       "/"     +   m_ResourceName.at(i)    +   ".png");
        if(file.exists())
        {
            continue;
        }

        pixmap.load(BackgroundPath);
        QSize   picSize(m_MainWindow->width()*1.2,m_MainWindow->height()*1.2);
        QPixmap scaledPixmap = pixmap.scaled(picSize);
        QPixmap scaledPixmap2 = scaledPixmap.copy(m_MainWindow->pos().x()+30,
                                                  m_MainWindow->pos().y()+30,
                                                  m_MainWindow->width(),m_MainWindow->height());
        QImage Image = scaledPixmap2.toImage();
        QImage img = m_blur->BlurImage(Image);
        BackgroundPath  =   QApplication::applicationDirPath()    +   "/Cache"     +
                "/"     +   m_ResourceName.at(i)    +   ".png";
        img.save(BackgroundPath);
        m_MainWindow->update();
    }
}

void Cachethread::CacheResourceFile(const QString& ResourceName)
{

}

QImage Cachethread::GetGaussImageByName(const QString&ImageName)
{

}

void Cachethread::CreateCacheFile()
{
    QDir dir;
    dir.setPath(QApplication::applicationDirPath());
    if(!dir.exists("Cache")){
        dir.mkdir("Cache");
    }
    m_CacheFile = dir.path()    +   "/Cache";
}
