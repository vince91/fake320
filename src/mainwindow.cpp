#include "mainwindow.h"
#include "mp3library.h"
#include <iostream>
#include <QWidget>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QString>
#include <QFileDialog>
#include <QHeaderView>
#include <QLabel>
#include <QColor>
#include <thread>
#include <functional>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("fake320");
    
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
    grid->addWidget(treeView, 2, 0, 1, 2);
    
    model = new QStandardItemModel(0, 4);
    model->setHorizontalHeaderItem(1, new QStandardItem("Filename"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Cut-off frequency"));
    model->setHorizontalHeaderItem(3, new QStandardItem("Rate"));
    
    treeView->setModel(model);
    treeView->header()->setStretchLastSection(false);
    treeView->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    treeView->hideColumn(0);
    
    window->setLayout(grid);
    setCentralWidget(window);
    
    connect(openButton, SIGNAL(clicked()), this, SLOT(openFolder()));
    connect(recursiveBox, SIGNAL(stateChanged(int)), this, SLOT(checkRecursive(int)));
    connect(analysisButton, SIGNAL(clicked()), this, SLOT(analysis()));
    //connect(this, SIGNAL(closedEvent()), this, SLOT(closeThread()));
    
    
    
    QDesktopWidget dw;
    resize(0.7*dw.width(), 0.5*dw.height());
}

MainWindow::~MainWindow()
{
    if (th != nullptr) {
        if (th->joinable())
            th->join();
    }
    
    analyzing = false;
}

void MainWindow::openFolder()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Select a directory"), QDir::currentPath());
    

    if (!dirname.isNull())
    {


        model->removeRows(0, model->rowCount());


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
            
            model->appendRow(list);

        }
        
        if (library->getListSize() > 0)
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
        analysisButton->setText("Stop MP3 analysis");
        analyzing = true;
        th = new std::thread(&MainWindow::analysisThread, this);
    }
    else {
        analysisButton->setEnabled(false);
        analysisButton->setText("Resume MP3 analysis");
        analyzing = false;
    }
    
    
    
}

void MainWindow::analysisThread()
{
    std::cout << "Analysis number: " << analysesCount << std::endl;
    
    while (analyzing)
    {
        library->analyzeMp3(analysesCount);
        
        int cutOffFrequency = library->getCutOffFrequency(analysesCount);
        double rate = library->getRate(analysesCount);
        
        QColor color("white");
        
        if (cutOffFrequency < 18000) {
            if (rate < 8)
                color = QColor("orange");
            else
                color = QColor("red");
        }
        
        QString freq = QString::number(cutOffFrequency);
        if (cutOffFrequency == 20000)
            freq  += "+";
            
        QList<QStandardItem*> list = model->takeRow(analysesCount);
        list.removeLast(); list.removeLast();
        list.append(new QStandardItem(freq));
        list.append(new QStandardItem(QString::number(rate)));
        
        for (QList<QStandardItem*>::iterator it = list.begin(); it != list.end(); ++it) {
            (*it)->setBackground(QBrush(color));
        }
        
        model->insertRow(analysesCount, list);
        
        
        if (++analysesCount == library->getListSize()) {
            analysisButton->setEnabled(false);
            analysisButton->setText("Start MP3 analysis");
            analyzing = false;
        }
    }
    
    if (analysesCount != library->getListSize())
        analysisButton->setEnabled(true);

    
    return;
}

void MainWindow::closeThread()
{
    analyzing = false;
}
