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
#include <fftw3.h>
}

#define FFT_NBITS 16

class Mp3File
{
public:
    Mp3File(std::string fileName);
    ~Mp3File();
    
    std::string getFilename() const { return fileName; }
    int decodeAndAnalyze();
    
private:
    std::string fileName, outfileName = "/Users/vincent/Documents/MATLAB/fake320/out";

    double **samples;
    fftw_complex *fftOut;
    double *fftMagnitude;
    
    int index = 0, currentArray = 0, fftSize, frameCount = 0, fftCount = 0;
    
    AVFormatContext *formatContext = NULL;
    AVCodecContext *codecContext = NULL;
    AVPacket packet;
    AVFrame *frame = NULL;
    AVStream *stream = NULL;
    int streamIndex = -1, gotFrame;
    
    FILE *outFile;
    std::fstream st;
    
    bool openCodecContext();
    bool fft();
    int decodePacket();
};

#endif /* defined(__Fake320__mp3file__) */
