#include "stdafx.h"

#include "FFmpegAudioStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"

#include "FFmpegUtils.h"


namespace bv {

const AVSampleFormat        FFmpegAudioStreamDecoder::SUPPORTED_FORMATS[]   = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_U8 };

// *******************************
//
FFmpegAudioStreamDecoder::FFmpegAudioStreamDecoder     ( AVAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize )
	: m_swrCtx( nullptr )
    , m_needConversion( false )
{
    m_streamIdx = streamIdx;
    
    m_maxQueueSize = maxQueueSize;

	m_stream = formatCtx->streams[ streamIdx ];
	m_codecCtx = m_stream->codec;
	m_codec = avcodec_find_decoder( m_codecCtx->codec_id );
	assert( m_codec != nullptr );
    
	bool error = ( avcodec_open2( m_codecCtx, m_codec, nullptr ) < 0 );
	assert( !error ); { error; }
    
    m_sampleRate = m_codecCtx->sample_rate;
    m_format = GetSupportedFormat( m_codecCtx->sample_fmt );
    m_nbChannels = std::min( m_codecCtx->channels, 2 );
    m_duration = ( UInt64 )( 1000 * av_q2d( m_stream->time_base ) * m_stream->duration );

    if( !IsSupportedFormat( m_codecCtx->sample_fmt ) )
    {
        m_needConversion = true;

        m_swrCtx = swr_alloc();
        m_swrCtx = swr_alloc_set_opts( m_swrCtx, m_codecCtx->channel_layout, m_format, m_codecCtx->sample_rate, 
            m_codecCtx->channel_layout, m_codecCtx->sample_fmt, m_codecCtx->sample_rate, 0, nullptr );
        swr_init( m_swrCtx );
    }

    m_frame = av_frame_alloc();
}

// *******************************
//
FFmpegAudioStreamDecoder::~FFmpegAudioStreamDecoder    ()
{
    if( m_needConversion )
    {
        swr_free( &m_swrCtx );
    }

	avcodec_close( m_codecCtx );

    av_frame_free( &m_frame );

    m_bufferQueue.Clear();
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
        auto packet = demuxer->GetPacket( m_streamIdx );
        if( packet )
        {
            auto success = DecodePacket( packet );
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

    av_packet_unref( packet ); //FIXME

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