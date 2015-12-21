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
    ui->labelstate->setText("state: view");
    modeMap.insert(std::pair<QString,int>("KmedoidsPAM-mvMatrix",0));
    modeMap.insert(std::pair<QString,int>("KmedoidsZwz-mvMatrix",1));
    modeMap.insert(std::pair<QString,int>("kmeans-centroid_SaliencyArea",2));
    modeMap.insert(std::pair<QString,int>("kmeans-cameraDistanceDirection",3));

    this->setFocusPolicy(Qt::StrongFocus);
    flagViewMode = true;

    connect(label,SIGNAL(vfModeChanged(bool)),
            this,SLOT(vfModeChanged(bool)));

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Q)
    {
        viewModeChanged();
        label->setViewMode(this->flagViewMode);
    }
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
    label->savePics();
}

void MainWindow::on_load_clicked()
{
    label->load();
    treeWidget = label->treeWidget;
    ui->treeScroll->setWidget(treeWidget);
    connect(treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            label,SLOT(setCurItem()));
}

void MainWindow::vfModeChanged(bool vfMode)
{
    flagViewMode = vfMode;
    if(flagViewMode)
        ui->labelstate->setText("state: view");
    else
        ui->labelstate->setText("state: choose");
}

void MainWindow::viewModeChanged()
{
//    flagViewMode = label->getViewMode();
    if(flagViewMode)
    {
        flagViewMode = false;
        ui->labelstate->setText("state: choose");
    }
    else
    {
        flagViewMode = true;
        ui->labelstate->setText("state: view");
    }
}
