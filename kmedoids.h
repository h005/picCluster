#ifndef KMEDOIDS_H
#define KMEDOIDS_H

#include <opencv.hpp>
#include <iostream>
#include "mvDis_matlab.h"

#define ITTIMES 10000

class Kmedoids
{
public:
    Kmedoids();

    Kmedoids(cv::Mat &fea);

    void kmedoids(std::vector<int> &elements,
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

    cv::Mat fea;

    std::vector< std::vector<double> > disGraph;
    int itTimes;
    std::vector<int> lastCentersId;
    // absolute error
    std::vector<double> minMax;
    std::vector<double> minMaxId;

};

#endif // KMEDOIDS_H
