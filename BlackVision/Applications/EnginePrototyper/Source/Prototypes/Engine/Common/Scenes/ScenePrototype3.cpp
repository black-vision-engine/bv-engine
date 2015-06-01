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
	std::string sand( "sand.jpg" );
	std::string cubeMap( "cube_map.jpg" );

	std::vector<std::string*> textures;
	textures.push_back( &sand );
	textures.push_back( &cubeMap );		// textures.size() is at the same time number of passes

	SceneNode * root = MultipassNodeBuilder::CreateMultipassRectNodeTexture( 1.f, 1.f, -1.1f, textures.data(), (unsigned short)textures.size() );
    SetLocalTransform( root, Transformations::Scale( glm::vec3( 2.f, 2.f, 1.0f ) ) );

    return root;
}

} //bv
