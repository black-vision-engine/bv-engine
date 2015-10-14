#include "NodeEffectRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Tools/Profiler/HerarchicalProfiler.h"


namespace bv {

// *********************************
//
NodeEffectRenderLogic::NodeEffectRenderLogic   ()
{
}

// *********************************
//
NodeEffectRenderLogic::~NodeEffectRenderLogic  ()
{
}

// *********************************
//
void    NodeEffectRenderLogic::DrawNode        ( Renderer * renderer, SceneNode * node, OffscreenRenderLogic * offscreenRenderLogic )
{
	HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
    DrawNodeOnly( renderer, node );
    DrawChildren( renderer, node, 0, offscreenRenderLogic );
}

// *********************************
//
void    NodeEffectRenderLogic::DrawNodeOnly   ( Renderer * renderer, SceneNode * node )
{
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    NodeEffectRenderLogic::DrawChildren   ( Renderer * renderer, SceneNode * node, int firstChildIdx, OffscreenRenderLogic * offscreenRenderLogic )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
        RenderNode( renderer, node->GetChild( i ), offscreenRenderLogic ); 
    }
}

} //bv
