#include "treecluster.h"

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
                         std::vector<float> center)
{
    this->num = num;
    this->numc = (int)sqrt((double)num);
    this->fname = fname + "_" + QString::number(this->numc);
    this->picID = picID;
    child = NULL;
    next = NULL;
    this->center = center;
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
