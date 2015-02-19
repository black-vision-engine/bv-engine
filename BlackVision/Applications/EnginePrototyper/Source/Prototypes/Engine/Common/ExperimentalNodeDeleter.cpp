#include "ExperimentalNodeDeleter.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv
{

// **************************
//
void    ExperimentalDeleteAndCleanup( SceneNode * node, Renderer * renderer )
{
    // TODO: implement along with all necessary logging and tracking
    { renderer; }

    printf( "Experimental node deleter called\n" );

    delete node;
}

} // bv
