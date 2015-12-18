#ifndef KMEANS_H
#define KMEANS_H

#include <opencv.hpp>
#include <iostream>
#include "mvDis_matlab.h"


#define ITTIMES 10000

/*
 * 写这个类主要是为了处理自定义的距离聚类，但是有一个问题
 *
 * 这些特征是否可以平均？
 *
 * 在MATLAB中expm(logm(A)) A矩阵的特征值需要均为正实数
 *
 * 得到的聚类中心是否还会满足这个条件？
 *
 */

class Kmeans
{
public:
    Kmeans();
    void kmeans(cv::Mat &fea,int clusters,cv::Mat &label,cv::Mat &center);

private:
    // set label
    void setLabel(cv::Mat &fea,cv::Mat &center,cv::Mat &label);
    // reset Centers
    void setCenters(cv::Mat &fea,cv::Mat &label,cv::Mat &centers);
    // initial centers
    void initialCenters(cv::Mat &fea, cv::Mat &center);
    // cluster center won't changed
    bool centerChanged(cv::Mat &center);
    // compute distance between two case
    double getDis(cv::Mat &fea,int ind1,cv::Mat &center,int ind2);

    // last cluster center
    cv::Mat lastCenter;
    int itTimes;

};

#endif // KMEANS_H
