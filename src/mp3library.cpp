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
#include <cstdlib>
#include <QDirIterator>

#if defined(__MINGW32__) || defined(__MINGW64__)
#include <windows.h>
#endif

extern "C"
{
#include <libavformat/avformat.h>
}

void Mp3Library::fillList()
{
    /* analyses all 320kbps mp3s contained in list */
    AVFormatContext *formatContext = NULL; AVCodecContext *codecContext = NULL;
    bool keep;
    
    std::cout << "Opening folder: " << folder << std::endl;

    seekMp3();
    
    av_register_all();
    
    std::cout << list.size() << " files in the list before cleaning\n";
    
    for (std::list<QString>::iterator it = list.begin(); it != list.end(); ++it) {
        formatContext = NULL;
        keep = false;
        
        //std::cout << *it << std::endl;
        
                /* open input file, and allocate format context */
        if (avformat_open_input(&formatContext, it->toStdString().c_str(), NULL, NULL) < 0) {
            std::cerr << "Could not open source file (" << it->toStdString().c_str() << ")\n";
        }
        else {
            if (avformat_find_stream_info(formatContext, NULL) < 0) {
                std::cerr << "Could not find stream information (" << it->toStdString().c_str() << "\n";
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
            it = list.erase(it);
        }
    }
    
    std::cout << list.size() << " files in the list after cleaning\n\n";
    
    for (std::list<QString>::iterator it = list.begin(); it != list.end(); ++it) {
        mp3List.push_back(new Mp3File((*it).toStdString()));
    }
}

bool Mp3Library::seekMp3()
{
    /* adds all '*.mp3' contained in folder (recursively or not) to list */
    QDirIterator dirIterator(QString(folder.c_str()), recursive?QDirIterator::Subdirectories:QDirIterator::NoIteratorFlags);
    
    while (dirIterator.hasNext()) {
        dirIterator.next();
        if (QFileInfo(dirIterator.filePath()).isFile())
            if (QFileInfo(dirIterator.filePath()).suffix() == "mp3")
                list.push_back(dirIterator.filePath());
    }
    
    return true;
}

bool Mp3Library::analyzeMp3(int i)
{
    if (i >= (int)mp3List.size()) {
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

std::string Mp3Library::getFilename(int i) const
{
    if (i < (int)mp3List.size()) {
        return mp3List[i]->getFilename();
    }   
    return NULL;
}

int Mp3Library::getCutOffFrequency(int i) const
{
    if (i < (int)mp3List.size()) {
        return mp3List[i]->getCutOffFrequency();
    }  
    return -1;
}

double Mp3Library::getRate(int i) const
{
    if (i < (int)mp3List.size()) {
        return mp3List[i]->getRate();
    }  
    return -1;
}

void Mp3Library::update(QString _folder, bool _recursive)
{
    list.clear();
    mp3List.clear();
    
    folder = _folder.toStdString();
    recursive = _recursive;
}
