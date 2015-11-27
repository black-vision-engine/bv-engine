#include "EngineMode.h"


namespace bv
{
    
// ***********************
//
EngineMode::EngineMode()
    :   m_startTime( 0 ),
        m_frameNumber( 0 ),
        m_currentTime( 0 ),
        m_renderMode( RenderingMode::RenderRealTime )
{}

// ***********************
//
EngineMode::~EngineMode()
{}

// ***********************
//
void EngineMode::SetStartTime( unsigned long time )
{
    m_startTime = time;
    m_currentTime = m_startTime;
    m_frameNumber = 0;
}

// ***********************
//
void EngineMode::SetRenderToFileMode( float requestedFPS, unsigned int numFrames )
{
    m_nextFrameOffset = 1000 / requestedFPS;        // Time in millis
    m_framesToRender = numFrames;
}

// ***********************
//
TimeType EngineMode::StartFrame( unsigned long millis )
{
    m_frameNumber++;
    if( m_renderMode == RenderingMode::RenderRealTime )
        return TimeType( millis ) * TimeType( 0.001 );
    else if( m_renderMode == RenderingMode::RenderToFile )
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
TimeType EngineMode::GetFrameTime()
{    return TimeType( m_currentTime ) * TimeType( 0.001 );  }

// ***********************
//
unsigned int EngineMode::GetFrameNumber()
{    return m_frameNumber;  }


} //bv
