#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QCheckBox>
#include <QTreeView>
#include "mp3library.h"
#include "itemmodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void openFolder();
    void checkRecursive(int);

private:
    QPushButton *openButton;
    QPushButton *analyzeButton;
    QCheckBox *recursiveBox;
    QTreeView *treeView;
    ItemModel *model;
    
    
    Mp3Library * library;
    bool recursive;
    

    
};



#endif // MAINWINDOW_H
