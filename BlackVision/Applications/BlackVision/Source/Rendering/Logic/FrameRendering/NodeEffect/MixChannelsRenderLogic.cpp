#include "MixChannelsRenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/MixChannelsEffect.h"

#include "Rendering/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
MixChannelsRenderLogic::MixChannelsRenderLogic  ()
    : m_effect( new MixChannelsEffect() )
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
    
    logic( ctx )->DrawNode( renderer( ctx ), node, ctx );

//    RenderWireframeOverlay( renderer( ctx ), node );
}

// *********************************
//
void    MixChannelsRenderLogic::EnableMixChannelsEffect       ( Renderer * renderer, SceneNode * node )
{
    // FIXME: add IValues to the effect
    //auto pass = m_effect->GetPass( 0 );

    renderer->Enable( m_effect->GetPass( 0 ), static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

} //bv
