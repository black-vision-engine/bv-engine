#include "stdafxBVApp.h"

#include "RenderMode.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogic.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputLogic.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputScreenshot.h"
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
        m_renderMode( RenderingMode::RM_RenderRealTime )
{
    m_fps = DefaultConfig.TimerFPS();
}

// ***********************
//
RenderMode::~RenderMode()
{}

// ***********************
//
void        RenderMode::SetStartTime            ( unsigned long time )
{
	unsigned long d = unsigned long( 1000.f / float( m_fps ) );
    m_startTime = time - time % d;
    m_currentTime = TimeType( m_startTime ) * TimeType( 0.001 );
    m_frameNumber = 0;
}

// ***********************
//
void        RenderMode::SetRenderToFileMode     ( const std::string & filePath, float requestedFPS, unsigned int numFrames )
{
    m_nextFrameOffset = TimeType( 1 / requestedFPS );
    m_framesToRender = numFrames;

    if( m_renderLogic && m_renderer )
    {
        auto screenshotLogic = GetScreenshotLogic();
        screenshotLogic->AccessOutputState().SetActiveRenderChannel( nrl::RenderChannelType::RCT_OUTPUT_1 );

        screenshotLogic->MakeScreenShot( filePath, numFrames, false );

        m_renderer->SetVSync( false, 0 );
        m_renderer->SetFlushFinish( false, false );
    }

    m_renderMode = RenderingMode::RM_RenderOffscreen;
    m_currentTime = m_realTime;
}

// ***********************
//
void        RenderMode::MakeScreenShot          ( const std::string & filePath, bool onRenderedEvent, bool asyncWrite )
{
    if( m_renderLogic && m_renderer )
    {
        auto screenshotLogic = GetScreenshotLogic();
        screenshotLogic->AccessOutputState().SetActiveRenderChannel( nrl::RenderChannelType::RCT_OUTPUT_1 );

        screenshotLogic->MakeScreenShot( filePath, 1, onRenderedEvent, asyncWrite );
    }
}

// ***********************
//
TimeType    RenderMode::StartFrame              ( unsigned long millis )
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
        {// Rendering to file ended. Restore previous state.
            m_renderMode = RenderingMode::RM_RenderRealTime;

            m_renderer->SetVSync( !DefaultConfig.GetRendererInput().m_DisableVerticalSync, DefaultConfig.GetRendererInput().m_VerticalBufferFrameCount );
            m_renderer->SetFlushFinish( DefaultConfig.GetRendererInput().m_EnableGLFlush, DefaultConfig.GetRendererInput().m_EnableGLFinish );

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

nrl::OutputScreenshot *     RenderMode::GetScreenshotLogic()
{
    auto outputLogic = m_renderLogic->GetOutputLogic();
    return static_cast< nrl::OutputScreenshot * >( outputLogic->GetOutput( nrl::CustomOutputType::COT_SCREENSHOT ) );
}

} //bv
