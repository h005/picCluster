#ifndef IMGLABEL_H
#define IMGLABEL_H

#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QPointF>
#include <QStringList>
#include <iostream>
#include <vector>
#include <opencv.hpp>
#include <QPointF>
#include <QDateTime>
#include <qDebug>
#include "cluster.h"

class ImgLabel : public QLabel
{
    Q_OBJECT
public:
    ImgLabel(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvnet(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);

    void open();

    void save();

private:
    // read images into imgs
    void readin();
    // set fea
    void setfea();
    // get centroid
    void getCentroid(int index,double &col,double &row);
    // get saliency area
    void getSaliencyArea(int index,double &area);

    void setKmeans(int k);

    void setCateCenterLabel(int f1,int f2);

    void drawCenter(QPainter painter);

    void initialClusters();

    QPointF shift(int i,QPointF &p);

    QPointF shift(QPointF &p);

    QImage mat2Qimage(cv::Mat &mat);

    QString getRgbPath(QString path);

    QString getFilename(QString path);
private:

    QImage image;
    QString rgbPath;
    int NUMCluster;
    QString path;
    QStringList filelist;
    std::vector<cv::Mat> imgs;
    std::vector<cv::Mat> scImgs;
    cv::Mat fea;
    cv::Mat label;
    cv::Mat centers;
    int imw,imh;
    int labelw,labelh;
    // cate centers in label
    std::vector<QPointF> cateCenterLabel;
    std::vector<Cluster> clusters;

    // cols rows in Qlabel
    int f1,f2;

};

#endif // IMGLABEL_H
