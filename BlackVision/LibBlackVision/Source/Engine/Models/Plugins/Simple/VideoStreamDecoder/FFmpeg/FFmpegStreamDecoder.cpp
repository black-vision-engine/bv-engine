#include "stdafx.h"

#include "FFmpegStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"




#include "Memory/MemoryLeaks.h"



namespace bv {


// *******************************
//
FFmpegStreamDecoder::FFmpegStreamDecoder                    ( AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize )
    : m_streamIdx( streamIdx )
    , m_maxQueueSize( maxQueueSize )
    , m_offset( 0 )
{
    m_stream = formatCtx->streams[ streamIdx ];
    m_codecCtx = m_stream->codec;
    m_codec = avcodec_find_decoder( m_codecCtx->codec_id );
    assert( m_codec != nullptr );

    bool error = ( avcodec_open2( m_codecCtx, m_codec, nullptr ) < 0 );
    assert( !error ); { error; }

    m_duration = ( UInt64 )( 1000 * av_q2d( m_stream->time_base ) * m_stream->duration );

    m_frame = av_frame_alloc();
}

// *******************************
//
FFmpegStreamDecoder::~FFmpegStreamDecoder                   ()
{
    avcodec_close( m_codecCtx );
    av_frame_free( &m_frame );
    m_bufferQueue.Clear();
}

// *******************************
//
UInt64              FFmpegStreamDecoder::GetDuration        () const
{
    return m_duration;
}

// *******************************
//
UInt64              FFmpegStreamDecoder::GetCurrentPTS      ()
{
    AVMediaData data;
    m_bufferQueue.Front( data );
    return data.framePTS;
}

// *******************************
//
bool		        FFmpegStreamDecoder::PopData	        ( AVMediaData & data )
{
    return m_bufferQueue.TryPop( data );
}

// *******************************
//
bool                FFmpegStreamDecoder::IsDataQueueEmpty   () const
{
    return m_bufferQueue.IsEmpty();
}

// *******************************
//
void				FFmpegStreamDecoder::Reset				()
{
    m_bufferQueue.Clear();
	avcodec_flush_buffers( m_codecCtx );
}

// *********************************
//
void			    FFmpegStreamDecoder::SetOffset	        ( UInt64 offset )
{
    m_offset = offset;
}

// *********************************
//
UInt64			    FFmpegStreamDecoder::GetOffset	        () const
{
    return m_offset;
}
    
// *********************************
//
Int32			    FFmpegStreamDecoder::GetStreamIdx	    () const
{
    return m_streamIdx;
}

// *******************************
//
Int64               FFmpegStreamDecoder::ConvertTime        ( Float64 time )
{
    auto timeBase = av_q2d( m_stream->time_base );
    return ( Int64 )( time / timeBase );
}

} //bv
