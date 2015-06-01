#include "ScenePrototype3.h"



namespace bv {

// **************************
//
ScenePrototype3::ScenePrototype3( Renderer * renderer )
    : ScenePrototype( renderer )
{
}

// **************************
//
void        ScenePrototype3::Update              ( TimeType t )
{
    { t; }
}

// **************************
//
void        ScenePrototype3::OnKey               ( unsigned char c )
{
    { c; }
}

// **************************
//
SceneNode * ScenePrototype3::BuildSceneImpl      ()
{
	SceneNode * root = MultipassNodeBuilder::CreateMultipassRectNodeTexture( 1.f, 1.f, -1.1f, "sand.jpg", 3 );
    SetLocalTransform( root, Transformations::Scale( glm::vec3( 2.f, 2.f, 1.0f ) ) );

    return root;
}

} //bv
