#pragma once

#include "Engine/Types/Enums.h"
#include "CoreDEF.h"


namespace bv { namespace audio {

class AudioUtils
{
public:
    
    static const UInt32                 DEFAULT_SAMPLE_RATE;
    static const AudioFormat            DEFAULT_SAMPLE_FORMAT;

public:

    static Int32        ChannelsCount   ( AudioFormat format );
    static Int32        ChannelDepth    ( AudioFormat format );

    /** @brief The most simple audio mixer */
    static void         MixAudio16      ( char * outData, const char * inData, SizeType size );

};

} //audio
} //bv
