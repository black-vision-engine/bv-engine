#include "stdafx.h"

#include "OutputLogic.h"

#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Preview.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/VideoOutput.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

// *********************************
//
OutputLogic::OutputLogic         ()
    : m_outputs( (unsigned int) CustomOutputType::COT_TOTAL )
{
}

// *********************************
//
OutputLogic::~OutputLogic        ()
{
}

// *********************************
//
void    OutputLogic::ProcessFrameData    ( NRenderContext * ctx, const RenderResult * data )
{
    // FIXME: this is kinda hackish
    if( numScenes == 0 )
    {
        auto renderTarget = data->GetActiveRenderTarget( RenderOutputChannelType::ROCT_OUTPUT_1 );

        enable( ctx, renderTarget );
        clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit
        disableBoundRT( ctx );
    }

    // FIXME: nrl - add screenshot logic somewhere near this line of code - based on previous implementation
    //if( m_screenShotLogic->ReadbackNeeded() )
    //{
    //    auto rt = m_offscreenDisplay->GetCurrentFrameRenderTarget();
    //    m_screenShotLogic->FrameRendered( rt, ctx );
    //}

    m_preview->ShowFrame( ctx, data );

    // FIXME: temporary
    if( m_videoOutput )
    {
        m_videoOutput->HandleFrame( ctx, data );
    }
}

// *********************************
//
Preview *       OutputLogic::GetPreview          ()
{
    return m_preview;
}

// *********************************
//
VideoOutput *   OutputLogic::GetVideoOutput      ()
{
    return m_videoOutput;
}

} //nrl
} //bv
