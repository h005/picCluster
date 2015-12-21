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

    void keyPressEvent(QKeyEvent *e);

    ~MainWindow();

private slots:
    void on_open_clicked();

    void on_save_clicked();

    void on_load_clicked();

    void vfModeChanged(bool vfMode);

private:

    void viewModeChanged();

    Ui::MainWindow *ui;
    ImgLabel *label;
    QTreeWidget *treeWidget;
    std::map<QString,int> modeMap;
    /*
     *  the flag of view and choose
     *
     * in the choose mode
     * click the treeWidget will show all the pics in this cluster
     * when click the pic a rounding box will appear
     * when there is a rounding box press delete will
     * delete this pic
     *
     * press ctrl + z can redo the delete operate
     *
     * in the view mode
     * the same as before
     *
     * flagViewMode true for view mode
     * false for false mode
     *
     */
    bool flagViewMode;

};

#endif // MAINWINDOW_H
