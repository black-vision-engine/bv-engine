#include "stdafx.h"

#include "NodeRenderLogic.h"

#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/SceneGraph/RenderableEntityWithBoundingBox.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Utils/RenderableEffectFactory.h"

#include "Engine/Audio/AudioRenderer.h"


namespace bv { 

// *********************************
//
void    NodeRenderLogic::Clear             ( const RenderTarget * rt, RenderContext * ctx, const glm::vec4 & color )
{
    enable( ctx, rt );
    clearBoundRT( ctx, color );
    disableBoundRT( ctx );
}

// *********************************
//
void    NodeRenderLogic::RenderAudio       ( Scene * scene, RenderContext * ctx, std::set< const audio::AudioEntity * > & audioEntities )

{
    auto rootNode = scene->GetRoot();

    if( rootNode )
    {
        RenderAudio( rootNode, aud( ctx ), audioEntities );
    }
}

// *********************************
//
void    NodeRenderLogic::RenderAudio       ( SceneNode * node, audio::AudioRenderer * audio, std::set< const audio::AudioEntity * > & audioEntities )
{
    // FIXME: nrl - and what about node effects???
    if ( node->IsVisible() )
    {
        auto nodeAudio = node->GetAudio();

        if( nodeAudio )
        {
            audio->Proccess( nodeAudio );
            audioEntities.insert( nodeAudio );
        }

        for( unsigned int i = 0; i < ( UInt32 )node->NumChildNodes(); ++i )
        {
            RenderAudio( node->GetChild( i ), audio, audioEntities );
        }
    }
}

// *********************************
//
void    NodeRenderLogic::RenderQueued      ( Scene * scene, const RenderTarget * output, RenderContext * ctx )
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
void    NodeRenderLogic::RenderQueued      ( SceneNode * node, RenderContext * ctx )
{
    // FIXME: nrl - remove this method and implement its logic in some other place (if necessary)
    auto queue = queue_allocator( ctx )->Allocate();
   
    queue->QueueNodeSubtree( node, ctx );
   
    queue->Render( ctx );

    queue_allocator( ctx )->Free();
}

// *********************************
//
void    NodeRenderLogic::Render            ( SceneNode * node, RenderContext * ctx )
{
    if( node->IsVisible() )
    {
        RenderImpl( node, ctx );
    }
}

// *********************************
//
void    NodeRenderLogic::Render            ( SceneNode * node, const RenderTarget * output, RenderContext * ctx )
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
void     NodeRenderLogic::RenderImpl      ( SceneNode * node, RenderContext * ctx )
{
    // FIXME: nrl transition
    auto nEffect = node->IsNodeEffectEnabled() ? node->GetNodeEffect() : nullptr;
    auto repr   = node->GetRepr();

    if( nEffect )
    {
        // FIXME: nrl - remove whem all effects are implemented in nrl
        //RenderImpl( repr, ctx );
        assert( nEffect->GetType() == NodeEffectType::NET_DEFAULT ||
				nEffect->GetType() == NodeEffectType::NET_ALPHA_MASK ||
				nEffect->GetType() == NodeEffectType::NET_NODE_MASK ||
				nEffect->GetType() == NodeEffectType::NET_BLUR ||
				nEffect->GetType() == NodeEffectType::NET_LIGHT_SCATTERING ||
				nEffect->GetType() == NodeEffectType::NET_SHADOW ||
                nEffect->GetType() == NodeEffectType::NET_Z_SORT ||
				nEffect->GetType() == NodeEffectType::NET_GLOW || 
				nEffect->GetType() == NodeEffectType::NET_SOFT_MASK
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
void    NodeRenderLogic::RenderGridLines   ( Scene * scene, RenderContext * ctx )
{
    if( scene->GetGridLinesVisibility() )
    {
        renderer( ctx )->Draw( scene->GetGridLines() );
    }    
}

// *********************************
//
void    NodeRenderLogic::Render            ( SceneNodeRepr * nodeRepr, const RenderTarget * output, RenderContext * ctx )
{
    enable( ctx, output );
    clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit

    RenderImpl( nodeRepr, ctx );
	renderer( ctx )->GenerateMipmaps( output );

    disableBoundRT( ctx );    
}

// ***********************
//
void    NodeRenderLogic::RenderQueued      ( SceneNodeRepr * nodeRepr, const RenderTarget * output, RenderContext * ctx )
{
    enable( ctx, output );
    clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit

    RenderQueued( nodeRepr, ctx );

    disableBoundRT( ctx );
}

// ***********************
//
void    NodeRenderLogic::RenderQueued      ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    // FIXME: nrl - remove this method and implement its logic in some other place (if necessary)
    auto queue = queue_allocator( ctx )->Allocate();

    queue->QueueNodeSubtree( nodeRepr, ctx );

    queue->Render( ctx );

    queue_allocator( ctx )->Free();
}

// *********************************
//
void    NodeRenderLogic::Render            ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    RenderImpl( nodeRepr, ctx );
}

// *********************************
//
void    NodeRenderLogic::RenderImpl        ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    RenderRoot( nodeRepr, ctx );

    RenderChildren( nodeRepr, ctx );
}

// *********************************
//
void    NodeRenderLogic::RenderRoot        ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    renderer( ctx )->Draw( renderable( nodeRepr ) );
}

// *********************************
//
void    NodeRenderLogic::RenderChildren    ( SceneNodeRepr * nodeRepr, RenderContext * ctx, unsigned int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) nodeRepr->NumChildNodes(); i++ )
    {
        Render( nodeRepr->GetChild( i ), ctx ); 
    }
}

// *********************************
//
void     NodeRenderLogic::RenderBoundingBox( SceneNode * node, RenderContext * ctx )
{
    // FIXME: nrl - a bit better initialization mechanics would be handy
    static auto effect = RenderableEffectFactory::CreateBoundingBoxEffect();
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


} // bv
