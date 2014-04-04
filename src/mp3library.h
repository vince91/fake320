//
//  mp3library.h
//  Fake320
//
//  Created by Vincent Timofti on 16/03/2014.
//  Copyright (c) 2014 Vincent Timofti. All rights reserved.
//

#ifndef __Fake320__mp3library__
#define __Fake320__mp3library__

#include <iostream>
#include <list>
#include <vector>
#include "mp3file.h"

class Mp3Library
{    
public:
    Mp3Library(std::string _folder, bool _recursive) : folder(_folder), recursive(_recursive) {}
    void fillList();
    bool analyzeMp3(int);
    int getListSize() { return (int) mp3List.size(); }
    std::string getFilename(int);
    int getCutOffFrequency(int);
    double getRate(int);
    
private:
    std::string folder;
    bool recursive;
    std::list<std::string> list;
    std::vector<Mp3File*> mp3List;
    
    bool seekMp3(std::string, bool);

};

#endif /* defined(__Fake320__mp3library__) */
