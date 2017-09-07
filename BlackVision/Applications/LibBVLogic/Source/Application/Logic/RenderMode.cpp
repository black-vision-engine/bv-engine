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
    :   m_renderMode( RenderingMode::RM_RenderRealTime )
    ,   m_editMode( false )
{}

// ***********************
//
RenderMode::~RenderMode()
{}

// ***********************
//
void        RenderMode::SetStartTime            ( unsigned long time )
{
    m_timer.SetStartTime( time );
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
    m_timer.SetOffscreenRenderMode( requestedFPS, numFrames );

    if( m_renderer )
    {
        m_renderer->SetVSync( false, 0 );
        m_renderer->SetFlushFinish( false, false );
    }
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
    auto time = m_timer.StartFrame( millis );

    // If offscreen rendering should end.
    if( m_renderMode == RenderingMode::RM_RenderOffscreen && m_timer.GetRenderingMode() == RenderingMode::RM_RenderRealTime )
    {
        m_renderer->SetVSync( !DefaultConfig.GetRendererInput().m_DisableVerticalSync, DefaultConfig.GetRendererInput().m_VerticalBufferFrameCount );
        m_renderer->SetFlushFinish( DefaultConfig.GetRendererInput().m_EnableGLFlush, DefaultConfig.GetRendererInput().m_EnableGLFinish );
    }

    return time;
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
