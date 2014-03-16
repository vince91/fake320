//
//  mp3file.cpp
//  Fake320
//
//  Created by Vincent Timofti on 28/02/2014.
//  Copyright (c) 2014 Vincent Timofti. All rights reserved.
//

#include "mp3file.h"
#include <cstdint>
#include <cmath>
#include <fstream>

#define BAND_INTERVAL 23
#define BAND_START_INDEX 674

extern "C"
{
#include <libavformat/avformat.h>
#include <fftw3.h>
}

Mp3File::Mp3File(std::string fileName)
{
    this->fileName = fileName;
    
    st.open("/Users/vincent/Documents/MATLAB/fake320/test.txt", std::fstream::out);
    if (!st.is_open())
        std::cout << "pas ouvert !!!";
}

Mp3File::~Mp3File()
{
    st.close();
}

int Mp3File::decodeAndAnalyze()
{
    int ret = 1, lenght;
    
    /* register all formats and codecs */
    av_register_all();
    
    /* open input file, and allocate format context */
    if (avformat_open_input(&formatContext, fileName.c_str(), NULL, NULL) < 0) {
        std::cerr << "Could not open source file (" << fileName << ")\n";
        return 0;
    }
    
    /* retrieve stream information */
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        std::cerr << "Could not find stream information (" << fileName << "\n";
        return 0;
    }
    
    if (openCodecContext()) {
        outFile = fopen(outfileName.c_str(), "wb");
        
        if (!outFile) {
            fprintf(stderr, "Could not open destination file %s\n", outfileName.c_str());
            ret = 0;
        }
    }
    
    /* dump mp3 info to stderr */
    //av_dump_format(formatContext, 0, fileName.c_str(), 0);
    
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");
    }
    
    av_init_packet(&packet);
    packet.data = NULL;
    packet.size = 0;
    
    /* read frames from the file */
    while (av_read_frame(formatContext, &packet) >= 0) {
        AVPacket orig_packet = packet;
        
        do {
            lenght = decodePacket();
            if (lenght < 0)
                break;
            packet.data += lenght;
            packet.size -= lenght;
            
        } while (packet.size > 0);
        
        av_free_packet(&orig_packet);
    }
    
    std::cout << "Frames: " << frameCount << ", FFTs:" << fftCount << std::endl;
    
    int maxIndex = 0, maxValue = counter[0];
    for (int i = 0; i < FFTMEANS_SIZE - 1; ++i) {
        std::cout << counter[i] << ",";
        if (counter[i] >= maxValue) {
            maxIndex = i;
            maxValue = counter[maxIndex];
        }
    }

    std::cout << std::endl << "Max:" << maxIndex << "(" << ((maxIndex+1)*500+14500) << ") :" << maxValue << "(" << 100.*maxValue/fftCount << "%)" << std::endl;
    
    avcodec_close(codecContext);
    avformat_close_input(&formatContext);
    
    return ret;
}

bool Mp3File::openCodecContext()
{
    int ret;
    AVCodec *codec;
    
    streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    
    stream = formatContext->streams[streamIndex];
    codecContext = formatContext->streams[streamIndex]->codec;
    
    codec = avcodec_find_decoder(AV_CODEC_ID_MP3);
    if (!codec) {
        std::cerr << "Codec not found\n";
        return false;
    }
    
    /* Init the decoders without reference counting */
    if ((ret = avcodec_open2(codecContext, codec, NULL)) < 0) {
        std::cerr << "Failed to open MP3 codec\n";
        return false;
    }
    
    return true;
}

int Mp3File::decodePacket()
{
    int ret = 0, saveIndex, saveCurrent;
    int decoded = packet.size;
    gotFrame = 0;
    double temp;
    bool samplesComplete = false;
    
    /* decode audio frame */
    if ((ret = avcodec_decode_audio4(codecContext, frame, &gotFrame, &packet)) < 0) {
        std::cerr << "Error decoding audio frame (" << av_err2str(ret) << ")\n";
        return ret;
    }
    
    decoded = FFMIN(ret, packet.size);
    
    if (gotFrame) {
        
        frameCount++;
        
        size_t unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample(codecContext->sample_fmt);
        
        saveIndex = index; saveCurrent = currentArray ;
        
        if (frame->channels >= 1) {
            /* mono */
            for (int i = 0; i < frame->nb_samples; ++i) {
                samples[currentArray][index++] = (short) (frame->extended_data[0][2 * i] | frame->extended_data[0][2 * i + 1] << 8) / SHORT_MAX;
            
                if (index == FFT_SIZE) {
                    index = 0;
                    currentArray = (currentArray == 1) ? 0 : 1;
                    samplesComplete = true;
                }
            }
            
            fwrite(frame->extended_data[0], 1, unpadded_linesize, outFile);
        }
        
        if (frame->channels == 2) {
            /* stereo -> mono */
            index = saveIndex; currentArray = saveCurrent;
            
            for (int i = 0; i < frame->nb_samples; ++i) {
                temp = (short) (frame->extended_data[1][2 * i] | frame->extended_data[1][2 * i + 1] << 8) / SHORT_MAX;
                temp = (temp + samples[currentArray][index]) / 2;
                samples[currentArray][index++] = temp;
                
                if (index == FFT_SIZE) {
                    index = 0;
                    currentArray = (currentArray == 1) ? 0 : 1;
                }
            }
        }
    }
    
    if(samplesComplete)
        fftAnalysis();
    
    return decoded;
}

bool Mp3File::fftAnalysis()
{
    int index = (currentArray) ? 0 : 1;
    ++fftCount;
    
    fftw_plan plan = fftw_plan_dft_r2c_1d(FFT_SIZE, samples[index], fftOut, FFTW_ESTIMATE);
    fftw_execute(plan);
    
    for (int i = 0; i < (FFT_SIZE/2 + 1); ++i) {
        fftMagnitude[i] = 10*log10(FFT_CORRECTION * (fftOut[i][0]*fftOut[i][0] + fftOut[i][1]*fftOut[i][1]));
    }
    
    for (int i = 0; i < FFTMEANS_SIZE; ++i) {
        fftMeans[i] = 0;
        for (int j = 0; j < BAND_INTERVAL; ++j) {
            fftMeans[i] += fftMagnitude[BAND_START_INDEX + i*BAND_INTERVAL + j];
        }
        fftMeans[i] /= BAND_INTERVAL;
    }
    
    /* differences between adjacente elements of fftMeans[] */
    for (int i = 0; i < FFTMEANS_SIZE - 1; ++i) {
        fftMeansDiff[i] = fftMeans[i + 1] - fftMeans[i];
    }
    
    double minValue = fftMeansDiff[0]; int minIndex = 0;
    
    for (int i = 1; i < FFTMEANS_SIZE - 1; ++i) {
        if (fftMeansDiff[i] < minValue) {
            minValue = fftMeansDiff[i];
            minIndex = i;
        }
    }
    
    if (minValue < -6) {
        /*
        bool valid = true;
        for (int i = minIndex; i < FFTMEANS_SIZE && valid == true; ++i) {
            if (fftMeans[i] > -70)
                valid = false;
        }
        
        if (valid)*/
            ++counter[minIndex];
        
    }
    
    
    if (fftCount == 3000) {
        st << fftMeans[0];
        
        for (int i = 1; i < FFTMEANS_SIZE - 1; ++i) {
            st << "," << fftMeans[i];
        }
    }
    
    return true;
}
