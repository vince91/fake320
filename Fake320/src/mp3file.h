//
//  mp3file.h
//  Fake320
//
//  Created by Vincent Timofti on 28/02/2014.
//  Copyright (c) 2014 Vincent Timofti. All rights reserved.
//

#ifndef __Fake320__mp3file__
#define __Fake320__mp3file__

#include <iostream>
#include <fstream>

extern "C"
{
#include <libavformat/avformat.h>
}

#define FFT_SIZE 65536

class Mp3File
{
public:
    Mp3File(const std::string & fileName);
    std::string getFilename() const { return fileName; }
    int decodeAndAnalyze();
    std::string outfile = "/Users/vincent/Documents/test";
    
private:
    std::string fileName;
    double samples[FFT_SIZE];
    int sample_index = 0;
    
    AVFormatContext *formatContext = NULL;
    AVCodecContext *codecContext = NULL;
    AVPacket packet;
    AVFrame *frame = NULL;
    AVStream *stream = NULL;
    FILE *outFile;
    int frameCount = 0;
    int streamIndex = -1;


    
    bool openCodecContext();
    int decodePacket(int *got_frame, int cached);
};

#endif /* defined(__Fake320__mp3file__) */
