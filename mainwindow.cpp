#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    treeWidget = new QTreeWidget();
    label = new ImgLabel(treeWidget);
    ui->scrollArea->setWidget(label);
    ui->scrollArea->resize(600,600);
//    ui->treeScroll->setWidget(treeWidget);
    ui->open->setShortcut(Qt::Key_O);
    ui->save->setShortcut(Qt::Key_S);
    ui->load->setShortcut(Qt::Key_L);
    modeMap.insert(std::pair<QString,int>("KmedoidsPAM-mvMatrix",0));
    modeMap.insert(std::pair<QString,int>("KmedoidsZwz-mvMatrix",1));
    modeMap.insert(std::pair<QString,int>("kmeans-centroid_SaliencyArea",2));
    modeMap.insert(std::pair<QString,int>("kmeans-cameraDistanceDirection",3));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_clicked()
{
    QString mode = ui->comboBox->currentText();
    std::cout << mode.toStdString() << std::endl;
    label->open(mode,modeMap[mode]);
    treeWidget = label->treeWidget;
    ui->treeScroll->setWidget(treeWidget);
    connect(treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                               label,SLOT(setCurItem()));
}

void MainWindow::on_save_clicked()
{
//    label->save();
}

void MainWindow::on_load_clicked()
{
    label->load();
    treeWidget = label->treeWidget;
    ui->treeScroll->setWidget(treeWidget);
    connect(treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                               label,SLOT(setCurItem()));
}
