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
#include <QtDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QProcess>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "cluster.h"
#include "mode.h"
#include "treecluster.h"
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include "kmeans.h"
#include "kmedoids.h"
#include "vcm.h"

class ImgLabel : public QLabel
{
    Q_OBJECT
public:
    ImgLabel(QTreeWidget *treeWidget,QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvnet(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);

    void open(QString modeString,int mode);

    void save();

    void savePics();

    void load();

    void setViewMode(bool flagViewMode);

    bool getViewMode();

signals:
    void vfModeChanged(bool flag);

public slots:
    void setCurItem();

private:
    // read images into imgs
    void readin();
    // set fea
    void setfea();
    // get centroid
    void getCentroid(int index,double &col,double &row);
    // get saliency area
    void getSaliencyArea(int index,double &area);
    // get camera distance
    void getCameraDis(int index,double &dis);
    // get camera direction
    void getCameraDir(int index,glm::vec4 &dirZ, glm::vec4 &dirY);

    void setKmeans(int k);

    void setCateCenterLabel(int f1,int f2);

    void drawCenter(QPainter painter);

    void initialClusters();

    QPointF shift(int i,QPointF &p);

    QPointF shift(QPointF &p);

    QImage mat2Qimage(cv::Mat &mat);

    QString getRgbPath(QString path);

    QString getFilename(QString path);

    int getNUMCluster(int num);

    void recursiveKmeans(TreeCluster *root,
                         std::vector<int> &elements);

    void recursiveLoad(TreeCluster *root,
                       QString path,
                       QStringList folderList,
                       QStringList fnameList);

    // check wheather all the elemnets are the same
    bool allsame(std::vector<int> &elements);

    void recursiveSave();

    void recursiveSave(TreeCluster *p, QDir &dir, QString &folder);

    void copyfilesLink(TreeCluster *root,
                       std::vector<int> &elements);

    void initialTreeWidget();

    void genTreeWidgetItem(QTreeWidgetItem *treeWidget,
                           TreeCluster *roots);

    void factorOfCate(int cate);

    void setClusterFolder(TreeCluster *root);

    // set filelist
    // set
    void readMatrixFile(QString file);

    void getFileFolderList(QString path,
                           QStringList &folder,
                           QStringList &file);

    void setLabelSize(int num);

    void deletePic(int num);



public:
    QTreeWidget *treeWidget;
private:

//    Kmeans *kmeans;

    Kmedoids *kmedoids;

    std::vector<glm::mat4> p_mv;
    std::vector<glm::mat4> p_proj;

    QImage image;
    QString rgbPath;
    QString savePath;
    int NUMCluster;
    QString path;
    QStringList filelist;
    std::vector<QString> clusterFolder;
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
    TreeCluster *tc,*cur;
    QTreeWidgetItem *curItem;
    // item cluster map
    std::map<QTreeWidgetItem* , TreeCluster* > icmap;
    QDir dir;
    // pic index map
    std::map<QString , int > pimap;
//    QString modeString;
    int mode;
    VCM vcm;
};

#endif // IMGLABEL_H
