#include "stdafx.h"

#include "OutputLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Preview.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/VideoOutput.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"

namespace bv { namespace nrl {

// *********************************
//
OutputLogic::OutputLogic         ()
    : m_preview( nullptr )
    , m_videoOutput( nullptr )
{
    m_preview = new Preview();
}

// *********************************
//
OutputLogic::~OutputLogic        ()
{
    delete m_preview;
    delete m_videoOutput;
}

// *********************************
//
void    OutputLogic::ProcessFrameData    ( NRenderContext * ctx, const RenderResult * data, unsigned int numScenes )
{
    if( numScenes == 0 )
    {
        auto renderTarget = data->GetActiveRenderTarget( RenderOutputChannelType::ROCT_OUTPUT_1 );

        enable( ctx, renderTarget );
        clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit
        disableBoundRT( ctx );
    }

    m_preview->ShowFrame( ctx, data );

    // FIXME: temporary
    if( m_videoOutput )
    {
        m_videoOutput->HandleFrame( data );
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
