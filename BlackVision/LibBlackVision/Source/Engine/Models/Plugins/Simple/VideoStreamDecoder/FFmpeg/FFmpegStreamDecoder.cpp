#include "stdafx.h"

#include "FFmpegStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"


namespace bv {


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
bool		        FFmpegStreamDecoder::GetData	        ( AVMediaData & data )
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
void                FFmpegStreamDecoder::ClearDataQueue     ()
{
    m_bufferQueue.Clear();
}

// *******************************
//
void				FFmpegStreamDecoder::Reset				()
{
	avcodec_flush_buffers( m_codecCtx );
    ClearDataQueue();
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
