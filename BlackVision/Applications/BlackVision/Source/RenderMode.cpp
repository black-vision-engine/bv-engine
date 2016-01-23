#include "RenderMode.h"

#include "Rendering/Logic/RenderLogic.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "BVConfig.h"

namespace bv
{
    
// ***********************
//
RenderMode::RenderMode()
    :   m_startTime( 0 ),
        m_frameNumber( 0 ),
        m_currentTime( 0 ),
        m_renderMode( RenderingMode::RenderRealTime )
{
	m_fps = ConfigManager::GetInt( "Renderer/TimerFPS" );

	if( m_fps == 0 )
	{
		m_fps = 25;
	}
}

// ***********************
//
RenderMode::~RenderMode()
{
}

// ***********************
//
void RenderMode::SetStartTime( unsigned long time )
{
	unsigned long d = unsigned long( 1000.f / float( m_fps ) );
    m_startTime = time - time % d;
    m_currentTime = TimeType( m_startTime ) * TimeType( 0.001 );
    m_frameNumber = 0;
}

// ***********************
//
void RenderMode::SetRenderToFileMode( const std::string & filePath, float requestedFPS, unsigned int numFrames )
{
    m_nextFrameOffset = TimeType( 1 / requestedFPS );
    m_framesToRender = numFrames;

    if( m_renderLogic )
	{
        m_renderLogic->MakeScreenShot( filePath, numFrames );
	}

    if( m_renderer )
    {
        m_renderer->SetVSync( false, 0 );
        m_renderer->SetFlushFinish( false, false );
    }

    m_renderMode = RenderingMode::RenderOffscreen;
    m_currentTime = 0.0f;
}

// ***********************
//
void RenderMode::MakeScreenShot( const std::string & filePath )
{
    if( m_renderLogic )
	{
        m_renderLogic->MakeScreenShot( filePath, 1 );
	}
}

// ***********************
//
TimeType RenderMode::StartFrame( unsigned long millis )
{
	unsigned long d = unsigned long( 1000.f / float( m_fps ) );
	millis = millis - millis % d;

    m_frameNumber++;
    m_realTime = TimeType( millis ) * TimeType( 0.001 );

    if( m_renderMode == RenderingMode::RenderRealTime )
    {
        m_currentTime = m_realTime;
        return m_currentTime;
    }
    else if( m_renderMode == RenderingMode::RenderOffscreen )
    {
        if( m_framesToRender == 0 )
        {// Rendering to file ended. Restore previous state.
            m_renderMode = RenderingMode::RenderRealTime;

            m_renderer->SetVSync( !BVConfig::Instance().GetRendererInput().m_DisableVerticalSync, BVConfig::Instance().GetRendererInput().m_VerticalBufferFrameCount );
            m_renderer->SetFlushFinish( BVConfig::Instance().GetRendererInput().m_EnableGLFlush, BVConfig::Instance().GetRendererInput().m_EnableGLFinish );

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
TimeType	RenderMode::GetFramesDelta				() const
{
	return 1.f / (float) m_fps;
}

} //bv
