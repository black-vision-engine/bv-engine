#include "DrawLogic.h"

#include "Tools/Profiler/HerarchicalProfiler.h"

#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv {

// *********************************
//
void    DrawLogic::DrawNode     ( Renderer * renderer, SceneNode * node )
{
    { renderer; node; }
	//HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );

 //   DrawNodeOnly( renderer, node );

 //   DrawChildren( renderer, node );
}

// *********************************
//
void    DrawLogic::DrawNodeOnly ( Renderer * renderer, SceneNode * node )
{
    { renderer; node; }
//    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    DrawLogic::DrawChildren ( RenderLogic * renderLogic, SceneNode * node, int firstChildIdx )
{
    { renderLogic; node; firstChildIdx; }
    //for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
    //{
    //    HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
    //    RenderNode  ( renderer, node->GetChild( i ) ); 
    //}
}

} //bv
