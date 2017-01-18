#include "stdafx.h"

#include "OutputLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputPreview.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputVideo.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputStreamSharedMem.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputScreenshot.h"


namespace bv { namespace nrl {

// *********************************
//
OutputLogic::OutputLogic                                ( unsigned int width, unsigned int height, unsigned int shmScaleFactor, RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutput )
    : m_outputs( (unsigned int) CustomOutputType::COT_TOTAL )
    , m_renderResult( allocator, numTrackedRenderTargetsPerOutput )
{
    unsigned int i = 0;

    m_outputs[ (unsigned int) CustomOutputType::COT_PREVIEW ]       = new OutputPreview();          ++i;
    m_outputs[ (unsigned int) CustomOutputType::COT_VIDEO ]         = new OutputVideo();            ++i;
    m_outputs[ (unsigned int) CustomOutputType::COT_STREAM_SHM ]    = new OutputStreamSharedMem( width, height, shmScaleFactor );  ++i; // FIXME: nrl - pass arguments using appropriate descriptor (static data init)
    m_outputs[ (unsigned int) CustomOutputType::COT_SCREENSHOT ]    = new OutputScreenshot();       ++i;

    assert( i == (unsigned int) CustomOutputType::COT_TOTAL );
}

// *********************************
//
OutputLogic::~OutputLogic                               ()
{
    for( auto output : m_outputs )
    {
        delete output;
    }
}

// ***************************************************** API directly related to frame rendering *****************************************************

// *********************************
// FIXME: nrl - add audio somewhere in this class
void                OutputLogic::ProcessFrameData       ( NRenderContext * ctx )
{
    auto result = &m_renderResult;

    for( auto output : m_outputs)
    {
        if( output && output->IsEnabled() )
        {
            output->ProcessFrameData( ctx, result ); 
        }
    }
}

// *********************************************** API relarted to global output state manipulation **************************************************

// *********************************
//
bool                OutputLogic::IsEnabled              ( CustomOutputType outputType )
{
    return GetOutput( outputType )->IsEnabled();
}

// *********************************
//
void                OutputLogic::EnableOutput           ( CustomOutputType outputType )
{
    GetOutput( outputType )->Enable();
}

// *********************************
//
void                OutputLogic::DisableOutput          ( CustomOutputType outputType )
{
    GetOutput( outputType )->Disable();
}

// *********************************
//
OutputInstance *    OutputLogic::GetOutput              ( CustomOutputType outputType )
{
    return m_outputs[ (unsigned int) outputType ];
}

// *********************************************** API related to render buffers state manipulation **************************************************

// *********************************
//
RenderResult *      OutputLogic::AccessRenderResult     ()
{
    return &m_renderResult;
}

// *********************************
//
void                OutputLogic::ActivateRenderChannel  ( RenderChannelType rct )
{
    m_renderResult.SetIsActive( rct, true );
}

// *********************************
//
void                OutputLogic::DeactivateRenderChannel( RenderChannelType rct )
{
    m_renderResult.SetIsActive( rct, false );
}

// *********************************
//
void                OutputLogic::UpdateRenderChannels   ()
{
    m_renderResult.UpdateRenderChannels();
}

} //nrl
} //bv
