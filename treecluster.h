#ifndef TREECLUSTER_H
#define TREECLUSTER_H

#include <iostream>
#include <QString>
#include <vector>
#include <math.h>

class TreeCluster
{
public:
    TreeCluster();

    TreeCluster(QString fname,
                int numc,
                int picID,
                std::vector<float> center);

    void appendChild(TreeCluster *p);

    TreeCluster* get(int i);

public:
    // folder name or file name
    // include parent's path, it should be substring
    // from last of '/' when is used
    // such as 0_2/1_3/2
    QString fname;
    // for folder num means number of cluseters
    // for file num means number of files
    int numc,picID,num;
    TreeCluster *child; // to child
    TreeCluster *next; // to brother
    std::vector<float> center;
};

#endif // TREECLUSTER_H
