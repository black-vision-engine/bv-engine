#pragma once

#include "CoreEnums.h"
#include "CoreDEF.h"


namespace bv { namespace audio {

class AudioUtils
{
public:
    
    static const UInt32                 DEFAULT_CHANNELS;
    static const UInt32                 DEFAULT_SAMPLE_RATE;
    static const AudioFormat            DEFAULT_SAMPLE_FORMAT;

public:

    static Int32		ChannelsCount	( AudioFormat format );
    static Int32		ChannelDepth    ( AudioFormat format );

    static SizeType     AudioDataSize   ( UInt64 sampleRate, UInt32 numChannels, UInt32 sampleSizeInBytes, UInt32 fps );

    /** @brief The most simple audio mixer */
    static void			MixAudio16      ( char * outData, const char * inData, SizeType size );

	static void			ApplyGain		( char * outData, const char * inData, SizeType size, Float32 gain );
    static void			ApplyGain		( char * inData, SizeType size, Float32 gain );
};

} //audio
} //bv
