#include "stdafx.h"

#include "FFmpegAudioStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/Demuxer/FFmpegDemuxer.h"
#include "Engine/Audio/Resources/AudioUtils.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/FFmpegUtils.h"

#include "UseLoggerLibBlackVision.h"


namespace bv {

const AVSampleFormat        FFmpegAudioStreamDecoder::SUPPORTED_FORMATS[] = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_U8 };

// *******************************
//
FFmpegAudioStreamDecoder::FFmpegAudioStreamDecoder     ( AVAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize, FFmpegDemuxer * demuxer )
    : FFmpegStreamDecoder( formatCtx, streamIdx, maxQueueSize, demuxer )
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

    m_maxBufferSize = ( SizeType )av_samples_get_buffer_size( nullptr, m_nbChannels, m_sampleRate, AV_SAMPLE_FMT_FLT, 1 );
    m_tmpBuffer = new uint8_t[ m_maxBufferSize ];
}

// *******************************
//
FFmpegAudioStreamDecoder::~FFmpegAudioStreamDecoder    ()
{
    if( m_needConversion )
    {
        swr_free( &m_swrCtx );
    }

    if( m_tmpBuffer )
    {
        delete m_tmpBuffer;
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
    AVMediaData mediaData;
    uint8_t * outBuffer = nullptr;
    SizeType frameSize = 0;
    Int64 outSamples = 0;
    

    if( m_needConversion )
    {
        outSamples = av_rescale_rnd( swr_get_delay( m_swrCtx, m_frame->sample_rate ) + m_frame->nb_samples, m_sampleRate, m_frame->sample_rate, AV_ROUND_UP );
        frameSize = ( SizeType )av_samples_get_buffer_size( nullptr, m_nbChannels, ( Int32 )outSamples, m_format, 1 );

        if( frameSize > m_maxBufferSize )
        {
            m_maxBufferSize = frameSize;
            delete m_tmpBuffer;
            m_tmpBuffer = new uint8_t[ m_maxBufferSize ];
        }
        outSamples = swr_convert( m_swrCtx, &m_tmpBuffer, ( Int32 )outSamples, ( const uint8_t ** )m_frame->data, m_frame->nb_samples );
        
		if( outSamples >= 0 )
		{
			frameSize = ( SizeType ) av_samples_get_buffer_size( nullptr, m_nbChannels, ( Int32 ) outSamples, m_format, 1 );
			outBuffer = new uint8_t[ frameSize ];
			memcpy( outBuffer, m_tmpBuffer, frameSize );
		}
		else
		{
			outSamples = 0;
			frameSize = 0;
			LOG_MESSAGE( SeverityLevel::error ) << "Conversion Error";
		}
    }
    else
    {
        outSamples = m_frame->nb_samples;
        frameSize = ( SizeType )av_samples_get_buffer_size( nullptr, m_nbChannels, ( Int32 )outSamples, m_format, 1 );
        outBuffer = new uint8_t[ frameSize ];
        memcpy( outBuffer, m_frame->data[ 0 ], frameSize );
    }

    mediaData.framePTS = ( UInt64 )( 1000 * av_q2d( m_stream->time_base ) * m_frame->pts );
    mediaData.frameData = MemoryChunk::Create( ( char * )outBuffer, frameSize );
    mediaData.nbSamples = outSamples;

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