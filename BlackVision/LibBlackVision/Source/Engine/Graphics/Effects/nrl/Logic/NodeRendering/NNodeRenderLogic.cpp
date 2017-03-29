#include "stdafx.h"

#include "NNodeRenderLogic.h"

#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/SceneGraph/RenderableEntityWithBoundingBox.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/BoundingBoxEffect.h"

#include "Engine/Audio/AudioRenderer.h"


namespace bv { namespace nrl {

// *********************************
//
void    NNodeRenderLogic::Clear             ( const RenderTarget * rt, NRenderContext * ctx, const glm::vec4 & color )
{
    enable( ctx, rt );
    clearBoundRT( ctx, color );
    disableBoundRT( ctx );
}

// *********************************
//
void    NNodeRenderLogic::RenderAudio       ( Scene * scene, NRenderContext * ctx )
{
    auto rootNode = scene->GetRoot();

    if( rootNode )
    {
        RenderAudio( rootNode, audio( ctx ) );
    }
}

// *********************************
//
void    NNodeRenderLogic::RenderAudio       ( SceneNode * node, audio::AudioRenderer * audio )
{
    // FIXME: nrl - and what about node effects???
    if ( node->IsVisible() )
    {
        auto nodeAudio = node->GetAudio();

        if( nodeAudio )
        {
            audio->Proccess( nodeAudio );
        }

        for( unsigned int i = 0; i < ( UInt32 )node->NumChildNodes(); ++i )
        {
            RenderAudio( node->GetChild( i ), audio );
        }
    }
}

// *********************************
//
void    NNodeRenderLogic::RenderQueued      ( Scene * scene, const RenderTarget * output, NRenderContext * ctx )
{
    auto renderer = ctx->GetRenderer();

    renderer->Performance().AverageScenePerformanceData( scene );
    renderer->SetCamera( scene->GetCamera() );

    // FIXME: nrl - reimplement it (do not use/preferably remove EnableScene which contains logic <implemented in renderer> 
    // FIXME: nrl - which does not belong there)
    renderer->EnableScene( scene );

    enable( ctx, output );

    RenderQueued( scene->GetRoot(), ctx );

    // FIXME: nrl - implement a generic solution when other editor helper object apear in engine
    RenderGridLines( scene, ctx );

    disableBoundRT( ctx );
}

// *********************************
//
void    NNodeRenderLogic::RenderQueued      ( SceneNode * node, NRenderContext * ctx )
{
    // FIXME: nrl - remove this method and implement its logic in some other place (if necessary)
    auto queue = queue_allocator( ctx )->Allocate();
   
    queue->QueueNodeSubtree( node, ctx );
   
    queue->Render( ctx );

    queue_allocator( ctx )->Free();
}

// *********************************
//
void    NNodeRenderLogic::Render            ( SceneNode * node, NRenderContext * ctx )
{
    if( node->IsVisible() )
    {
        RenderImpl( node, ctx );
    }
}

// *********************************
//
void    NNodeRenderLogic::Render            ( SceneNode * node, const RenderTarget * output, NRenderContext * ctx )
{
    if ( node->IsVisible() )
    {
        enable( ctx, output );
        clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit

        RenderImpl( node, ctx );

        disableBoundRT( ctx );
    }
}

// *********************************
//
void     NNodeRenderLogic::RenderImpl      ( SceneNode * node, NRenderContext * ctx )
{
    // FIXME: nrl transition
    auto nEffect = node->IsNNodeEffectEnabled() ? node->GetNNodeEffect() : nullptr;
    auto repr   = node->GetRepr();

    if( nEffect )
    {
        // FIXME: nrl - remove whem all effects are implemented in nrl
        //RenderImpl( repr, ctx );
        assert( nEffect->GetType() == nrl::NNodeEffectType::NNET_DEFAULT ||
				nEffect->GetType() == nrl::NNodeEffectType::NNET_ALPHA_MASK ||
				nEffect->GetType() == nrl::NNodeEffectType::NNET_NODE_MASK ||
				nEffect->GetType() == nrl::NNodeEffectType::NNET_BLUR ||
				nEffect->GetType() == nrl::NNodeEffectType::NNET_LIGHT_SCATTERING ||
				nEffect->GetType() == nrl::NNodeEffectType::NNET_SHADOW ||
                nEffect->GetType() == nrl::NNodeEffectType::NNET_Z_SORT ||
				nEffect->GetType() == nrl::NNodeEffectType::NNET_GLOW || 
				nEffect->GetType() == nrl::NNodeEffectType::NNET_SOFT_MASK
				);

        nEffect->Render( repr, ctx ); //FIXME: test and implement
    }
    else 
    {
        // FIXME: nrl - transition implementations
        assert( false );
    }
}

// *********************************
//
void    NNodeRenderLogic::RenderGridLines   ( Scene * scene, NRenderContext * ctx )
{
    if( scene->GetGridLinesVisibility() )
    {
        renderer( ctx )->Draw( scene->GetGridLines() );
    }    
}

// *********************************
//
void    NNodeRenderLogic::Render            ( SceneNodeRepr * nodeRepr, const RenderTarget * output, NRenderContext * ctx )
{
    enable( ctx, output );
    clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit

    RenderImpl( nodeRepr, ctx );
	renderer( ctx )->GenerateMipmaps( output );

    disableBoundRT( ctx );    
}

// ***********************
//
void    NNodeRenderLogic::RenderQueued      ( SceneNodeRepr * nodeRepr, const RenderTarget * output, NRenderContext * ctx )
{
    enable( ctx, output );
    clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit

    RenderQueued( nodeRepr, ctx );

    disableBoundRT( ctx );
}

// ***********************
//
void    NNodeRenderLogic::RenderQueued      ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    // FIXME: nrl - remove this method and implement its logic in some other place (if necessary)
    auto queue = queue_allocator( ctx )->Allocate();

    queue->QueueNodeSubtree( nodeRepr, ctx );

    queue->Render( ctx );

    queue_allocator( ctx )->Free();
}

// *********************************
//
void    NNodeRenderLogic::Render            ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    RenderImpl( nodeRepr, ctx );
}

// *********************************
//
void    NNodeRenderLogic::RenderImpl        ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    RenderRoot( nodeRepr, ctx );

    RenderChildren( nodeRepr, ctx );
}

// *********************************
//
void    NNodeRenderLogic::RenderRoot        ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    renderer( ctx )->Draw( renderable( nodeRepr ) );
}

// *********************************
//
void    NNodeRenderLogic::RenderChildren    ( SceneNodeRepr * nodeRepr, NRenderContext * ctx, unsigned int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) nodeRepr->NumChildNodes(); i++ )
    {
        Render( nodeRepr->GetChild( i ), ctx ); 
    }
}

// *********************************
//
void     NNodeRenderLogic::RenderBoundingBox( SceneNode * node, NRenderContext * ctx )
{
    // FIXME: nrl - a bit better initialization mechanics would be handy
    static auto effect = std::make_shared< BoundingBoxEffect >();
    static auto pass   = effect->GetPass( 0 );

    const auto & color = node->GetBoundingBoxColor();
    
    // FIXME: nrl - rly "RenderableEntityWithBoundingBox"?
    auto obj = Cast< RenderableEntityWithBoundingBox * >( node->GetTransformable() );
    assert( obj );

    auto bb = obj->GetBoundingBox();
    if( bb )
    {
        auto renderer = ctx->GetRenderer();

        auto param = Cast< ShaderParamVec4 * >( pass->GetPixelShader()->GetParameters()->AccessParam( "color" ) );
        param->SetValue( color );

        renderer->Enable( pass, bb );
        renderer->DrawRenderable( bb );

        auto wc = obj->GetCenterOfMass();
        renderer->Enable( pass, wc );
        renderer->DrawRenderable( wc );
    }

}

} // nrl
} // bv
