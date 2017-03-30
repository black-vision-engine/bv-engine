#include "stdafx.h"

#include "FFmpegVideoStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/Demuxer/FFmpegDemuxer.h"

#include <cassert>
#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/FFmpegUtils.h"


namespace bv {


// *******************************
//
FFmpegVideoStreamDecoder::FFmpegVideoStreamDecoder          ( AVAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize, FFmpegDemuxer * demuxer )
    : FFmpegStreamDecoder( formatCtx, streamIdx, maxQueueSize, demuxer )
    , m_swsCtx( nullptr )
    , m_width( 0 )
    , m_height( 0 )
    , m_frameRate( 0 )
{
    //raw video desc should provide width, height & format
    
    if( m_codecCtx->width == 0 || m_codecCtx->height == 0 )
    {
        m_codecCtx->width = asset->GetWidth();
        m_codecCtx->height = asset->GetHeight();

        m_stream->avg_frame_rate = av_d2q( asset->GetFrameRate(), INT_MAX );
        m_stream->time_base = av_inv_q( m_stream->avg_frame_rate );

        m_codecCtx->pix_fmt = FFmpegUtils::ToFFmpegPixelFormat( asset->GetVideoFormat() );
    }

    m_width = ( UInt32 )m_codecCtx->width;
    m_height = ( UInt32 )m_codecCtx->height;
    
    if( m_stream->avg_frame_rate.den && m_stream->avg_frame_rate.num )
    {
        m_frameRate = ( Float64 )av_q2d( m_stream->avg_frame_rate );
    }
    else
    {
        //invalid frame rate - assign default
        m_frameRate = 25.;
    }

    assert( m_width > 0 );
    assert( m_height > 0 );

    auto ffmpegFormat = FFmpegUtils::ToFFmpegPixelFormat( asset->GetTextureFormat() );
    m_swsCtx = sws_getCachedContext( m_swsCtx, m_width, m_height, m_codecCtx->pix_fmt,
        m_width, m_height, ffmpegFormat, SWS_BILINEAR, nullptr, nullptr, nullptr );

    m_outFrame = av_frame_alloc();
    m_frameSize = ( SizeType )av_image_get_buffer_size( ffmpegFormat, m_width, m_height, 1 );
    m_outBuffer = ( uint8_t * )av_malloc( ( Int32 )m_frameSize * sizeof( uint8_t ) );
    av_image_fill_arrays( ( uint8_t ** ) m_outFrame->data, m_outFrame->linesize, m_outBuffer, ffmpegFormat, m_width, m_height, 1 );
    m_outFrame->width = m_width;
    m_outFrame->height = m_height;
    m_outFrame->format = ( int )ffmpegFormat;
}

// *******************************
//
FFmpegVideoStreamDecoder::~FFmpegVideoStreamDecoder             ()
{
    sws_freeContext( m_swsCtx );
    av_free( m_outBuffer );             // deallocation of m_outFrame 
}

// *******************************
//
AVMediaData         FFmpegVideoStreamDecoder::ConvertFrame          ()
{
    sws_scale( m_swsCtx, m_frame->data, m_frame->linesize, 0, m_frame->height, m_outFrame->data, m_outFrame->linesize );

    char * data = new char[ m_frameSize ];
    memcpy( data, ( char * )m_outFrame->data[ 0 ], m_frameSize );

    AVMediaData mediaData;
    mediaData.framePTS = ( UInt64 )( 1000 * av_q2d( m_stream->time_base ) * m_frame->pts );
    mediaData.frameData = MemoryChunk::Create( data, SizeType( m_frameSize ) );
    
    return mediaData;
}

// *******************************
//
SizeType            FFmpegVideoStreamDecoder::GetFrameSize      () const
{
    return m_frameSize;
}

// *******************************
//
UInt32              FFmpegVideoStreamDecoder::GetWidth          () const
{
    return m_width;
}

// *******************************
//
UInt32              FFmpegVideoStreamDecoder::GetHeight         () const
{
    return m_height;
}

// *******************************
//
Float64             FFmpegVideoStreamDecoder::GetFrameRate      () const
{
    return m_frameRate;
}

// *******************************
//
bool                 FFmpegVideoStreamDecoder::IsAudio          () const
{
    return false;
}


} //bv