#include "stdafx.h"

#include "AudioUtils.h"
#include "CoreDEF.h"


namespace bv { namespace audio {

namespace
{

    Int32 AudioChannelsCount[] =
    {
        1,      // MONO8,
        2,      // STEREO8,
        1,      // MONO16,
        2       // STEREO16,
    };

    Int32 AudioChannelBitDepth[] =
    {
        8,      // MONO8,
        8,      // STEREO8,
        16,     // MONO16,
        16      // STEREO16,
    };

} //anonymous

// ****************************
//
Int32       AudioUtils::ChannelsCount   ( AudioFormat format )
{
    return AudioChannelsCount[ ( Int32 )format ]; 
}

// ****************************
//
Int32       AudioUtils::ChannelBitDepth ( AudioFormat format )
{
    return AudioChannelBitDepth[ ( Int32 )format ]; 
}

} //audio
} //bv
