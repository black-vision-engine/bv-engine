#include "AVEncoderThread.h"

#include "FFmpegUtils.h"

#include <thread>

namespace bv { namespace avencoder
{

//**************************************
//
AVEncoderThread::AVEncoderThread    ( AVFormatContext * oc, OutputStream * videoOS, OutputStream * audioOS )
    : m_oc( oc )
    , m_videoOS( videoOS )
    , m_audioOS( audioOS )
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
        FFmpegUtils::write_video_frame( m_oc, m_videoOS, frm );

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