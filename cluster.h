#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <opencv.hpp>
#include <QPointF>
#include <QtDebug>

class Cluster
{
public:
    Cluster();
    Cluster(cv::Mat &fea,
            std::vector<float> center,
            cv::Mat &label,
            int id,
            float wstep,
            float hstep);

    Cluster(cv::Mat &label,
            int id);

    std::vector<QPointF> coordImgs;
    std::vector<int> numImgs;


private:
    void setCoord(cv::Mat &fea,
                  double theta,
                  double rmax);

private:
    std::vector<float> center;

    int cateId;

};

#endif // CLUSTER_H
