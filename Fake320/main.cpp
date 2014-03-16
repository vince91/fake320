//
//  main.cpp
//  Fake320
//
//  Created by Vincent Timofti on 28/02/2014.
//  Copyright (c) 2014 Vincent Timofti. All rights reserved.
//

#include <iostream>
#include "mp3file.h"
#include "mp3library.h"

using namespace std;

int main(int argc, const char * argv[])
{
    //
    Mp3File mp3("/Users/vincent/Documents/MATLAB/freq/mano.mp3");
    
    mp3.decodeAndAnalyze();
     //*/

    /*Mp3Library library("/Users/vincent/Music/MARS", false);
    
    library.analyzeFolder();*/

    return 0;
}

