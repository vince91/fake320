//
//  mp3library.h
//  Fake320
//
//  Created by Vincent Timofti on 16/03/2014.
//  Copyright (c) 2014 Vincent Timofti. All rights reserved.
//

#ifndef MP3LIBRARY_H
#define MP3LIBRARY_H

#include <iostream>
#include <list>
#include <vector>
#include <QString>

#include "mp3file.h"

class Mp3Library
{    
public:
    void fillList();
    bool analyzeMp3(int);
    
    int getListSize() const { return (int) mp3List.size(); }
    std::string getFilename(int) const;
    int getCutOffFrequency(int) const;
    double getRate(int) const;
    void update(QString, bool);
    
private:
    std::string folder;
    bool recursive;
    
    std::list<QString> list;
    std::vector<Mp3File*> mp3List;
    
    bool seekMp3();
};

#endif // MP3LIBRARY_H
