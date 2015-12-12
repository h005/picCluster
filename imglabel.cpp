#include "imglabel.h"

ImgLabel::ImgLabel(QWidget *parent):
    QLabel(parent)
{
    image = QImage();
    imw = 100;
    imh = 100;
    NUMCluster = 50;
}

void ImgLabel::mousePressEvent(QMouseEvent *e)
{

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

}

void ImgLabel::open()
{
    filelist = QFileDialog::getOpenFileNames(this,
               tr("Open Image"),".",
               tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));

    // set path
    path = filelist.at(0);
    int pos = path.lastIndexOf('/');
    path = path.remove(pos,path.length() - pos);
    qDebug() << "read in path " << path <<endl;
    readin();
    qDebug()<<"read in done"<< endl;
    setfea();
    qDebug() << "setfea done" << endl;
    setKmeans(NUMCluster);
    qDebug() << "kmeans done" << endl;
    initialClusters();
    qDebug() << "clusters done " << endl;
//    labelw = imw * cate * 5;
//    labelh = imh * cate * 5;
    // factor of cate
    double f0 = sqrt((double)NUMCluster);

    f1 = f0,f2 = f0;

    while(f1 * f2 < NUMCluster)
        f1++;

    // set label width and height
    labelw = imw * f1 * 5;
    labelh = imh * f2 * 5;

    setCateCenterLabel(f1,f2);
    QPixmap tmp(labelw,labelh);
    tmp.fill(Qt::white);
    this->setPixmap(tmp);
    qDebug()<<"setCateCenterLabel done"<<endl;
    update();
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

//        int pos_,pos__;
//        QString tmp = filelist.at(i);
//        pos_ = tmp.lastIndexOf('/');
//        tmp = tmp.replace(pos_,1,'_');
//        pos__ = tmp.lastIndexOf('/');
//        tmp = tmp.replace(pos__,1,'_');
//        pos__ = tmp.lastIndexOf('/');
//        tmp = tmp.remove(pos__+1,pos_ - pos__);
        cv::Mat tmpscImg = cv::imread(tmp.toStdString().c_str());
//        qDebug() << rgbPath.append(filelist.at(i)) << endl;
        double ratiow,ratioh;
        ratiow = (double)imw / tmpscImg.cols;
        ratioh = (double)imh / tmpscImg.rows;
        double ratio = ratiow > ratioh ? ratioh : ratiow;
        cv::resize(tmpscImg,
                   tmpscImg,
                   cv::Size((int)(ratio * tmpscImg.cols + 0.5),(int)(ratio * tmpscImg.rows + 0.5)));
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
    int wstep = imw * 5;
    int hstep = imh * 5;
    QPointF ini(wstep / 2,hstep / 2);
    for(int i=0;i<f1;i++)
        for(int j=0;j<f2;j++)
        {
            QPointF tmp = ini + QPointF(i * wstep, j * hstep);
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
        Cluster tmp(fea,
                    tmpcenter,
                    label,
                    i,
                    2.0 * imw,
                    2.0 * imh);
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
//    folder = path + "/" + folder;
    dir.mkdir(folder);

    // make folders named by the id of cluster
    // and copy the file in to them

    for(int i=0;i<NUMCluster;i++)
        dir.mkdir(folder + "/" + QString::number(i));

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
    cv::cvtColor(mat,mat,CV_BGR2RGB);
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
    QString res = path.mid(pos + 1);
    return res;
}




