#include "stdafx.h"

#include "PreviewHandler.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"


namespace bv { namespace nrl {
   
// **************************
//
PreviewHandler::PreviewHandler      ()
    : m_mixChannelsEffect( nullptr )
    , m_activeRenderOutput( 1 )
{
    m_mixChannelsEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_MIX_CHANNELS );
}

// **************************
//
PreviewHandler::~PreviewHandler     ()
{
    delete m_mixChannelsEffect;
}

// **************************
//
void                                PreviewHandler::HandleFrameData     ( const NOutputState &, NRenderContext * ctx, const RenderChannel * channel )
{
    m_activeRenderOutput.SetEntry( 0, channel->GetActiveRenderTarget() );

    // FIXME: nrl - DefaultShow is only a very siple way of showing rendered result on preview - ask Pawelek about other possibilities
    m_mixChannelsEffect->Render( ctx, m_activeRenderOutput );

    // Make sure that local preview is displayed properly
    renderer( ctx )->DisplayColorBuffer();
}

// **************************
//
NFullscreenEffectComponentStatePtr  PreviewHandler::GetInternalFSEState ()
{
    return m_mixChannelsEffect->GetState();
}

} //nrl
} //bv
