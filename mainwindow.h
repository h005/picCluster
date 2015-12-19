#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <iostream>
#include <map>
#include <QString>
#include "imglabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open_clicked();

    void on_save_clicked();

    void on_load_clicked();

private:
    Ui::MainWindow *ui;
    ImgLabel *label;
    QTreeWidget *treeWidget;
    std::map<QString,int> modeMap;
};

#endif // MAINWINDOW_H
