#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H

/*
  TODO: 实现高斯模糊
  Author: ZzBoAYU
  Date: 2023.05.07
  原理参考: https://blog.csdn.net/mahabharata_/article/details/69066942
*/

#include <vector>
#include "qcolor.h"
#include "qimage.h"

using std::vector;

class GaussianBlur
{

public:
    // 构造，初始化并调用CreateConvolutionMatrix()
    GaussianBlur(int blurRadius, float sigma);
    // 高斯模糊图片
    QImage BlurImage(const QImage& in);
    // 高斯模糊函数
    float GaussFunc(float x);
    // 获得模糊半径
    int getBlurRadius() const;
    void setBlurRadius(int value);
    // 获得标准差
    float getSigma() const;
    void setSigma(float value);

    ~GaussianBlur();

private:

    vector<float> mConvolutionMatrix;
    // 边界处理函数，将矩阵中超出部分用内部点来代替
    int ReflectIndex(int x, int length);
    // 创建权值控件
    void CreateConvolutionMatrix();

    int mBlurRadius;  // 模糊卷积半径
    float mSigma;     // 标准差σ
};

#endif // GAUSSIANBLUR_H
