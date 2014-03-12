//
//  main.cpp
//  Fake320
//
//  Created by Vincent Timofti on 28/02/2014.
//  Copyright (c) 2014 Vincent Timofti. All rights reserved.
//

#include <iostream>
#include "mp3file.h"

using namespace std;

int main(int argc, const char * argv[])
{
    Mp3File mp3("/Users/vincent/Documents/short.mp3");
    
    mp3.decodeAndAnalyze();

    return 0;
}

