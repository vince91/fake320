#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QCheckBox>
#include <QTreeView>
#include "mp3library.h"
#include <thread>
#include <QStandardItemModel>

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
    QPushButton *openButton;
    QPushButton *analysisButton;
    QCheckBox *recursiveBox;
    QTreeView *treeView;
    QStandardItemModel *model2;
    
    int analysesCount = 0;
    
    Mp3Library * library;
    
    std::thread *th;
    
    bool recursive = false;
    bool analyzing = false;

};



#endif // MAINWINDOW_H
