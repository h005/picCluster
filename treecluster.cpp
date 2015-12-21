#include "treecluster.h"
#include <algorithm>
TreeCluster::TreeCluster()
{
    fname = "";
    numc = -1;
    num = -1;
    picID = -1;
    child = NULL;
    next = NULL;
}



TreeCluster::TreeCluster(QString fname,
                         int num,
                         int picID,
                         std::vector<float> center,
                         std::vector<int> imgs)
{
    this->num = num;
    this->numc = (int)sqrt((double)num);
    this->fname = fname + "_" + QString::number(this->numc);
    this->picID = picID;
    child = NULL;
    next = NULL;
    this->center = center;
    this->imgs = imgs;
}

void TreeCluster::appendChild(TreeCluster *p)
{
    TreeCluster *q = this->child;
    if(!q)
    {
        this->child = p;
        return;
    }
    else
    {
        while(q->next)
            q = q->next;
        q->next = p;
        return;
    }

}

TreeCluster *TreeCluster::get(int index)
{
    TreeCluster *p = this->child;
    for(int i=0;i<index;i++)
    {
        if(!p)
            return NULL;
        p = p->next;
    }
    return p;
}

int TreeCluster::deletePic(int pic)
{
    for(int i=0;i<imgs.size();i++)
    {
        if(imgs[i] == pic)
        {
            imgs.erase(imgs.begin() + i);
            garbge.push(pic);
            this->num--;
            this->numc = (int)sqrt((double)num);
            return 1;
        }
    }
    return -1;
}

int TreeCluster::recoveryPic(int pic)
{
    std::cout << "recovery Pic before " << pic << std::endl;
    if(garbge.empty())
        return -1;
    if(garbge.top() != pic)
        return -1;
    std::cout << "recovery Pic " << pic << std::endl;
    garbge.pop();
    imgs.push_back(pic);
    std::sort(imgs.begin(),imgs.end(),std::less<int>());
    std::cout << "recovery ... ";
    for(int i=0;i<imgs.size();i++)
        std::cout << imgs[i] << " ";
    std::cout << std::endl;
    this->num++;
    this->numc = (int)sqrt((double)num);
    return 1;
}


