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

#define SHORT_MAX 32768.
#define FFT_NBITS 14
#define FFT_SIZE 16384
#define FFTMEANS_SIZE 12
#define FFT_CORRECTION 4./(FFT_SIZE*FFT_SIZE)

extern "C"
{
#include <libavformat/avformat.h>
#include <fftw3.h>
}

class Mp3File
{    
public:
    Mp3File(std::string);
    ~Mp3File();
    
    bool decodeAndAnalyze();
    std::string getFilename() const { return filename; }
    int getCutOffFrequency() const { return cutOffFrequency; }
    double getRate() const { return rate; }
    void coutInformations() const;
    
private:
    std::string filename;

    double samples[2][FFT_SIZE];
    fftw_complex fftOut[FFT_SIZE/2 + 1];
    double fftMagnitude[FFT_SIZE/2 + 1], fftMeans[FFTMEANS_SIZE], fftMeansDiff[FFTMEANS_SIZE - 1], counter[FFTMEANS_SIZE - 1] = {0};
    
    int index = 0, currentArray = 0, frameCount = 0, fftCount = 0, streamIndex = -1, gotFrame;
    
    AVFormatContext *formatContext = NULL;
    AVCodecContext *codecContext = NULL;
    AVPacket packet;
    AVFrame *frame = NULL;
    AVStream *stream = NULL;
    
    int cutOffFrequency;
    double rate;
    
    bool openCodecContext();
    bool fftAnalysis();
    int decodePacket();
};

#endif /* defined(__Fake320__mp3file__) */
