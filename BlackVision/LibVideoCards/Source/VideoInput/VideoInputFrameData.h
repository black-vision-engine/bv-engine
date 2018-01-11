#pragma once

#include "VideoInput/VideoInputChannelDesc.h"


namespace bv {
namespace videocards
{

// ***********************
/// @ingroup VideoCards
struct SingleChannelFrameData
{
    VideoCardID         CardID;
    VideoInputID        InputID;
    AVFramePtr          FrameData;
};

/**@brief Input data from video cards
@ingroup VideoCards*/
struct VideoInputFrameData
{
    std::vector< SingleChannelFrameData >   Frames;
};


} //videocards
} //bv



