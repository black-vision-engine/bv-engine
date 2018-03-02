#pragma once

#include "Memory/AVFrame.h"




namespace bv
{


// ***********************
//
inline AVFrameDescriptor       CreateDefaultAVFrame    ()
{
    AVFrameDescriptor desc;
    desc.width = 1000;
    desc.height = 1000;
    desc.channels = 2;
    desc.depth = 4;
    desc.numSamples = 44000;
    desc.channelDepth = 2;

    return desc;
}



}	// bv


