#ifndef CACHETHREAD_H
#define CACHETHREAD_H

/*
  TODO: 创建一个子线程缓存背景图片
  Author: ZzBoAYU
  Date: 2023.05.07
*/

#include <QThread>
#include "ForYou/gaussianblur.h"
#include <QDialog>
#include <vector>

class Cachethread : public QThread
{
    Q_OBJECT
public:
    Cachethread(const QString ResourcePath,std::vector<QString>ResourceName,QWidget* window);
    ~Cachethread();
protected:
    //线程函数
    virtual void run();
public:
    void CacheResourceFile(const QString& ResourceName);
    QImage GetGaussImageByName(const QString&ImageName);
    QString GetCacheFilePath(){return m_CacheFile;}
private:
    void CreateCacheFile();
private:
    //高斯模糊
    GaussianBlur*    m_blur;
    //缓存文件路径
    QString          m_CacheFile;
    //文件路径
    QString          m_ResourcePath;
    //资源名称
    std::vector<QString>m_ResourceName;
    //主窗口对象
    QWidget*         m_MainWindow;
};

#endif // CACHETHREAD_H
