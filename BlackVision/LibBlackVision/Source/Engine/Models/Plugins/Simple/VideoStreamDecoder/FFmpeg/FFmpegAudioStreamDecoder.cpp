#include "stdafx.h"

#include "FFmpegAudioStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"

#include "FFmpegUtils.h"


namespace bv {


// *******************************
//
FFmpegAudioStreamDecoder::FFmpegAudioStreamDecoder     ( VideoStreamAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize )
	: m_swrCtx( nullptr )
    , m_maxQueueSize( maxQueueSize )
{
    m_streamIdx = streamIdx;

	m_stream = formatCtx->streams[ streamIdx ];
	m_codecCtx = m_stream->codec;
	m_codec = avcodec_find_decoder( m_codecCtx->codec_id );
	assert( m_codec != nullptr );
    
	bool error = ( avcodec_open2( m_codecCtx, m_codec, nullptr ) < 0 );
	assert( !error ); { error; }

    m_swrCtx = swr_alloc();
    m_swrCtx = swr_alloc_set_opts( m_swrCtx, m_codecCtx->channel_layout, AV_SAMPLE_FMT_S16, m_codecCtx->sample_rate, m_codecCtx->channel_layout, m_codecCtx->sample_fmt, m_codecCtx->sample_rate, 0, nullptr );
    swr_init( m_swrCtx );

	if( m_codecCtx->channels == 1 )
    {
		m_format = AudioFormat::MONO16;
	}
    else if( m_codecCtx->channels == 2 )
    {
		m_format = AudioFormat::STEREO16;
	}

    m_frame = av_frame_alloc();
	m_outFrame = av_frame_alloc();
        
    m_outFrame->nb_samples = m_codecCtx->frame_size;
    m_outFrame->format = m_codecCtx->sample_fmt;
    m_outFrame->channel_layout = m_codecCtx->channel_layout;
    m_outFrame->channels = m_codecCtx->channels;

    m_frameSize = ( SizeType )av_samples_get_buffer_size( nullptr, m_codecCtx->channels, m_codecCtx->frame_size, m_codecCtx->sample_fmt, 0 );
	m_outBuffer = ( uint8_t * )av_malloc( ( Int32 )m_frameSize * sizeof( uint8_t ) );
    avcodec_fill_audio_frame( m_outFrame, m_codecCtx->channels, m_codecCtx->sample_fmt, ( const uint8_t * )m_outBuffer, ( Int32 )m_frameSize, 0 );
}

// *******************************
//
FFmpegAudioStreamDecoder::~FFmpegAudioStreamDecoder    ()
{
    swr_free( &m_swrCtx );
	avcodec_close( m_codecCtx );

    av_frame_free( &m_frame );
    av_free( m_outBuffer );     // deallocation of m_outFrame 

    m_bufferQueue.Clear();
}

// *******************************
//
AudioFormat             FFmpegAudioStreamDecoder::GetAudioFormat    () const
{
    return m_format;
}

// *******************************
//
bool		            FFmpegAudioStreamDecoder::GetData	        ( AudioMediaData & data )
{
    return m_bufferQueue.TryPop( data );
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
            if( DecodePacket( packet ) )
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
    auto decoded = avcodec_decode_audio4( m_codecCtx, m_frame, &frameReady, packet );
    decoded;
	av_packet_unref( packet ); //FIXME
    
    //return std::min( decoded, packet->size );
    return ( frameReady != 0 );
}

// *******************************
//
AudioMediaData		FFmpegAudioStreamDecoder::ConvertFrame		()
{
    swr_convert( m_swrCtx, m_outFrame->data, m_outFrame->nb_samples, ( const uint8_t ** )m_frame->extended_data, m_frame->nb_samples );

    auto size = 0; //FIXME
    char * data = new char[ size ];
    memcpy( data, ( char * )m_outFrame->data[ 0 ], size );

    AudioMediaData mediaData;

    mediaData.frameIdx = ( UInt32 )m_frame->pkt_pts;
    mediaData.frameData = MemoryChunk::Create( data, SizeType( size ) );
    mediaData.sampleRate = m_frame->sample_rate;
    mediaData.format = GetAudioFormat();

    return mediaData;
}


// *******************************
//
Int64					FFmpegAudioStreamDecoder::ConvertTime		( Float64 time )
{
    auto timeBase = av_q2d( m_stream->time_base );
	return ( Int64 )( time / timeBase );
}

// *******************************
//
void					FFmpegAudioStreamDecoder::Reset				()
{
	avcodec_flush_buffers( m_codecCtx );
    m_bufferQueue.Clear();
}

// *******************************
//
Int32					FFmpegAudioStreamDecoder::GetStreamIdx		() const
{
	return m_streamIdx;
}

} //bv