#include "mainwindow.h"
#include "mp3library.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QString>
#include <QFileDialog>
#include <QHeaderView>
#include <thread>
#include <functional>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Fake320");
    
    QWidget *window = new QWidget;
    QGridLayout *grid = new QGridLayout;
    
    openButton = new QPushButton("Open folder");
    analysisButton = new QPushButton("Start MP3 analysis");
    analysisButton->setEnabled(false);
    recursiveBox = new QCheckBox("Recursive");
    treeView = new QTreeView;
    
    grid->addWidget(openButton, 0, 0);
    grid->addWidget(recursiveBox, 1, 0);
    grid->addWidget(analysisButton, 0, 1, 1, 1);
    grid->addWidget(treeView, 2, 0, 2, 2);
    
    model2 = new QStandardItemModel(0, 4);
    model2->setHorizontalHeaderItem(1, new QStandardItem("Filename"));
    model2->setHorizontalHeaderItem(2, new QStandardItem("Cut-off frequency"));
    model2->setHorizontalHeaderItem(3, new QStandardItem("Rate"));
    
    treeView->setModel(model2);
    treeView->header()->setStretchLastSection(false);
    treeView->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    treeView->hideColumn(0);
    
    window->setLayout(grid);
    setCentralWidget(window);
    
    connect(openButton, SIGNAL(clicked()), this, SLOT(openFolder()));
    connect(recursiveBox, SIGNAL(stateChanged(int)), this, SLOT(checkRecursive(int)));
    connect(analysisButton, SIGNAL(clicked()), this, SLOT(analysis()));
    
    QDesktopWidget dw;
    resize(0.7*dw.width(), 0.5*dw.height());
}

MainWindow::~MainWindow()
{
    if (th != nullptr) {
        if (th->joinable())
            th->join();
    }
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
            //model->addItem(i, s.substr(s.find_last_of("/") + 1, s.size() - 1));
            QList<QStandardItem *> list;
            list.append(new QStandardItem(i));
            list.append(new QStandardItem(s.substr(s.find_last_of("/") + 1, s.size() - 1).c_str()));
            
            for (QList<QStandardItem *>::iterator it = list.begin(); it != list.end(); ++it) {
                (*it)->setFlags((*it)->flags() & ~Qt::ItemIsEditable);
            }
            
            model2->appendRow(list);

        }
        
        analysisButton->setEnabled(true);
        
    }
    
}

void MainWindow::checkRecursive(int check)
{
    recursive = check ? true : false;
}

void MainWindow::analysis()
{
    if (!analyzing) {
        analysisButton->setText("Stop analysis");
        analyzing = true;
        th = new std::thread(&MainWindow::analysisThread, this);
    }
    else {
        analysisButton->setEnabled(false);
        analysisButton->setText("Resume analysis");
        analyzing = false;
    }
    
    
    
}

void MainWindow::analysisThread()
{
    std::cout << "Analysis number: " << analysesCount << std::endl;
    
    while (analyzing)
    {
        library->analyzeMp3(analysesCount);
        
        QList<QStandardItem*> list = model2->takeRow(analysesCount);
        list.removeLast(); list.removeLast();
        list.append(new QStandardItem(QString::number(library->getCutOffFrequency(analysesCount))));
        list.append(new QStandardItem(QString::number(library->getRate(analysesCount))));
        model2->insertRow(analysesCount, list);
        
        //model->updateItem(analysesCount, library->getCutOffFrequency(analysesCount), library->getRate(analysesCount));
        //treeView->update();
        
        if (++analysesCount == library->getListSize()) {
            analysisButton->setEnabled(false);
            analysisButton->setText("Start analysis");
            analyzing = false;
        }
    }
    
    if (analysesCount != library->getListSize())
        analysisButton->setEnabled(true);

    
    return;
}
