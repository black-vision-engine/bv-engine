#include "stdafx.h"

#include "OutputVideoContainer.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/CustomOutputs/OutputVideo.h"


namespace bv { namespace nrl {

// **************************
//
OutputVideoContainer::OutputVideoContainer    ( unsigned int widthHD, unsigned int heightHD )
    : OutputInstance( widthHD, heightHD )
{
}

// **************************
//
OutputVideoContainer::~OutputVideoContainer   ()
{
}

// **************************
//
void    OutputVideoContainer::ProcessFrameData        ( NRenderContext * ctx, RenderResult * input )
{
}

// **************************
//
OutputVideo *   OutputVideoContainer::GetVideoOutput          ( OutputVideoChannel ovc )
{
}

// **************************
//
void            OutputVideoContainer::RegisterOutput          ( OutputVideo * output, OutputVideoChannel ovc )
{
}

// **************************
//
bool            OutputVideoContainer::RemoveOutput            ( OutputVideoChannel ovc )
{
}

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

} //nrl
} //bv
