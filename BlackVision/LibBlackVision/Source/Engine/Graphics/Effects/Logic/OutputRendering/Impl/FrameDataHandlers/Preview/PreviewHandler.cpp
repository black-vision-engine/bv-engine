#include "stdafx.h"

#include "PreviewHandler.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"


namespace bv { 
   
// **************************
//
PreviewHandler::PreviewHandler      ()
    : m_mixChannelsEffect( nullptr )
    , m_activeRenderOutput( 1 )
{
    m_mixChannelsEffect = CreateFullscreenEffect( FullscreenEffectType::NFET_MIX_CHANNELS );
}

// **************************
//
PreviewHandler::~PreviewHandler     ()
{
    delete m_mixChannelsEffect;
}

// **************************
//
void                                PreviewHandler::HandleFrameData     ( const OutputState &, RenderContext * ctx, const RenderChannel * channel )
{
    m_activeRenderOutput.SetEntry( 0, channel->GetActiveRenderTarget() );

    // FIXME: nrl - DefaultShow is only a very siple way of showing rendered result on preview - ask Pawelek about other possibilities
    m_mixChannelsEffect->Render( ctx, m_activeRenderOutput );

    // Make sure that local preview is displayed properly
    renderer( ctx )->DisplayColorBuffer();
}

// **************************
//
FullscreenEffectComponentStatePtr  PreviewHandler::GetInternalFSEState ()
{
    return m_mixChannelsEffect->GetState();
}


} //bv
