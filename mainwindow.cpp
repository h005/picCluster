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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_clicked()
{
    label->open();
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
