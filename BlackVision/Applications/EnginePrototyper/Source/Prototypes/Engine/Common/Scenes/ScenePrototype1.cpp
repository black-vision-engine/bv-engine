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
void        ScenePrototype1::Update              ( TimeType t )
{
    { t; }
}

// **************************
//
void        ScenePrototype1::OnKey               ( unsigned char c )
{
    { c; }
}

// **************************
//
SceneNode * ScenePrototype1::BuildSceneImpl      ()
{
    SceneNode * root = SimpleNodeBuilder::CreateRectNodeTexture( 1.f, 1.f, -1.1f, "rsrcy/test_frame_1.jpg" );
    SetLocalTransform( root, Transformations::Scale( glm::vec3( 2.f, 2.f, 1.0f ) ) );

    return root;
}

} //bv
