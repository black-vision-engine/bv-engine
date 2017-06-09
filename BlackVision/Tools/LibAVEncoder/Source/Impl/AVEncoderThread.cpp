#include "AVEncoderThread.h"

#include "FFmpegEncoderUtils.h"

#include <thread>

namespace bv { namespace avencoder
{

//**************************************
//
AVEncoderThread::AVEncoderThread    ( AVFormatContext * oc, OutputStream * videoOS, OutputStream * audioOS, std::function< void ( const AVFrameConstPtr & ) > frameCompleteCallback )
    : m_oc( oc )
    , m_videoOS( videoOS )
    , m_audioOS( audioOS )
    , m_frameCompleteCallback( frameCompleteCallback )
{}

//**************************************
//
AVEncoderThread::~AVEncoderThread   () {}

//**************************************
//
void AVEncoderThread::Process       ()
{
    AVFrameConstPtr frm;
    m_framesQueue.WaitAndPop( frm );

    if( frm )
    {
        if( m_videoOS )
            FFmpegEncoderUtils::write_video_frame( m_oc, m_videoOS, frm );
        if( m_audioOS )
            FFmpegEncoderUtils::write_audio_frame( m_oc, m_audioOS, frm );
    }

    m_frameCompleteCallback( frm );

    // TODO: Add audio frame writing.
}

//**************************************
//
bool AVEncoderThread::IsEmpty       () const
{
    return m_framesQueue.IsEmpty();
}

//**************************************
//
void AVEncoderThread::EnqueueFrame  ( const AVFrameConstPtr & frame )
{
    m_framesQueue.Push( frame );
}

} // avencoder
} // bv