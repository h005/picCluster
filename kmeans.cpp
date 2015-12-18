#include "kmeans.h"

Kmeans::Kmeans()
{
    lastCenter = cv::Mat();
    itTimes = 0;
    assert(mvDis_matlabInitialize());
}

void Kmeans::kmeans(cv::Mat &fea,
                    int clusters,
                    cv::Mat &label,
                    cv::Mat &center)
{
//    std::cout << "call kmeans function" << std::endl;
    itTimes = 0;
//    std::cout << "fea rows " << fea.rows << " cluster " << clusters << std::endl;
    assert(fea.rows > clusters);
//    std::cout << "kmeans function before center release" << std::endl;
    center.release();
//    std::cout << "kmeans function center release done" << std::endl;
    lastCenter = cv::Mat(clusters,fea.cols,CV_64F,cv::Scalar(0.0));
    center = cv::Mat(clusters,fea.cols,CV_64F);
    label = cv::Mat(fea.rows,1,CV_32S,cv::Scalar(0));
//    std::cout << "kmeans function" << std::endl;
    // initial centers
    initialCenters(fea,center);
//    std::cout << "fea size " << fea.rows << " " << fea.cols << std::endl;
//    for(int i=0;i<center.rows;i++)
//    {
//        for(int j=0;j<center.cols;j++)
//        {
//            std::cout << center.at<double>(i,j) << " ";
//        }
//        std::cout << std::endl;
//    }

//    std::cout << "initial centers done"<< std::endl;
    while(!centerChanged(center))
    {
//        std::cout << "center check done" << std::endl;
        // set label
        setLabel(fea,center,label);
        std::cout << "setlabel done" << std::endl;
        // set centers
        setCenters(fea,label,center);
        std::cout << "setCenters done" << std::endl;
    }
}

void Kmeans::setLabel(cv::Mat &fea, cv::Mat &center, cv::Mat &label)
{
    double dis = 1e30;
    double tmpdis = 0;
    int minCate = 0;
    for(int i=0;i<fea.rows;i++)
    {
        for(int j=0;j<center.rows;j++)
        {
            tmpdis = getDis(fea,i,center,j);
            if(dis > tmpdis)
            {
                dis = tmpdis;
                minCate = j;
            }
//            std::cout << "feature row " << i << " center row " << j << " tmpdis " << tmpdis << std::endl;
        }
        label.at<int>(i,0) = minCate;
    }
}

void Kmeans::setCenters(cv::Mat &fea, cv::Mat &label, cv::Mat &centers)
{
    centers.copyTo(lastCenter);
    double *counter = new double[label.rows];
    for(int i=0;i<label.rows;i++)
        counter[i] = 0.0;

    for(int i=0;i<centers.rows;i++)
        for(int j=0;j<centers.cols;j++)
            centers.at<double>(i,j) = 0.0;

    for(int i=0;i<fea.rows;i++)
    {
        for(int j=0;j<fea.cols;j++)
            centers.at<double>(label.at<int>(i,0),j) += fea.at<double>(i,j);
        counter[label.at<int>(i,0)] ++;
    }

    for(int i=0;i<centers.rows;i++)
        for(int j=0;j<centers.cols;j++)
            centers.at<double>(i,j) /= counter[i];

}

void Kmeans::initialCenters(cv::Mat &fea, cv::Mat &center)
{
    int step = fea.rows / center.rows;
    int ini = rand() % step;
    for(int i=0;i<center.rows;i++)
        for(int j=0;j<fea.cols;j++)
            center.at<double>(i,j) = fea.at<double>(ini + step*i,j);
}

bool Kmeans::centerChanged(cv::Mat &center)
{
    if(itTimes > ITTIMES)
        return true;
    else
    {
        for(int i=0;i<center.rows;i++)
        {
            for(int j=0;j<center.cols;j++)
            {
                if(center.at<double>(i,j) != lastCenter.at<double>(i,j))
                    return false;
            }
        }
    }
    return true;
}

double Kmeans::getDis(cv::Mat &fea, int ind1, cv::Mat &center, int ind2)
{
//    std::cout << "mwArray start" << std::endl;
    mwArray in1(4,4,mxDOUBLE_CLASS);
//    std::cout << "mwArray done" << std::endl;
    mwArray in2(4,4,mxDOUBLE_CLASS);
    // set data
    double *data = new double[16];
//    std::cout << "get Dis 1" << std::endl;
    for(int i=0;i<fea.cols;i++)
        data[i] = fea.at<double>(ind1,i);
    // column first
    // 1 2 3 4
    // ==>
    // 1 3
    // 2 4
    // so mv matrix to fea should be row first

    /*
     * mv matrix in matrix file
     * a1 a2
     * b1 b2
     * in glm
     * a1 b1
     * a2 b2
     * in fea should be
     * a1 b1 a2 b2
     * in mwArray
     * a1 a2
     * b1 b2
     */

    in1.SetData(data,16);
    for(int i=0;i<fea.cols;i++)
        data[i] = center.at<double>(ind2,i);
    in2.SetData(data,16);
//    std::cout << "getDis set data done" << std::endl;
    mwArray dis;
    mvDis(1,dis,in1,in2);
//    std::cout << "mvdis done" << std::endl;
    assert(!dis.IsComplex());
//    std::cout << (double)dis << std::endl;
    return (double)dis;
}

