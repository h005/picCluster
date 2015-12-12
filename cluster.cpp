#include "cluster.h"

Cluster::Cluster()
{

}

Cluster::Cluster(cv::Mat &fea,
                 std::vector<float> center,
                 cv::Mat &label,
                 int id,
                 float wstep,
                 float hstep)
{
    cateId = id;
    this->center = center;
    for(int i=0;i<label.rows;i++)
        if(label.at<int>(i,0) == id)
            numImgs.push_back(i);
    double pi = asin(1.0) * 2.0;
//    qDebug()<< "pi" << pi << endl;
    double theta = 2.0 * pi / numImgs.size();
    double rmax =  wstep >  hstep ?  hstep :  wstep;
    setCoord(fea,theta,rmax);
}

void Cluster::setCoord(cv::Mat &fea,double theta, double rmax)
{
    // distance to center
    std::vector<double> dis;
    double max = 0.0;
    for(int i=0;i<numImgs.size();i++)
    {
        double tmp = 0.0;
        for(int j=0;j < center.size();j++)
        {
            tmp += (fea.at<float>(numImgs[i],j) - center[j])
                    * (fea.at<float>(numImgs[i],j) - center[j]);
            max = max > tmp ? max : tmp;
        }
        dis.push_back(tmp);
    }
    // compute x,y coordinate of center of image
    for(int i=0;i<numImgs.size();i++)
    {
        double r = 0.0;
        if(max)
            r = rmax * dis[i] / max;
        else
            r = 0;
        double x = r * sin(theta * i);
        double y = r * cos(theta * i);
        coordImgs.push_back(QPointF(x,y));
    }


}

