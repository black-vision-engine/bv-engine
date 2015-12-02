#include "RenderMode.h"

#include "Rendering/Logic/RenderLogic.h"

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
    m_currentTime = TimeType( m_startTime ) * TimeType( 0.001 );
    m_frameNumber = 0;
}

// ***********************
//
void RenderMode::SetRenderToFileMode( const std::string& filePath, float requestedFPS, unsigned int numFrames )
{
    m_nextFrameOffset = 1000 / requestedFPS;        // Time in millis
    m_framesToRender = numFrames;

    if( m_renderLogic )
        m_renderLogic->MakeScreenShot( filePath, numFrames );

    m_renderMode = RenderingMode::RenderOffscreen;
}

// ***********************
//
void RenderMode::MakeScreenShot( const std::string& filePath )
{
    if( m_renderLogic )
        m_renderLogic->MakeScreenShot( filePath, 1 );
}

// ***********************
//
TimeType RenderMode::StartFrame( unsigned long millis )
{
    m_frameNumber++;
    m_realTime = TimeType( millis ) * TimeType( 0.001 );

    if( m_renderMode == RenderingMode::RenderRealTime )
    {
        m_currentTime = m_realTime;
        return m_currentTime;
    }
    else if( m_renderMode == RenderingMode::RenderOffscreen )
    {
        if( m_framesToRender < 0 )
        {// Rendering to file ended. Restore previous state.
            m_renderMode = RenderingMode::RenderRealTime;
            // @todo Make something with timestamp to keep continuity.
            m_currentTime = m_realTime;
            return m_currentTime;
        }
        
        --m_framesToRender;
        m_currentTime = ( TimeType( millis ) + TimeType( m_nextFrameOffset ) ) * TimeType( 0.001 );
        return m_currentTime;
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
