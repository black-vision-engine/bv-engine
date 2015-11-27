#include "RenderMode.h"


namespace bv
{
    
// ***********************
//
RenderMode::RenderMode()
    :   m_startTime( 0 ),
        m_frameNumber( 0 ),
        m_currentTime( 0 ),
        m_renderMode( RenderingMode::RenderRealTime )
{}

// ***********************
//
RenderMode::~RenderMode()
{}

// ***********************
//
void RenderMode::SetStartTime( unsigned long time )
{
    m_startTime = time;
    m_currentTime = m_startTime;
    m_frameNumber = 0;
}

// ***********************
//
void RenderMode::SetRenderToFileMode( float requestedFPS, unsigned int numFrames )
{
    m_nextFrameOffset = 1000 / requestedFPS;        // Time in millis
    m_framesToRender = numFrames;
}

// ***********************
//
TimeType RenderMode::StartFrame( unsigned long millis )
{
    m_frameNumber++;
    if( m_renderMode == RenderingMode::RenderRealTime )
        return TimeType( millis ) * TimeType( 0.001 );
    else if( m_renderMode == RenderingMode::RenderOffscreen )
    {
        if( m_framesToRender < 0 )
        {// Rendering to file ended. Restore previous state.
            m_renderMode = RenderingMode::RenderRealTime;
            // @todo Make something with timestamp to keep continuity.
            return TimeType( millis ) * TimeType( 0.001 );
        }
        
        --m_framesToRender;
        return ( TimeType( millis ) + TimeType( m_nextFrameOffset ) ) * TimeType( 0.001 );
    }
    return 0.0f;
}

// ***********************
//
TimeType RenderMode::GetFrameTime()
{    return TimeType( m_currentTime ) * TimeType( 0.001 );  }

// ***********************
//
unsigned int RenderMode::GetFrameNumber()
{    return m_frameNumber;  }


} //bv
