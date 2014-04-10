//
//  main.cpp
//  Fake320
//
//  Created by Vincent Timofti on 28/02/2014.
//  Copyright (c) 2014 Vincent Timofti. All rights reserved.
//


#include <QApplication>
#include <QTextCodec>
#include <iostream>
#include <thread>
#include "mainwindow.h"
#include "mp3file.h"
#include "mp3library.h"

using namespace std;

int main(int argc, char * argv[])
{
    /*/
    Mp3File mp3("e:/Desktop/MARS/ok.mp3");
    
    mp3.decodeAndAnalyze();
    mp3.coutInformations();
     //*/

    /*/
    Mp3Library library("e:/Desktop/MARS", false);
    
    //Mp3Library library("/Users/vincent/Music/MARS/Will and Ink - Wilson [VINYL]", false);
    library.fillList();
    //library.analyzeMp3(0);
     //*/
    
    //*/

    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    MainWindow w;
    w.show();
    
    return a.exec();
     //*/
}

