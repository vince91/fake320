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
#include <QCloseEvent>
#include <thread>

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
    treeView->setColumnWidth(2, 150);
    treeView->setSortingEnabled(true);
    treeView->hideColumn(0);
    
    window->setLayout(grid);
    setCentralWidget(window);
    
    connect(openButton, SIGNAL(clicked()), this, SLOT(openFolder()));
    connect(recursiveBox, SIGNAL(stateChanged(int)), this, SLOT(checkRecursive(int)));
    connect(analysisButton, SIGNAL(clicked()), this, SLOT(analysis()));
    
    QDesktopWidget dw;
    resize(0.5*dw.width(), 0.5*dw.height());
}

MainWindow::~MainWindow()
{    
    closeThread();
    if (th != nullptr) {
        if (th->joinable())
            th->join();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    closeThread();
    event->accept();
}

void MainWindow::openFolder()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("Select a directory"), QDir::currentPath());
    
    if (!dirname.isNull())
    {
        closeThread();
        if (th != nullptr) {
            if (th->joinable())
                th->join();
        }

        model->removeRows(0, model->rowCount());
        
        library = new Mp3Library(dirname.toStdString(), recursive);
        library->fillList();
        
        for (int i = 0; i < library->getListSize(); ++i) {
            std::string s = library->getFilename(i);
            s = s.substr(s.find_last_of("/") + 1, s.size() - 1);





            QList<QStandardItem *> list;
            list.append(new QStandardItem(QString::number(i)));
            list.append(new QStandardItem(s.c_str()));

            for (QList<QStandardItem *>::iterator it = list.begin(); it != list.end(); ++it) {
                (*it)->setFlags((*it)->flags() & ~Qt::ItemIsEditable);
            }
            
            model->appendRow(list);
        }
        
        if (library->getListSize() > 0) {
            analysesCount = 0;
            analysisButton->setText("Start MP3 analysis");
            analysisButton->setEnabled(true);
        }
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
        if (th != nullptr) {
            if (th->joinable())
                th->join();

        }
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
        
        QStandardItem *item;
        int realRow = -1;
        
        /* seek the real index, if user sorted the columns */
        for (int i = 0; i < model->rowCount() && realRow == -1; ++i) {
            item = model->item(i, 0);
            int temp = item->data(Qt::DisplayRole).toInt();
            if (temp == analysesCount)
                realRow = i;
        }
        
        QList<QStandardItem*> list = model->takeRow(realRow);
        list.removeLast(); list.removeLast();
        list.append(new QStandardItem(freq));
        list.append(new QStandardItem(QString::number(rate)));
        
        for (QList<QStandardItem*>::iterator it = list.begin(); it != list.end(); ++it) {
            (*it)->setBackground(QBrush(color));
        }
        
        model->insertRow(realRow, list);
        
        if (++analysesCount == library->getListSize()) {
            analysisButton->setEnabled(false);
            analysisButton->setText("Start MP3 analysis");
            closeThread();
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
