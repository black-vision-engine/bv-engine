#include "stdafxBVApp.h"
#include "TimerMode.h"


#include "BVConfig.h"


namespace bv
{




// ***********************
//
TimerMode::TimerMode()
    : m_startTime( 0 )
    , m_frameNumber( 0 )
    , m_currentTime( 0 )
    , m_renderMode( RenderingMode::RM_RenderRealTime )
    , m_fps( DefaultConfig.TimerFPS() )
{}

// ***********************
//
void            TimerMode::SetStartTime     ( unsigned long time )
{
    unsigned long d = unsigned long( 1000.f / float( m_fps ) );
    m_startTime = time - time % d;
    m_currentTime = 0.0f;
    m_realTime = 0.0f;  //TimeType( m_startTime ) * TimeType( 0.001 );
    m_frameNumber = 0;
}

// ***********************
//
TimeType        TimerMode::StartFrame       ( unsigned long millis )
{
    unsigned long d = unsigned long( 1000.f / float( m_fps ) );
    millis = millis - millis % d;

    m_frameNumber++;
    m_realTime = TimeType( millis ) * TimeType( 0.001 );

    if( m_renderMode == RenderingMode::RM_RenderRealTime )
    {
        m_currentTime = m_realTime;
        return m_currentTime;
    }
    else if( m_renderMode == RenderingMode::RM_RenderOffscreen )
    {
        if( m_framesToRender == 0 )
        {
            // Offscreen Rendering ended. Restore previous state.
            m_renderMode = RenderingMode::RM_RenderRealTime;

            // @todo Make something with timestamp to keep continuity.
            m_currentTime = m_realTime;
            return m_currentTime;
        }

        --m_framesToRender;
        m_currentTime = m_currentTime + m_nextFrameOffset;
        return m_currentTime;
    }

    return 0.0f;
}

// ***********************
//
void            TimerMode::SetOffscreenRenderMode  ( float requestedFPS, UInt64 numFrames )
{
    m_nextFrameOffset = TimeType( 1 / requestedFPS );
    m_framesToRender = numFrames;

    m_renderMode = RenderingMode::RM_RenderOffscreen;
    m_currentTime = m_realTime;
}

// ***********************
//
TimeType        TimerMode::GetFramesDelta   () const
{
    return 1.f / ( float )m_fps;
}

}	// bv


