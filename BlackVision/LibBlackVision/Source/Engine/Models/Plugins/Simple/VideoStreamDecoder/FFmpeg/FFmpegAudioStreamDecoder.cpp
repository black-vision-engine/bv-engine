#include "stdafx.h"

#include "FFmpegAudioStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"

#include "FFmpegUtils.h"


namespace bv {

const AVSampleFormat        FFmpegAudioStreamDecoder::DEFAULT_FORMAT  = AV_SAMPLE_FMT_S16;

// *******************************
//
FFmpegAudioStreamDecoder::FFmpegAudioStreamDecoder     ( VideoStreamAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize )
	: m_swrCtx( nullptr )
    , m_maxQueueSize( maxQueueSize )
    , m_frameSize( 0 )
{
    m_streamIdx = streamIdx;

	m_stream = formatCtx->streams[ streamIdx ];
	m_codecCtx = m_stream->codec;
	m_codec = avcodec_find_decoder( m_codecCtx->codec_id );
	assert( m_codec != nullptr );
    
	bool error = ( avcodec_open2( m_codecCtx, m_codec, nullptr ) < 0 );
	assert( !error ); { error; }
    
    m_swrCtx = swr_alloc();
    m_swrCtx = swr_alloc_set_opts( m_swrCtx, m_codecCtx->channel_layout, DEFAULT_FORMAT, m_codecCtx->sample_rate, 
        m_codecCtx->channel_layout, m_codecCtx->sample_fmt, m_codecCtx->sample_rate, 0, nullptr );
    swr_init( m_swrCtx );

    m_sampleRate = m_codecCtx->sample_rate;
	if( m_codecCtx->channels == 1 )
    {
		m_format = AudioFormat::MONO16;
	}
    else if( m_codecCtx->channels == 2 )
    {
		m_format = AudioFormat::STEREO16;
	}

    m_frame = av_frame_alloc();

    m_frameSize = ( SizeType )av_samples_get_buffer_size( nullptr, m_codecCtx->channels, m_codecCtx->frame_size, DEFAULT_FORMAT, 0 );
	m_outBuffer = ( uint8_t * )av_malloc( ( Int32 )m_frameSize );
    memset( m_outBuffer, 0, ( Int32 )m_frameSize );
}

// *******************************
//
FFmpegAudioStreamDecoder::~FFmpegAudioStreamDecoder    ()
{
    swr_free( &m_swrCtx );
	avcodec_close( m_codecCtx );

    av_frame_free( &m_frame );
    av_free( m_outBuffer );

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
    return m_format;
}

// *******************************
//
bool		            FFmpegAudioStreamDecoder::GetData	        ( AVMediaData & data )
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
    avcodec_decode_audio4( m_codecCtx, m_frame, &frameReady, packet );

    av_packet_unref( packet ); //FIXME

    return ( frameReady != 0 );
}

// *******************************
//
AVMediaData		FFmpegAudioStreamDecoder::ConvertFrame		()
{
    swr_convert( m_swrCtx, &m_outBuffer, m_frame->nb_samples, ( const uint8_t ** )m_frame->extended_data, m_frame->nb_samples );

    char * data = new char[ m_frameSize ];
    memcpy( data, ( char * )&m_outBuffer, m_frameSize );

    AVMediaData mediaData;

    mediaData.frameIdx = ( UInt32 )m_frame->pkt_pts;
    mediaData.frameData = MemoryChunk::Create( data, SizeType( m_frameSize ) );

    return mediaData;
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