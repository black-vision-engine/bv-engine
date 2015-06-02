#include "ScenePrototype3.h"
#include "Mathematics\Defines.h"


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
	std::string cubeMap( "Penguins.jpg" );

	std::vector<std::string*> textures;
	textures.push_back( &sand );
	textures.push_back( &cubeMap );		// textures.size() is at the same time number of passes

	SceneNode * root = MultipassNodeBuilder::CreateMultipassRectNodeTexture( 1.f, 1.f, -1.1f, textures.data(), (unsigned short)textures.size() );
    //SetLocalTransform( root, Transformations::Scale( glm::vec3( 2.f, 2.f, 1.0f ) ) );

	float alfa = 45;

	for( unsigned int i = 0; i < 4; ++i )
	{
		SceneNode* newNode = MultipassNodeBuilder::CreateMultipassRectNodeTexture( 1.f, 1.f, -1.1f, textures.data(), (unsigned short)textures.size() );
		SetLocalTransform( newNode, Transformations::Translation( glm::vec3( 1.0*sin( alfa ), 1.0*cos( alfa ), 0.0f ) ) );
		SetWorldTransform( newNode, Transformations::Rotation( glm::vec3( 0.0, 0.0, 1.0f ), alfa ) );

		root->AddChildNode( newNode );

		alfa += 360.0f;
	}

    return root;
}

} //bv
