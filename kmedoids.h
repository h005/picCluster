#ifndef KMEDOIDS_H
#define KMEDOIDS_H

#include <opencv.hpp>
#include <iostream>
#include "mvDis_matlab.h"

#define ITTIMES 10000

/*
 *  创建这个类主要是为了解决kmeans中距离不能平均的bug
 *
 *  kmedoids 函数使用的是|| expm( logm(A*B^-1) ) || 来
 *  度量距离，并且使用的是类内距离最大最小化来确定下一个聚类中心
 *
 *  kmedoidsPAM 使用的是PAM算法来确定下一个聚类中心
 *
 */

class Kmedoids
{
public:
    Kmedoids();

    Kmedoids(cv::Mat &fea);

    void kmedoids(std::vector<int> &elements,
                  int k,
                  cv::Mat &label,
                  cv::Mat &centers);

    void kmedoidsPAM(std::vector<int> &elements,
                     int k,
                     cv::Mat &label,
                     cv::Mat &centers);

private:

    bool centersChanged(std::vector<int> centersId);

    void initialCenters(std::vector<int> &elements,
                        int k,
                        std::vector<int> &centersId);

    void resetlabel(std::vector<int> &elements,
                    std::vector<int> &centersId,
                    cv::Mat &label);

    void resetCentersId(std::vector<int> &centersId,
                        std::vector<int> &elements,
                        cv::Mat &label);

    void resetCentersIdPAM(std::vector<int> &centersId,
                           std::vector<int> &elements,
                           cv::Mat &label);

    cv::Mat fea;

    std::vector< std::vector<double> > disGraph;
    int itTimes;
    std::vector<int> lastCentersId;
    // absolute error
    std::vector<double> minMax;
    std::vector<double> minMaxId;

};

#endif // KMEDOIDS_H
