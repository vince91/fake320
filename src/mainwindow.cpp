#include "mainwindow.h"
#include "mp3library.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QString>
#include <QFileDialog>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Fake320");
    recursive = false;
    
    QWidget *window = new QWidget;
    QGridLayout *grid = new QGridLayout;
    
    openButton = new QPushButton("Open folder");
    analyzeButton = new QPushButton("Analyze");
    recursiveBox = new QCheckBox("Recursive");
    treeView = new QTreeView;
    
    grid->addWidget(openButton, 0, 0);
    grid->addWidget(recursiveBox, 1, 0);
    grid->addWidget(analyzeButton, 0, 1, 1, 1);
    grid->addWidget(treeView, 2, 0, 2, 2);
    
    model = new ItemModel();
    treeView->setModel(model);
    treeView->header()->setStretchLastSection(false);
    treeView->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    treeView->hideColumn(0);
    
    window->setLayout(grid);
    setCentralWidget(window);
    
    connect(openButton, SIGNAL(clicked()), this, SLOT(openFolder()));
    connect(recursiveBox, SIGNAL(stateChanged(int)), this, SLOT(checkRecursive(int)));
    
    QDesktopWidget dw;
    resize(0.7*dw.width(), 0.5*dw.height());
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::openFolder()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Select a directory"), QDir::currentPath());
    
    if (!dirname.isNull())
    {
        library = new Mp3Library(dirname.toStdString(), recursive);
        library->fillList();
        
        for (int i = 0; i < library->getListSize(); ++i) {
            std::string s = library->getFilename(i);
            model->addItem(i, s.substr(s.find_last_of("/") + 1, s.size() - 1));
        }
        
    }
    
}

void MainWindow::checkRecursive(int check)
{
    recursive = check ? true : false;
}
