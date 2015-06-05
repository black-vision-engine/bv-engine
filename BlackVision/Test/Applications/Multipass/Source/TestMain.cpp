#include "Assets/Assets.h"
#include "Engine/Types/EnumsUtils.h"
#include "DataTypes/Hash.h"

#include "MultipassTest.h"
#include "Common\Scenes\ScenePrototype3.h"
#include "Common\ScenePrototype.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine\Graphics\SceneGraph\RenderableEntity.h"

#include "gtest/gtest.h"
#include "VisualTester\WinApplicationTester.h"


const std::string AssetsPath = "../../../../Test/Assets/Multipass/";

namespace bv {




TEST(MultipassAlphaBlending, Multipass)
{
	Renderer* renderer = application->getRenderer();


	std::string sand( "sand.jpg" );
	std::string cubeMap( "Penguins.jpg" );

	sand = AssetsPath + sand;
	cubeMap = AssetsPath + cubeMap;

	ScenePrototype3 scene( renderer );
	SceneNode* root = scene.CreateScene( sand, cubeMap );

	ASSERT_TRUE( root != nullptr );

	RenderableEntity* ent = dynamic_cast<RenderableEntity*>( root->GetTransformable() );

	ASSERT_TRUE( ent != nullptr );		// Dynamic_cast failed

	auto effect = ent->GetRenderableEffect();

	ASSERT_TRUE( effect != nullptr );
	ASSERT_EQ( effect->NumPasses(), (unsigned int)scene.getNumPasses() );


	application->testRender( AssetsPath + "MultipassTestReferenceImage.bmp", root );
}



}//bv

//int main( int argc, char **argv )
//{
//	::testing::InitGoogleTest( &argc, argv );
//
//	return RUN_ALL_TESTS();
//}

// enable intrinsic functions - w konfiguracji Common (dla Release na true)
