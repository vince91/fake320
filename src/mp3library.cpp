//
//  mp3library.cpp
//  Fake320
//
//  Created by Vincent Timofti on 16/03/2014.
//  Copyright (c) 2014 Vincent Timofti. All rights reserved.
//

#include "mp3library.h"
#include "mp3file.h"
#include <dirent.h>
#include <errno.h>
#include <vector>

extern "C"
{
#include <libavformat/avformat.h>
}

void Mp3Library::fillList()
{
    /* analyses all 320kbps mp3s contained in list */
    AVFormatContext *formatContext = NULL; AVCodecContext *codecContext = NULL;
    bool keep;
    
    seekMp3(folder, recursive);
    
    av_register_all();
    
    std::cout << list.size() << " files in the list before cleaning\n\n";
    
    for (std::list<std::string>::iterator it = list.begin(); it != list.end(); ++it) {
        formatContext = NULL;
        keep = false;
        
        //std::cout << *it << std::endl;
        
        /* open input file, and allocate format context */
        if (avformat_open_input(&formatContext, it->c_str(), NULL, NULL) < 0) {
            std::cerr << "Could not open source file (" << it->c_str() << ")\n";
        }
        else {
            if (avformat_find_stream_info(formatContext, NULL) < 0) {
                std::cerr << "Could not find stream information (" << it->c_str() << "\n";
            }
            else {
                //av_dump_format(formatContext, 0, it->c_str(), 0);
                
                codecContext = formatContext->streams[0]->codec;
                if(codecContext != NULL) {
                    if(codecContext->bit_rate >= 320000 && codecContext->codec_id == AV_CODEC_ID_MP3)
                        keep = true;
                }
            }
        }
        
        if(keep == false) {
            //std::cout << "___erase:" << *it << " " << codecContext->bit_rate << "-" <<codecContext->codec_id << std::endl;
            list.erase(it);
        }
    }
    
    std::cout << list.size() << " files in the list after cleaning\n\n";
    
    for (std::list<std::string>::iterator it = list.begin(); it != list.end(); ++it) {
        mp3List.push_back(new Mp3File(*it));
    }
}

bool Mp3Library::seekMp3(std::string _folder, bool recursive)
{
    /* adds all files contained in folder (recursively or not), whose extension is ".mp3", to list */
    std::string file;
    std::vector<std::string> folderList;
    
#ifdef WIN32
    
#else
    DIR *d = NULL;
    struct dirent *dir;
    
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
#endif //#ifdef WIN32
    
    if (recursive) {
        for (std::vector<std::string>::iterator it = folderList.begin(); it != folderList.end(); ++it) {
            seekMp3(*it, true);
        }
    }
    
    return true;
}

bool Mp3Library::analyzeMp3(int i)
{
    if (i >= mp3List.size()) {
        std::cerr << "i exceeds list size\n";
        return false;
    }
    
    Mp3File *mp3 = mp3List[i];
    
    if (!mp3->decodeAndAnalyze()) {
        std::cerr << "Could not analyze " << mp3->getFilename() << std::endl;
        return false;
    }
    mp3->coutInformations();
    
    return true;
}
