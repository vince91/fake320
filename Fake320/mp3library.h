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
#include "mp3file.h"

class Mp3Library
{
public:
    Mp3Library(std::string _folder, bool _recursive) : folder(_folder), recursive(_recursive) {}
    bool analyzeFolder();
    bool fillList(std::string, bool);
    
    
private:
    bool recursive;
    std::string folder;
    std::list<std::string> list;
};

#endif /* defined(__Fake320__mp3library__) */
