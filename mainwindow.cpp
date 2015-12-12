#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label = new ImgLabel();
    ui->scrollArea->setWidget(label);
    ui->scrollArea->resize(600,600);
    ui->open->setShortcut(Qt::Key_O);
    ui->save->setShortcut(Qt::Key_S);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_clicked()
{
    label->open();
}

void MainWindow::on_save_clicked()
{
    label->save();
}
