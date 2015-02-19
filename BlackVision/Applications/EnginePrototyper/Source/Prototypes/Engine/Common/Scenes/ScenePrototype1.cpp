#include "ScenePrototype1.h"


namespace bv {

// **************************
//
ScenePrototype1::ScenePrototype1( Renderer * renderer )
    : ScenePrototype( renderer )
{
}

// **************************
//
SceneNode * ScenePrototype1::BuildSceneImpl      ()
{
    return nullptr;
}

} //bv
