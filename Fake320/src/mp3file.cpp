//
//  mp3file.cpp
//  Fake320
//
//  Created by Vincent Timofti on 28/02/2014.
//  Copyright (c) 2014 Vincent Timofti. All rights reserved.
//

#include "mp3file.h"
#include <cstdint>
#include <fstream>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
}

Mp3File::Mp3File(const std::string & fileName)
{
    this->fileName = fileName;
}

int Mp3File::decodeAndAnalyze()
{
    int ret = 1, got_frame, lenght;
    
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
       outFile = fopen(outfile.c_str(), "wb");
        
        if (!outFile) {
            fprintf(stderr, "Could not open destination file %s\n", outfile.c_str());
            ret = 0;
        }
    }
    
    /* dump mp3 info to stderr */
    av_dump_format(formatContext, 0, fileName.c_str(), 0);
    
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
            lenght = decodePacket(&got_frame, 0);
            if (lenght < 0)
                break;
            packet.data += lenght;
            packet.size -= lenght;

        } while (packet.size > 0);
        
        av_free_packet(&orig_packet);
    }

    
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

int Mp3File::decodePacket(int *got_frame, int cached)
{
    int ret = 0;
    int decoded = packet.size;
    *got_frame = 0;
    
    /* decode audio frame */
    ret = avcodec_decode_audio4(codecContext, frame, got_frame, &packet);
    if (ret < 0) {
        fprintf(stderr, "Error decoding audio frame (%s)\n", av_err2str(ret));
        return ret;
    }
    /* Some audio decoders decode only part of the packet, and have to be
     * called again with the remainder of the packet data.
     * Sample: fate-suite/lossless-audio/luckynight-partial.shn
     * Also, some decoders might over-read the packet. */
    decoded = FFMIN(ret, packet.size);
    
    
    if (*got_frame) {
        
        size_t unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample(codecContext->sample_fmt);
        printf("audio_frame%s n:%d nb_samples:%d pts:%s\n", cached ? "(cached)" : "",
               frameCount++, frame->nb_samples,
               av_ts2timestr(frame->pts, &codecContext->time_base));
        
        if (frame->channels == 1)
            /* mono */
            fwrite(frame->extended_data[1], 1, unpadded_linesize, outFile);
        else if (frame->channels == 2) {
            /* stereo -> mono */
            for (int i = 0; i < frame->nb_samples; ++i) {
                
            }
        }
    }
    
    return decoded;
    
}