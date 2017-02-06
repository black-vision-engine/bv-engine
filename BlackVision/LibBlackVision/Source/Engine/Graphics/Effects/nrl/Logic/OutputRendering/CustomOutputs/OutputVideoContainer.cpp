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

} //nrl
} //bv
