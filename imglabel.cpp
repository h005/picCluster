#include "imglabel.h"

ImgLabel::ImgLabel(QTreeWidget *treeWidget, QWidget *parent):
    QLabel(parent)
{
    this->treeWidget = treeWidget;
    image = QImage();
    imw = 100;
    imh = 100;
    NUMCluster = 2;
    tc = new TreeCluster();
    curItem = NULL;
}

void ImgLabel::mousePressEvent(QMouseEvent *e)
{

#ifdef NETMAP

#else
    QPoint pos = e->pos();
    int cluster = pos.x() / imw + pos.y() / imh * f1;
    // click the white area will map to a cluster larger then NUMCluster
    // will lead to error
#ifdef TREECLUSTER
    if(cluster >= cur->numc)
        return;
    QString tmpPath;
    if(cur->numc > 1)
        tmpPath = path + '/' + cur->get(cluster)->fname;
    else
        tmpPath = path + '/' + cur->fname;
#else

#endif

    std::cout << "mouse press event " <<cluster<< " " << tmpPath.toStdString() << std::endl;
    tmpPath.replace("/","\\"); //将地址中的"/"替换为"\"，因为在Windows下使用的是"\"。
    QProcess::startDetached("explorer "+tmpPath);

#endif

}

void ImgLabel::mouseReleaseEvnet(QMouseEvent *e)
{

}

void ImgLabel::paintEvent(QPaintEvent *e)
{

    QLabel::paintEvent(e);
    QPainter *painter = new QPainter(this);
    QPen pen = QPen(Qt::red,2);
    painter->setPen(pen);
#ifdef NETMAP
    if(cateCenterLabel.size())
    {
        for(int i=0;i<NUMCluster;i++)
        {
            painter->drawEllipse(cateCenterLabel[i],5,5);
            for(int j=0;j<clusters[i].coordImgs.size();j++)
            painter->drawLine(cateCenterLabel[i],shift(i,clusters[i].coordImgs[j]));
        }
        for(int i=0; i < NUMCluster;i++)
        {
            for(int j=0;j < clusters[i].numImgs.size();j++)
            {
                QImage tmp = mat2Qimage(scImgs[clusters[i].numImgs[j]]);
                painter->drawImage(shift(i,clusters[i].coordImgs[j]),tmp);
            }
        }
    }
#else
    std::cout << "cateCenterLabel size " << cateCenterLabel.size() << std::endl;
    if(cateCenterLabel.size())
    {
#ifdef TREECLUSTER
        // TreeCluster 中如果只有一个元素，那么这个TreeCluster则没有child
        // so it is
        if(cur->numc != 1)
            for(int i=0; i < cur->numc; i++)
            {
                QImage tmp = mat2Qimage(scImgs[cur->get(i)->picID]);
                painter->drawImage(shift(cateCenterLabel[i]),tmp);
            }
        else
        {
            for(int i=0; i < cur->num; i++)
            {
                QImage tmp = mat2Qimage(scImgs[cur->imgs[i]]);
                painter->drawImage(shift(cateCenterLabel[i]),tmp);
            }

        }
#else
        for(int i=0; i < NUMCluster; i++)
        {
            QImage tmp = mat2Qimage(scImgs[clusters[i].numImgs[0]]);
            painter->drawImage(shift(cateCenterLabel[i]),tmp);
        }
#endif
    }

#endif

}

void ImgLabel::open()
{
    filelist = QFileDialog::getOpenFileNames(this,
               tr("Open Image"),".",
               tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));

    std::cout << "filelist size " << filelist.size() << std::endl;

    // set path
    path = filelist.at(0);
    int pos = path.lastIndexOf('/');
    path = path.remove(pos,path.length() - pos);
    std::cout << "read in path" << std::endl;
    readin();
    std::cout << "read in done"<< std::endl;
    setfea();
    std::cout << "setfea done" << std::endl;
#ifdef TREECLUSTER
    std::vector<float> tmpcenter;
    for(int i=0;i<fea.cols;i++)
        tmpcenter.push_back(fea.at<float>(0,i));
    QDateTime time = QDateTime::currentDateTime();
    QString folder = time.toString("yyyyMMddhhmm");
    std::vector<int> elements;
    for(int i=0;i<filelist.size();i++)
        elements.push_back(i);
    // not absolute path just a folder name
    tc = new TreeCluster(folder,
                         filelist.size(),
                         0,
                         tmpcenter,
                         elements);
    pos = path.lastIndexOf('/');
    QString pathC = path.remove(pos,path.length() - pos);
    dir = QDir(pathC);
//    std::cout << "kmeans before" << std::endl;
    recursiveKmeans(tc,elements);
    std::cout << "kmeans done" << std::endl;
    initialTreeWidget();
    std::cout << "tree widget done" << std::endl;

    // factor of cate
    factorOfCate(tc->numc);
    setCateCenterLabel(f1,f2);

    QPixmap tmp(labelw,labelh);
    tmp.fill(Qt::white);
    this->setPixmap(tmp);
    cur = tc;
    std::cout << "tc numc " << tc->numc << std::endl;
    update();
#else
#ifdef NETMAP
    setKmeans(NUMCluster);
    std::cout << "kmeans done"<< std::endl;
    initialClusters();
    std::cout << "clusters done " << std::endl;
    // factor of cate
    double f0 = sqrt((double)filelist.size());
    //    double f0 = sqrt((double)tc->num);

    f1 = f0,f2 = f0;

    while(f1 * f2 < filelist.size())
        f1++;

    labelw = imw * f1 * 5;
    labelh = imh * f2 * 5;

    setCateCenterLabel(f1,f2);
    QPixmap tmp(labelw,labelh);
    tmp.fill(Qt::white);
    this->setPixmap(tmp);
    std::cout << "setCateCenterLabel done " << std::endl;

    save();
    update();

#else
    setKmeans(NUMCluster);
    std::cout << "kmeans done"<< std::endl;
    initialClusters();
    std::cout << "clusters done " << std::endl;
    // factor of cate
    double f0 = sqrt((double)filelist.size());
    //    double f0 = sqrt((double)tc->num);

    f1 = f0,f2 = f0;

    while(f1 * f2 < filelist.size())
        f1++;

    std::cout << "f1 " << f1 << std::endl;
    std::cout << "f2 " << f2 << std::endl;

    setCateCenterLabel(f1,f2);
    qDebug()<<"setCateCenterLabel done"<<endl;
    QPixmap tmp(labelw,labelh);
    tmp.fill(Qt::white);
    this->setPixmap(tmp);
    save();
    update();

#endif
#endif

}

void ImgLabel::readin()
{
    imgs.clear();
    for(int i=0;i<filelist.size();i++)
    {
//        qDebug() << filelist.at(i) << endl;
        cv::Mat tmpimg = cv::imread(filelist.at(i).toStdString().c_str(),0);
        imgs.push_back(tmpimg);

        QString tmp = getRgbPath(filelist.at(i));

        cv::Mat tmpscImg = cv::imread(tmp.toStdString().c_str());
//        qDebug() << rgbPath.append(filelist.at(i)) << endl;
        double ratiow,ratioh;
        ratiow = (double)imw / tmpscImg.cols;
        ratioh = (double)imh / tmpscImg.rows;
        double ratio = ratiow > ratioh ? ratioh : ratiow;
        cv::resize(tmpscImg,
                   tmpscImg,
                   cv::Size((int)(ratio * tmpscImg.cols + 0.5),(int)(ratio * tmpscImg.rows + 0.5)));
        cv::cvtColor(tmpscImg,tmpscImg,CV_BGR2RGB);
        scImgs.push_back(tmpscImg);

    }
}

void ImgLabel::setfea()
{
    std::vector< std::vector<double> > feavec;
    for(int i=0;i<filelist.size();i++)
    {
        std::vector<double> featmp;
        double col,row;
        double area;
        getCentroid(i,col,row);
        getSaliencyArea(i,area);
        featmp.push_back(col);
        featmp.push_back(row);
        featmp.push_back(area);
        feavec.push_back(featmp);
    }
    // set fea
    fea = cv::Mat(feavec.size(),feavec[0].size(),CV_32F);
    for(int i=0;i<fea.rows;i++)
        for(int j=0;j<fea.cols;j++)
            fea.at<float>(i,j) = feavec[i][j];
}

void ImgLabel::getCentroid(int index,double &col,double &row)
{
    col = 0.0;
    row = 0.0;
    double count = 0.0;
//    cv::namedWindow("img");
//    cv::imshow("img",imgs[index]);
    Q_ASSERT( imgs[index].cols != 0);
    Q_ASSERT( imgs[index].rows != 0);
    for(int j=0;j<imgs[index].cols;j++)
    {
        for(int i=0;i<imgs[index].rows;i++)
        {
            if(imgs[index].at<uchar>(i,j))
            {
                count ++;
                col += j;
                row += i;
            }
        }
    }
    if(!count)
    {
        col = -1;
        row = -1;
        return;
    }
    col /= count;
    row /= count;
    col /= imgs[index].cols;
    row /= imgs[index].rows;
}

void ImgLabel::getSaliencyArea(int index, double &area)
{
    area = 0.0;
    for(int j=0;j<imgs[index].cols;j++)
        for(int i=0;i<imgs[index].rows;i++)
            if(imgs[index].at<uchar>(i,j))
                area ++;
    area /= imgs[index].rows;
    area /= imgs[index].cols;
}

void ImgLabel::setKmeans(int k)
{
//    qDebug() << k << endl;
    cv::kmeans(fea,k,label,
               cv::TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,10,1.0),
               3,cv::KMEANS_PP_CENTERS,centers);

}



void ImgLabel::setCateCenterLabel(int f1,int f2)
{
#ifdef NETMAP
    int wstep = imw * 5;
    int hstep = imh * 5;
#else
    int wstep = imw;
    int hstep = imh;
#endif
    QPointF ini(wstep / 2,hstep / 2);
    for(int i=0;i<f2;i++)
        for(int j=0;j<f1;j++)
        {
            QPointF tmp = ini + QPointF(j * wstep, i * hstep);
            cateCenterLabel.push_back(tmp);
        }
}

void ImgLabel::drawCenter(QPainter painter)
{
    //    painter.drawPoints(cateCenterLabel,cate);
}

void ImgLabel::initialClusters()
{
    for(int i=0;i<NUMCluster;i++)
    {
        std::vector<float> tmpcenter;
        for(int j=0; j < fea.cols ; j++)
            tmpcenter.push_back(centers.at<float>(i,j));
#ifdef NETMAP
        Cluster tmp(fea,
                    tmpcenter,
                    label,
                    i,
                    2.0 * imw,
                    2.0 * imh);
#else
        Cluster tmp(label, i);
#endif
        clusters.push_back(tmp);
    }
}

void ImgLabel::save()
{
    // save cluster results in current dir
    // and make a folder named time_NUMCLUSTERS to store the result
    // int this folder make folders named by the id of cluster
    // contains the cooresponding image

    // make a folder named time_NUMCLUSTERS
    int pos = path.lastIndexOf('/');
    QString pathC = path.remove(pos,path.length() - pos);
    QDir dir(pathC);
    QDateTime time = QDateTime::currentDateTime();
    QString folder = time.toString("yyyyMMddhhmm");
    folder = folder + "_" + QString::number(NUMCluster);
//    qDebug()<< folder << endl;
    std::cout << "save... " << path.toStdString() << std::endl;
//    folder = path + "/" + folder;
    dir.mkdir(folder);
    clusterFolder.clear();
    // make folders named by the id of cluster
    // and copy the file in to them

    for(int i=0;i<NUMCluster;i++)
    {
        dir.mkdir(folder + "/" + QString::number(i));
        clusterFolder.push_back(folder + "/" + QString::number(i));
    }

    for(int i=0;i<filelist.size();i++)
    {
        QString rgbFile = getRgbPath(filelist.at(i));
        QFile file(rgbFile);
        QString filename = getFilename(filelist.at(i));
        file.link(pathC + "/" + folder + "/" + QString::number(label.at<int>(i,0)) + "/" + filename + ".lnk");
//        qDebug() << "link" << path + "/" + folder + "/" + QString::number(label.at<int>(i,0)) + filename << endl;
//        break;
    }

}

void ImgLabel::setCurItem()
{
    curItem = treeWidget->currentItem();
    cur = icmap[curItem];
    update();
}

QPointF ImgLabel::shift(int id, QPointF &p)
{
    QPointF res;
//    res = p + QPointF(imw / 2.0,imh / 2.0);
//    res = p;
//    res = res + QPointF(imw * 5 / 2,imh * 5 / 2);
//    res = res + QPointF((id % f1) * imw * 5,(id / f1) * imh * 5);
    res = p + cateCenterLabel[id];
    return res;
}

QPointF ImgLabel::shift(QPointF &p)
{
    return p - QPointF(imw / 2.0,imh / 2.0);
}

QImage ImgLabel::mat2Qimage(cv::Mat &mat)
{
    QImage img((uchar*)mat.data,
               mat.cols,mat.rows,
               mat.cols * mat.channels(),
               QImage::Format_RGB888);
    return img;
}

QString ImgLabel::getRgbPath(QString path)
{
    // the last pos of / and the last next next pos of /
    QString tmp = path;
    int pos_ = tmp.lastIndexOf('/');
    tmp = tmp.replace(pos_,1,'_');
    int pos__ = tmp.lastIndexOf('/');
    tmp = tmp.replace(pos__,1,'_');
    pos__ = tmp.lastIndexOf('/');
    tmp = tmp.remove(pos__+1,pos_ - pos__);
    return tmp;
}

QString ImgLabel::getFilename(QString path)
{
    int pos = path.lastIndexOf('/');
    // equals substring
    QString res = path.mid(pos + 1);
    return res;
}

int ImgLabel::getNUMCluster(int num)
{
    return (int)sqrt((double)num);
}

bool ImgLabel::allsame(std::vector<int> &elements)
{
    for(unsigned int i=0;i<elements.size();i++)
    {
        for(int j=1;j<fea.cols;j++)
        {
            if(fea.at<float>(elements[i],j)
                != fea.at<float>(0,j))
                return false;
        }
    }
    return true;
}

void ImgLabel::copyfilesLink(TreeCluster *root,
                             std::vector<int> &elements)
{
    for(unsigned int i=0;i<elements.size();i++)
    {
        QString rgbFile = getRgbPath(filelist.at(elements[i]));
        QFile file(rgbFile);
        QString filename = getFilename(filelist.at(elements[i]));
        file.link(dir.path() + "/" + root->fname + "/" + filename + ".lnk");
    }
}

void ImgLabel::initialTreeWidget()
{
    treeWidget = new QTreeWidget();
    // filename numClusters numPics
    treeWidget->setColumnCount(3);
    QTreeWidgetItem *head = new QTreeWidgetItem((QTreeWidget*)0,(QStringList() << "cluster" << "numCluster" << "size"));
    treeWidget->setHeaderItem(head);
    QTreeWidgetItem *top = new QTreeWidgetItem((QStringList() << "root" << QString::number(tc->numc) << QString::number(tc->num) ));
    treeWidget->addTopLevelItem(top);
    TreeCluster *root = tc;
    genTreeWidgetItem(top,root);
    icmap.insert(std::pair<QTreeWidgetItem*, TreeCluster*>(top,root));
}

void ImgLabel::genTreeWidgetItem(QTreeWidgetItem *item,
                                 TreeCluster *root)
{
    TreeCluster *p = root->child;
    while(p)
    {
        QStringList tmp;
        int pos = p->fname.lastIndexOf('/');
        QString tmpss = p->fname;
        tmpss = tmpss.mid(pos+1,p->fname.length() - pos);
        tmp.append(tmpss);
        tmp.append(QString::number(p->numc));
        tmp.append(QString::number(p->num));
        QTreeWidgetItem *child = new QTreeWidgetItem(tmp);
        item->addChild(child);
        genTreeWidgetItem(child,p);
        icmap.insert(std::pair<QTreeWidgetItem*, TreeCluster*>(child,p));
        p = p->next;
    }
}

void ImgLabel::factorOfCate(int cate)
{
    double f0 = sqrt((double)cate);
    f1 = f0, f2 = f0;
    while(f1 * f2 < cate)
        f1++;
//    std::cout << "f1 " << f1 << "f2 " << f2 << endl;
}

void ImgLabel::setClusterFolder(TreeCluster *root)
{
    clusterFolder.clear();
    TreeCluster *p = root->child;
    while(p)
    {
        clusterFolder.push_back(p->fname);
        p = p->next;
    }
}


void ImgLabel::recursiveKmeans(TreeCluster *root,
                               std::vector<int> &elements)
{
//    std::cout << "root " << root->fname.toStdString() << std::endl;
    dir.mkdir(root->fname);
    copyfilesLink(root,elements);
    if(elements.size() < 4)
        return;
    // wheather all elements ara the same
    if(allsame(elements))
        return;

    cv::Mat feature = cv::Mat(elements.size(),fea.cols,CV_32F);
    cv::Mat tmplabel;
    cv::Mat tmpcenters;

    // load feature
    for(unsigned int i=0;i<elements.size();i++)
    {
        for(int j=0;j<fea.cols;j++)
            feature.at<float>(i,j) = fea.at<float>(elements[i],j);
    }
//    std::cout << "load feature done" << std::endl;
    cv::kmeans(feature,root->numc,tmplabel,
               cv::TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,10,1.0),
               3,cv::KMEANS_PP_CENTERS,tmpcenters);

//    std::cout << "kmeans done" << std::endl;

    std::vector< std::vector<float> > tmpvecCenters;
    std::vector< std::vector<int> > cElements;
    // initial
    for(int i=0;i<root->numc;i++)
    {
        std::vector<int> tmp;
        cElements.push_back(tmp);
    }
    for(unsigned int i=0;i<elements.size();i++)
        cElements[tmplabel.at<int>(i,0)].push_back(elements[i]);

    for(int i=0;i<tmpcenters.rows;i++)
    {
        std::vector<float> tmp;
        for(int j=0;j<tmpcenters.cols;j++)
            tmp.push_back(tmpcenters.at<float>(i,j));
        tmpvecCenters.push_back(tmp);
    }

    for(int i=0;i<root->numc;i++)
    {
        TreeCluster *child = new TreeCluster(root->fname + "/" + QString::number(i),
                                               cElements[i].size(),
                                               cElements[i][0],
                                               tmpvecCenters[i],
                                               cElements[i]);
//        std::cout << "child " << child->fname.toStdString() << std::endl;
        root->appendChild(child);
        recursiveKmeans(child,cElements[i]);
    }
}




