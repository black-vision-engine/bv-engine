#include "SplitChildrenPreFullscreenLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"


namespace bv {

// *********************************
//
SplitChildrenPreFullscreenLogic::SplitChildrenPreFullscreenLogic        ()
    : m_firstChild( 0 )
{
}

// *********************************
//
void    SplitChildrenPreFullscreenLogic::SetFirstChild                   ( unsigned int i )
{
    m_firstChild = i;
}

// *********************************
//
void    SplitChildrenPreFullscreenLogic::Render                          ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs )
{
    assert( m_firstChild + outputs.size() <= node->NumChildNodes() );
    assert( ctx->GetBoundRenderTarget() == nullptr );

    unsigned int i = m_firstChild;

    for( auto rt : outputs )
    {
        enable( ctx, rt );
        clearBoundRT( ctx, glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );

        logic( ctx )->RenderNode( node->GetChild( i++ ), ctx );

        disable( ctx, rt );
    }
}

} //bv
