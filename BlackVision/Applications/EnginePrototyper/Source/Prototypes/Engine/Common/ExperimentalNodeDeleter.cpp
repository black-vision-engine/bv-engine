#include "ExperimentalNodeDeleter.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"


namespace bv
{

// **************************
//
void    ExperimentalDeleteAndCleanup( SceneNode * node, Renderer * renderer )
{
    std::vector< SceneNode * > nodes;
    for( unsigned int i = 0; i < node->NumChildNodes(); ++i )
    {
        nodes.push_back( node->GetChild( i ) );
    }

    while( node->NumChildNodes() > 0 )
    {
        node->DetachChildNode( 0 );
    }

    for( auto node : nodes )
    {
        ExperimentalDeleteAndCleanup( node, renderer );
    }

    ExperimentalDeleteSingleNode( node, renderer );
}

// **************************
//
void    ExperimentalDeleteSingleNode( SceneNode * node, Renderer * renderer )
{
    assert( node->NumChildNodes() == 0 );

    ExperimentalFreePdrResources( static_cast< RenderableEntity * >( node->GetTransformable() ), renderer );

    delete node;
}

// **************************
//
void    ExperimentalFreePdrResources( RenderableEntity * renderable, Renderer * renderer )
{
    { renderable; }
    { renderer; }
    /*
    auto vao    = renderable->GetRenderableArrayData();
    
    auto effect = renderable->GetRenderableEffect();

    renderer->DeletePDR( vao );
    */
}

} // bv
