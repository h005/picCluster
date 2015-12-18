#include "kmedoids.h"

Kmedoids::Kmedoids()
{

}

Kmedoids::Kmedoids(cv::Mat &fea)
{
    assert(mvDis_matlabInitialize());
    this->fea = fea;
    minMax.clear();
    minMaxId.clear();
    mwArray in1(4,4,mxDOUBLE_CLASS);
    mwArray in2(4,4,mxDOUBLE_CLASS);
    mwArray out;
    double *data = new double[16];
    for(int i=0;i<fea.rows;i++)
    {
        for(int k=0;k<fea.cols;k++)
            data[k] = fea.at<double>(i,k);
        in1.SetData(data,16);
        std::vector<double> tmp;
        for(int j=0;j<fea.rows;j++)
        {
            for(int k=0;k<fea.cols;k++)
                data[k] = fea.at<double>(j,k);
            in2.SetData(data,16);
            mvDis(1,out,in1,in2);
            tmp.push_back((double)out);
//            std::cout << i <<" " << j << " " << (double)out << std::endl;
        }
        disGraph.push_back(tmp);
    }

}

void Kmedoids::kmedoids(std::vector<int> &elements,
                        int k,
                        cv::Mat &label,
                        cv::Mat &centers)
{
    std::cout << "elements size " << elements.size()<< std::endl;
    std::cout << "clusters k " << k << std::endl;
    std::cout << "kmedoids function" << std::endl;
    lastCentersId.clear();
    for(int i=0;i<k;i++)
        lastCentersId.push_back(0);
    std::cout << "kmedoids elements size " << elements.size() << " clusters " << k << std::endl;
    assert(elements.size() > k);
    std::vector<int> centersId;
    label = cv::Mat(elements.size(),1,CV_32S,cv::Scalar(0));
    centers = cv::Mat(k,fea.cols,CV_64F,cv::Scalar(0));
    std::cout << "kmedoids initial Centers before" << std::endl;
    // centers id in elements
    // the absolute element is elements[centers[id]]
    initialCenters(elements,k,centersId);

    for(int i=0;i<centersId.size();i++)
        std::cout << "centersId " << i << " " << centersId[i] << std::endl;

    while(centersChanged(centersId))
    {
        std::cout << "resetlabel before" << std::endl;
        resetlabel(elements,centersId,label);
        for(int i=0;i<label.rows;i++)
        {
            std::cout << "label " << i << " " << label.at<int>(i,0) << std::endl;
        }
        std::cout << "resetlabel done resetCenterId before" << std::endl;
        resetCentersId(centersId,elements,label);
        std::cout << "resetCenterId done" << std::endl;
    }

}

bool Kmedoids::centersChanged(std::vector<int> centersId)
{
    if(itTimes > ITTIMES)
        return false;
    for(int i=0;i<centersId.size();i++)
        if(centersId[i] != lastCentersId[i])
            return true;
    return false;
}

void Kmedoids::initialCenters(std::vector<int> &elements,
                              int k,
                              std::vector<int> &centersId)
{
    int step = elements.size() / k;
    int ini = rand() % step;
//    int ini = 2;
    centersId.clear();
    for(int i=0;i<k;i++)
        centersId.push_back(ini + i*step);
}

void Kmedoids::resetlabel(std::vector<int> &elements,
                          std::vector<int> &centersId,
                          cv::Mat &label)
{
    double dis;
    int minid;
    for(int i=0;i<elements.size();i++)
    {
        dis = disGraph[elements[i]][elements[centersId[0]]];
        minid = 0;
        for(int j=1;j<centersId.size();j++)
        {
            if(dis > disGraph[elements[i]][elements[centersId[j]]])
            {
                dis = disGraph[elements[i]][elements[centersId[j]]];
                minid = j;
            }
        }
        label.at<int>(i,0) = minid;
//        std::cout << "resetlabel " << i << " " << minid << std::endl;
    }
}

void Kmedoids::resetCentersId(std::vector<int> &centersId,
                              std::vector<int> &elements,
                              cv::Mat &label)
{
    lastCentersId = centersId;
    // for each centers
    for(int i=0;i<centersId.size();i++)
    {
        std::vector<double> vecDis;
        std::vector<int> vecIndices;
        // for each item in this cluster find the max distance
        for(int j=0;j<elements.size();j++)
        {
            if(label.at<int>(j,0) == i)
            {
                double dis = disGraph[elements[j]][elements[j]];
                for(int k=0;k<elements.size();k++)
                {
                    if(label.at<int>(k,0) == i)
                    {
                        if(dis < disGraph[elements[j]][elements[k]])
                            dis = disGraph[elements[j]][elements[k]];
                    }
                }
                vecDis.push_back(dis);
                vecIndices.push_back(j);
            }
        }

        double minDis = vecDis[0];
        int minIndex = vecIndices[0];
        for(int j=1;j<vecDis.size();j++)
        {
            if(minDis > vecDis[j])
            {
                minDis = vecDis[j];
                minIndex = vecIndices[j];
            }
        }
        centersId[i] = minIndex;
    }
}



