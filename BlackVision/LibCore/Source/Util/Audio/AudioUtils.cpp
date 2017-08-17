#include "stdafxCore.h"

#include <algorithm>

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

// *********************************
//
const UInt32        AudioUtils::DEFAULT_CHANNELS      = 2;
const UInt32        AudioUtils::DEFAULT_SAMPLE_RATE   = 48000;
const AudioFormat   AudioUtils::DEFAULT_SAMPLE_FORMAT = AudioFormat::STEREO16;

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

// ****************************
//
void         AudioUtils::MixAudio16   ( char * outData, const char * inData, SizeType size )
{
    //FIXME: use better method for mixing two audio channels, e.g.
    // factor = average(a1)
    // a1 = (a1/max(a1))*factor
    // a2 = (a2/max(a2))*factor
    // output = ((a1+a2)/max(a1+a2))*factor

    auto outData16 = reinterpret_cast< Int16 * >( outData );
    auto inData16 = reinterpret_cast< const Int16 * >( inData );
    auto size16 = size / sizeof( Int16 );

    for( SizeType i = 0; i < size16; ++i )
    {
        auto sum = ( Float32 )outData16[ i ] / ( Float32 )SHRT_MAX + ( Float32 )inData16[ i ] / ( Float32 )SHRT_MAX;
        sum = std::min( sum, 1.0f );
        sum = std::max( sum, -1.0f );
        outData16[ i ] = ( Int16 )( sum * SHRT_MAX );
    }
}

// ****************************
//
void		AudioUtils::ApplyGain		( char * outData, const char * inData, SizeType size, Float32 gain )
{
	gain = std::max( gain, 0.f ); // Limit gain to range [0, 1]
	gain = std::min( gain, 1.f );

	auto outData16 = reinterpret_cast< Int16 * >( outData );
	auto inData16 = reinterpret_cast< const Int16 * >( inData );
	auto size16 = size / sizeof( Int16 );

	for( SizeType i = 0; i < size16; ++i )
	{
		auto inVal = ( Float32 ) inData16[ i ] / ( Float32 ) SHRT_MAX;

		auto outVal = inVal * gain;
		outVal = std::min( outVal, 1.0f );
		outVal = std::max( outVal, -1.0f );

		outData16[ i ] = ( Int16 ) ( outVal * SHRT_MAX );
	}
}

// ****************************
//
void		AudioUtils::ApplyGain		( char * inData, SizeType size, Float32 gain )
{
    gain = std::max( gain, 0.f ); // Limit gain to range [0, 1]
    gain = std::min( gain, 1.f );

    auto inData16 = reinterpret_cast< Int16 * >( inData );
    auto size16 = size / sizeof( Int16 );

    for( SizeType i = 0; i < size16; ++i )
    {
        auto inVal = ( Float32 ) inData16[ i ] / ( Float32 ) SHRT_MAX;

        auto outVal = inVal * gain;
        outVal = std::min( outVal, 1.0f );
        outVal = std::max( outVal, -1.0f );

        inData16[ i ] = ( Int16 ) ( outVal * SHRT_MAX );
    }
}

// ****************************
//
SizeType     AudioUtils::AudioDataSize   ( UInt64 sampleRate, UInt32 numChannels, UInt32 sampleSizeInBytes, UInt32 fps )
{
    return sampleRate * numChannels * sampleSizeInBytes / fps;
}


} //audio
} //bv
