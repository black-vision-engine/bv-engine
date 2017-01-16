#include "stdafx.h"

#include "OutputLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputPreview.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputVideo.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputStream.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputScreenshot.h"


namespace bv { namespace nrl {

// *********************************
//
OutputLogic::OutputLogic                    ()
    : m_outputs( (unsigned int) CustomOutputType::COT_TOTAL )
{
    unsigned int i = 0;

    m_outputs[ (unsigned int) CustomOutputType::COT_PREVIEW ]       = new OutputPreview();      ++i;
    m_outputs[ (unsigned int) CustomOutputType::COT_VIDEO ]         = new OutputVideo();        ++i;
    m_outputs[ (unsigned int) CustomOutputType::COT_STREAM ]        = new OutputStream();       ++i;
    m_outputs[ (unsigned int) CustomOutputType::COT_SCREENSHOT ]    = new OutputScreenshot();   ++i;

    assert( i == (unsigned int) CustomOutputType::COT_TOTAL );
}

// *********************************
//
OutputLogic::~OutputLogic                   ()
{
    for( auto output : m_outputs )
    {
        delete output;
    }
}

// *********************************
//
void    OutputLogic::UpdateOutputChannels   ()
{
    m_renderResult->UpdateOutputChannels();
}

// *********************************
// FIXME: nrl - add audio somewhere in this class
void    OutputLogic::ProcessFrameData       ( NRenderContext * ctx  )
{
    // FIXME: this is kinda hackish
    auto data = m_renderResult;

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
Preview *       OutputLogic::GetPreview     ()
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
