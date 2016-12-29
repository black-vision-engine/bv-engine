#include "NNodeRenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

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
    auto nEffect = node->GetNNodeEffect();
    auto repr   = node->GetRepr();

    if( nEffect )
    {
        //RenderImpl( repr, ctx );
        assert( nEffect->GetType() == nrl::NNodeEffectType::NNET_DEFAULT || nEffect->GetType() == nrl::NNodeEffectType::NNET_ALPHA_MASK );

        nEffect->Render( repr, ctx ); //FIXME: test and implement
    }
    else 
    {   //Transition implementations
        auto effect = node->GetNodeEffect();

        if( !effect || effect->GetType() ==  NodeEffectType::NET_DEFAULT )
        {
            RenderImpl( repr, ctx );
        }
        else
        {
            effect->Render( repr, ctx );
        }
    }
}

// *********************************
//
void    NNodeRenderLogic::Render            ( SceneNodeRepr * nodeRepr, const RenderTarget * output, NRenderContext * ctx )
{
    enable( ctx, output );
    clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit

    RenderImpl( nodeRepr, ctx );

    disableBoundRT( ctx );    
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

} // nrl
} // bv
