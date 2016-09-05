#include "stdafx.h"

#include "FFmpegAudioStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"

#include "FFmpegUtils.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

const AVSampleFormat        FFmpegAudioStreamDecoder::SUPPORTED_FORMATS[]   = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_U8 };

// *******************************
//
FFmpegAudioStreamDecoder::FFmpegAudioStreamDecoder     ( AVAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize )
    : FFmpegStreamDecoder( formatCtx, streamIdx, maxQueueSize )
    , m_swrCtx( nullptr )
    , m_needConversion( false )
{
    m_sampleRate = m_codecCtx->sample_rate;
    m_format = GetSupportedFormat( m_codecCtx->sample_fmt );
    m_nbChannels = ( std::min )( m_codecCtx->channels, 2 );

    if( !IsSupportedFormat( m_codecCtx->sample_fmt ) )
    {
        m_needConversion = true;

        m_swrCtx = swr_alloc();
        m_swrCtx = swr_alloc_set_opts( m_swrCtx, m_codecCtx->channel_layout, m_format, m_codecCtx->sample_rate, 
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
bool			        FFmpegAudioStreamDecoder::ProcessPacket		( FFmpegDemuxer * demuxer )
{
    if( m_bufferQueue.Size() < m_maxQueueSize )
    {
        auto ffmpegPacket = demuxer->GetPacket( m_streamIdx );
        if( ffmpegPacket )
        {
            auto success = DecodePacket( ffmpegPacket->GetAVPacket() );
            if( success )
            {
                auto data = ConvertFrame();
                m_bufferQueue.Push( data );

                return true;
            }
        }
    }
    return false;
}

// *******************************
//
bool				FFmpegAudioStreamDecoder::DecodePacket		( AVPacket * packet )
{
	assert( packet != nullptr );

	int frameReady = 0;
    
    while ( packet->size > 0)
    {
        auto len = avcodec_decode_audio4( m_codecCtx, m_frame, &frameReady, packet );
        if( len < 0 )
        {
            //FIXME: error
        }

        packet->size -= len;
        packet->data += len;
    }

    return ( frameReady != 0 );
}

// *******************************
//
AVMediaData		FFmpegAudioStreamDecoder::ConvertFrame		()
{
    auto frameSize = ( SizeType )av_samples_get_buffer_size( nullptr, m_nbChannels, m_frame->nb_samples, m_format, 0 );
	auto outBuffer = new uint8_t[ frameSize ];

    if( m_needConversion )
    {
        swr_convert( m_swrCtx, &outBuffer, m_frame->nb_samples, ( const uint8_t ** )m_frame->extended_data, m_frame->nb_samples );
    }
    else
    {
        memcpy( outBuffer, m_frame->data, frameSize );
    }
    
    AVMediaData mediaData;

    mediaData.framePTS = ( UInt64 )( 1000 * av_q2d( m_stream->time_base ) * m_frame->pkt_pts );
    mediaData.frameData = MemoryChunk::Create( ( char * )outBuffer, SizeType( frameSize ) );
    mediaData.nbSamples = m_frame->nb_samples;

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

} //bv