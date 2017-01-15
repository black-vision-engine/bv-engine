#include "stdafx.h"

#include "OutputLogic.h"

#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

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
void    OutputLogic::ProcessFrameData    ( NRenderContext * ctx  )
{
    // FIXME: this is kinda hackish
    auto data = m_renderResult;
    if( data->IsActive( RenderOutputChannelType::ROCT_OUTPUT_1 ) && data->!ContainsValidData( RenderOutputChannelType::ROCT_OUTPUT_1 ) )
    {
        auto outputRT = data->GetActiveRenderTarget( RenderOutputChannelType::ROCT_OUTPUT_1 );

        // FIXME: nrl - this is so low level that should be implented in an addtional layer (not in NNodeRenderLogic per se)
        NNodeRenderLogic::Clear( outputRT, ctx ); 
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
