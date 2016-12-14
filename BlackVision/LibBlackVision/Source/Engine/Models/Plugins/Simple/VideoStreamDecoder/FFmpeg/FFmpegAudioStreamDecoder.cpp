#include "stdafx.h"

#include "FFmpegAudioStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"
#include "Engine/Audio/Resources/AudioUtils.h"

#include "FFmpegUtils.h"


namespace bv {

const AVSampleFormat        FFmpegAudioStreamDecoder::SUPPORTED_FORMATS[]   = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_U8 };

// *******************************
//
FFmpegAudioStreamDecoder::FFmpegAudioStreamDecoder     ( AVAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize )
    : FFmpegStreamDecoder( formatCtx, streamIdx, maxQueueSize )
    , m_swrCtx( nullptr )
    , m_needConversion( false )
{
    m_sampleRate = audio::AudioUtils::DEFAULT_SAMPLE_RATE;
    m_format = ConvertFormat( audio::AudioUtils::DEFAULT_SAMPLE_FORMAT, m_nbChannels );
    m_nbChannels = ( std::min )( m_codecCtx->channels, m_nbChannels );

    //FIXME: always convert audio to default settings
    if( ( m_sampleRate != m_codecCtx->sample_rate ) || ( m_format != m_codecCtx->sample_fmt ) )
    {
        m_needConversion = true;

        m_swrCtx = swr_alloc();
        m_swrCtx = swr_alloc_set_opts( m_swrCtx, m_codecCtx->channel_layout, m_format, m_sampleRate, 
            m_codecCtx->channel_layout, m_codecCtx->sample_fmt, m_codecCtx->sample_rate, 0, nullptr );
        swr_init( m_swrCtx );
    }
}

// *******************************
//
FFmpegAudioStreamDecoder::~FFmpegAudioStreamDecoder    ()
{
    if( m_needConversion )
    {
        swr_free( &m_swrCtx );
    }
}

// *******************************
//
Int32                   FFmpegAudioStreamDecoder::GetSampleRate     () const
{
    return m_sampleRate;
}

// *******************************
//
AudioFormat             FFmpegAudioStreamDecoder::GetFormat         () const
{
    return ConvertFormat( m_format, m_nbChannels );
}

// *******************************
//
AVMediaData		FFmpegAudioStreamDecoder::ConvertFrame		()
{
    auto dst_nb_samples = av_rescale_rnd( swr_get_delay( m_swrCtx, m_frame->sample_rate ) + m_frame->nb_samples, m_sampleRate,  m_frame->sample_rate, AV_ROUND_UP);

    auto frameSize = ( SizeType )av_samples_get_buffer_size( nullptr, m_nbChannels, (int)dst_nb_samples, m_format, 0 );

	auto outBuffer = new uint8_t[ frameSize ];

    if( m_needConversion )
    {
        auto sn = swr_convert( m_swrCtx, &outBuffer, (int)dst_nb_samples, ( const uint8_t ** )m_frame->data, m_frame->nb_samples );
        int a = 0;
        { sn; a; };
    }
    else
    {
        memcpy( outBuffer, m_frame->data, frameSize );
    }
    
    AVMediaData mediaData;

    mediaData.framePTS = ( UInt64 )( 1000 * av_q2d( m_stream->time_base ) * m_frame->pts );
    mediaData.frameData = MemoryChunk::Create( ( char * )outBuffer, SizeType( frameSize ) );
    mediaData.nbSamples = dst_nb_samples;

    return mediaData;
}

// *******************************
//
AVSampleFormat          FFmpegAudioStreamDecoder::GetSupportedFormat    ( AVSampleFormat format )
{
    for( auto & supported : SUPPORTED_FORMATS )
    {
        if( supported == format )
        {
            return format;
        }
    }
    return SUPPORTED_FORMATS[ 0 ];
}

// *******************************
//
bool                    FFmpegAudioStreamDecoder::IsSupportedFormat     ( AVSampleFormat format )
{
    for( auto & supported : SUPPORTED_FORMATS )
    {
        if( supported == format )
        {
            return true;
        }
    }
    return false;
}

// *******************************
//
AudioFormat             FFmpegAudioStreamDecoder::ConvertFormat         ( AVSampleFormat format, Int32 nbChannels )
{
    if( nbChannels == 1 )
    {
        if( format == AV_SAMPLE_FMT_U8 )
        {
		    return AudioFormat::MONO8;
        }

        if( format == AV_SAMPLE_FMT_S16 )
        {
		    return AudioFormat::MONO16;
        }

        return AudioFormat::MONO16;
    }

    if( nbChannels == 2 )
    {
        if( format == AV_SAMPLE_FMT_U8 )
        {
		    return AudioFormat::STEREO8;
        }

        if( format == AV_SAMPLE_FMT_S16 )
        {
		    return AudioFormat::STEREO16;
        }

		return AudioFormat::STEREO16;
    }

    return AudioFormat::STEREO16;
}

// *******************************
//
AVSampleFormat          FFmpegAudioStreamDecoder::ConvertFormat         ( AudioFormat format, Int32 & nbChannels )
{
    if( format == AudioFormat::MONO8 )
    {
        nbChannels = 1;
		return AV_SAMPLE_FMT_U8;
    }
    else if( format == AudioFormat::MONO16 )
    {
        nbChannels = 1;
		return AV_SAMPLE_FMT_S16;
    }
    else if( format == AudioFormat::STEREO8 )
    {
        nbChannels = 2;
		return AV_SAMPLE_FMT_U8;
    }
    else if( format == AudioFormat::STEREO16 )
    {
        nbChannels = 2;
		return AV_SAMPLE_FMT_S16;
    }

    nbChannels = 2;
	return AV_SAMPLE_FMT_S16;
}

} //bv