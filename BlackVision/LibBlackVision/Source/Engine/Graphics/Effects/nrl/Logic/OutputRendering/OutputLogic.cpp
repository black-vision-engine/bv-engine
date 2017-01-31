#include "stdafx.h"

#include "OutputLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

// *********************************
//
OutputLogic::OutputLogic                                ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargetsPerOutput )
    : m_outputs( (unsigned int) CustomOutputType::COT_TOTAL )
    , m_renderResult( allocator, numTrackedRenderTargetsPerOutput )
{
    unsigned int i = 0;

    //m_outputs[ (unsigned int) CustomOutputType::COT_PREVIEW ]       = new OutputPreview         ( width, height );          ++i;
    //m_outputs[ (unsigned int) CustomOutputType::COT_VIDEO_HD ]      = new OutputVideo           ( width, height );          ++i;
    //m_outputs[ (unsigned int) CustomOutputType::COT_VIDEO_SD ]      = new OutputVideo           ( width / 2, height / 2 );  ++i; // FIXME: nrl - pass arguments using appropriate descriptor (static data init)
    //m_outputs[ (unsigned int) CustomOutputType::COT_STREAM_SHM ]    = new OutputStreamSharedMem ( width / f, height / f );  ++i; // FIXME: nrl - pass arguments using appropriate descriptor (static data init)
    //m_outputs[ (unsigned int) CustomOutputType::COT_SCREENSHOT ]    = new OutputScreenshot      ( width, height  );         ++i;

    //assert( i == (unsigned int) CustomOutputType::COT_TOTAL );

    //// FIXME: nrl - default output channels configuration - should be read from some static config descriptor
    //{
    //    // Preview
    //    {
    //        auto & state = m_outputs[ 0 ]->AccessOutputState();
 
    //        state.SetActiveRenderChannel( RenderChannelType::RCT_OUTPUT_1 );

    //        state.SetRIdx( 0 );
    //        state.SetGIdx( 1 );
    //        state.SetBIdx( 2 );
    //        state.SetAIdx( 3 );
    //    }

    //    // Video HD
    //    {
    //        auto & state = m_outputs[ 1 ]->AccessOutputState();
 
    //        state.SetActiveRenderChannel( RenderChannelType::RCT_OUTPUT_1 );
    //        
    //        state.SetRIdx( 0 );
    //        state.SetGIdx( 1 );
    //        state.SetBIdx( 2 );
    //        state.SetAIdx( 3 );
    //    }

    //    // Video SD
    //    {
    //        auto & state = m_outputs[ 2 ]->AccessOutputState();
 
    //        state.SetActiveRenderChannel( RenderChannelType::RCT_OUTPUT_1 );
    //        
    //        state.SetRIdx( 0 );
    //        state.SetGIdx( 1 );
    //        state.SetBIdx( 2 );
    //        state.SetAIdx( 3 );
    //    }

    //    // SHM
    //    {
    //        auto & state = m_outputs[ 3 ]->AccessOutputState();
 
    //        state.SetActiveRenderChannel( RenderChannelType::RCT_OUTPUT_1 );
    //        
    //        state.SetRIdx( 0 );
    //        state.SetGIdx( 1 );
    //        state.SetBIdx( 2 );
    //        state.SetAIdx( 3 );
    //    }

    //    // Screenshot
    //    {
    //        auto & state = m_outputs[ 4 ]->AccessOutputState();
 
    //        state.SetActiveRenderChannel( RenderChannelType::RCT_OUTPUT_1 );
    //        
    //        state.SetRIdx( 0 );
    //        state.SetGIdx( 1 );
    //        state.SetBIdx( 2 );
    //        state.SetAIdx( 3 );
    //    }
    //}
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
RenderChannelType   OutputLogic::GetActiveRenderChannel  ( CustomOutputType outputType ) const
{
    return m_outputs[ (unsigned int) outputType ]->GetActiveRenderChannel();
}

// *********************************
//
bool                OutputLogic::SetActiveRenderChannel  ( CustomOutputType outputType, RenderChannelType rct )
{
    bool res = false;

    if( m_renderResult.IsActive( rct ) )
    {
        auto & state = GetOutput( outputType )->AccessOutputState();

        state.SetActiveRenderChannel( rct );

        res = true;
    }

    return res;
}

// *********************************
//
Output *            OutputLogic::GetOutput              ( CustomOutputType outputType )
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
