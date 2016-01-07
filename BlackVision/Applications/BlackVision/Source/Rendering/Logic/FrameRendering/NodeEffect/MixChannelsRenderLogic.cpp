#include "MixChannelsRenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/MixChannelsEffect.h"

#include "Rendering/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
MixChannelsRenderLogic::MixChannelsRenderLogic  ()
    : m_effect( nullptr )
{
}

// *********************************
//
MixChannelsRenderLogic::~MixChannelsRenderLogic ()
{
    delete m_effect;
}

// *********************************
//
void    MixChannelsRenderLogic::RenderNode      ( SceneNode * node, RenderLogicContext * ctx )
{   
    auto mixChannlesValue = node->GetNodeEffect()->GetValue( "channelMask" );
    auto mixMask = QueryTypedValue< ValueIntPtr >( mixChannlesValue )->GetValue();

    if( mixMask == 228 ) //FIXME: magic number (meaning that no mixing takes place
    {
        logic( ctx )->DrawNode( renderer( ctx ), node, ctx );
    }
    else
    {
        auto renderer = ctx->GetRenderer();
        auto rtAllocator    = ctx->GetRenderTargetAllocator();
        auto mainTarget = disableBoundRT( ctx );

        auto rt = rtAllocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
 
        enable( ctx, rt );

        clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

        logic( ctx )->DrawNode( renderer, node, ctx );

        disableBoundRT( ctx );
        rtAllocator->Free();

        enable( ctx, mainTarget );

        BlitWithMixchannels( renderer, rt, mixMask );
    }
}

// *********************************
//
void    MixChannelsRenderLogic::EnableMixChannelsEffect       ( Renderer * renderer, SceneNode * node )
{
    // FIXME: add IValues to the effect
    //auto pass = m_effect->GetPass( 0 );

    renderer->Enable( m_effect->GetPass( 0 ), static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
MixChannelsEffect *  MixChannelsRenderLogic::AccessMixChannelsEffect    ( RenderTarget * rt, int mixMask )
{
    if ( !m_effect )
    {
        auto rtTex = rt->ColorTexture( 0 );

        { mixMask; }
        //FIXME: texture
        m_effect = new MixChannelsEffect( rtTex );
    }

    //FIXME: mixChannelsMask
    //FIXME: vec4 float mask to multiplu each channel
    //m_blitEffect->SetAlpha( alpha );

    return m_effect;    
}

// *********************************
//
void    MixChannelsRenderLogic::BlitWithMixchannels             ( Renderer * renderer, RenderTarget * rt, int mixMask )
{
    auto effect = AccessMixChannelsEffect( rt, mixMask );

    { effect; renderer; }
}

} //bv
