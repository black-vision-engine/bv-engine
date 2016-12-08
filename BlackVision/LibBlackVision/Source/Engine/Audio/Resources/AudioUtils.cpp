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

    Int32 AudioChannelDepth[] =
    {
        1,      // MONO8,
        1,      // STEREO8,
        2,      // MONO16,
        2       // STEREO16,
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
Int32       AudioUtils::ChannelDepth ( AudioFormat format )
{
    return AudioChannelDepth[ ( Int32 )format ]; 
}

} //audio
} //bv
