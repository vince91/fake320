//
//  mp3library.cpp
//  Fake320
//
//  Created by Vincent Timofti on 16/03/2014.
//  Copyright (c) 2014 Vincent Timofti. All rights reserved.
//

#include "mp3library.h"
#include <dirent.h>
#include <errno.h>
#include <vector>


bool Mp3Library::analyzeFolder()
{
    /* analyses all 320kbps mp3s contained in list */
    fillList(folder, recursive);
    
    std::cout << list.size() << std::endl;
    
    for (std::list<std::string>::iterator it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << std::endl;
    }
    
    return true;
}

bool Mp3Library::fillList(std::string _folder, bool recursive)
{
    /* adds all files contained in folder (recursively or not), whose extension is ".mp3", to list */
    
    DIR *d = NULL;
    struct dirent *dir;
    std::string file;
    std::vector<std::string> folderList;
    
    if ((d = opendir(_folder.c_str())) == NULL) {
        std::cerr << "Couldn't open dir: " << _folder << ", errno: " << errno << std::endl;
        return false;
    }
    
    while ((dir = readdir(d)) != NULL) {
        
        file = _folder + "/" + dir->d_name;
        
        if (dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
            folderList.push_back(file);
        }
        else if (dir->d_type == DT_REG) {
            if(file.substr(file.size()-4, file.size()-1).compare(".mp3") == 0)
            list.push_back(file);
        }
        
    }
    
    closedir(d);
    
    if (recursive) {
        for (std::vector<std::string>::iterator it = folderList.begin(); it != folderList.end(); ++it) {
            fillList(*it, true);
        }
    }
    
    return true;
}


