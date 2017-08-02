#include "stdafxBVApp.h"

#include "RenderMode.h"

#include "Engine/Graphics/Effects/Logic/RenderLogic.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/OutputLogic.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/FrameDataHandlers/AVFileOutput/AVFileOutput.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "BVConfig.h"

namespace bv
{
    
// ***********************
//
RenderMode::RenderMode()
    :   m_startTime( 0 )
    ,   m_frameNumber( 0 )
    ,   m_currentTime( 0 )
    ,   m_renderMode( RenderingMode::RM_RenderRealTime )
    ,   m_editMode( false )
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
    m_realTime = m_currentTime;
    m_frameNumber = 0;
}

// ***********************
//
void        RenderMode::SetRenderToFileMode     ( const std::string & filePath, float requestedFPS, UInt64 numFrames )
{
    SetOffscreenRenderMode( requestedFPS, numFrames );

    if( m_renderLogic )
    {
        auto outputLogic = m_renderLogic->GetOutputLogic();
        outputLogic->RequestScreenshot( filePath, RenderChannelType::RCT_OUTPUT_1, (unsigned int)numFrames, false );
    }
}

// ***********************
//
void        RenderMode::SetOffscreenRenderMode  ( float requestedFPS, UInt64 numFrames )
{
    m_nextFrameOffset = TimeType( 1 / requestedFPS );
    m_framesToRender = numFrames;

    if( m_renderer )
    {
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
        auto outputLogic = m_renderLogic->GetOutputLogic();
        outputLogic->RequestScreenshot( filePath, RenderChannelType::RCT_OUTPUT_1, 1, onRenderedEvent, asyncWrite );
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
        {
            // Offscreen Rendering ended. Restore previous state.
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

// ***********************
//
void        RenderMode::SwitchToEditMode            ( bool mode )
{
    if( m_editMode != mode )
    {
        m_editMode = mode;
        m_renderLogic->SwitchEditMode( mode );
    }
}

// ***********************
//
bool        RenderMode::IsEditMode                  () const
{
    return m_editMode;
}

// ***********************
//
void        RenderMode::StartToAVFileRendering      ( const std::string & outputFilePath )
{
    if( m_renderLogic  )
    {
        auto outputLogic = m_renderLogic->GetOutputLogic();

        auto avfileOutput = outputLogic->GetOutput( CustomOutputType::COT_AV_FILE );

        if( avfileOutput )
        {
            assert( dynamic_cast< AVFileOutput * >( avfileOutput ) );
            static_cast< AVFileOutput * >( avfileOutput )->StartToAVFileRendering( outputFilePath );
        }
    }
}

// ***********************
//
void        RenderMode::StopToAVFileRendering       ()
{
    if( m_renderLogic  )
    {
        auto outputLogic = m_renderLogic->GetOutputLogic();

        auto avfileOutput = outputLogic->GetOutput( CustomOutputType::COT_AV_FILE );

        if( avfileOutput )
        {
            assert( dynamic_cast< AVFileOutput * >( avfileOutput ) );
            static_cast< AVFileOutput * >( avfileOutput )->StopToAVFileRendering();
        }
    }
}

} //bv
