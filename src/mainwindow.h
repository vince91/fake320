#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QCheckBox>
#include <QTreeView>
#include <QStandardItemModel>
#include <thread>
#include "mp3library.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void analysisThread();
    
private slots:
    void openFolder();
    void checkRecursive(int);
    void analysis();

private:
    void closeEvent(QCloseEvent *);
    void closeThread();
    
    QPushButton *openButton;
    QPushButton *analysisButton;
    QCheckBox *recursiveBox;
    QTreeView *treeView;
    QStandardItemModel *model;
    
    int analysesCount = 0;
    
    Mp3Library * library;
    
    std::thread *th;
    
    bool recursive = false;
    bool analyzing = false;

};



#endif // MAINWINDOW_H
