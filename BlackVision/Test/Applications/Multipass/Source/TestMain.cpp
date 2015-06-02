#include "Assets/Assets.h"
#include "Engine/Types/EnumsUtils.h"
#include "DataTypes/Hash.h"

#include "../..\Applications/BlackVision/Source/Rendering/RenderLogic.h"
//#include "../../Applications/EnginePrototyper/Source/Prototypes/Engine/Common/MultipassNodeBuilder.h"
//#include "../../Applications/EnginePrototyper/Source/Prototypes/Engine/Common/Scenes/ScenePrototype3.h"
//#include "../../Applications/EnginePrototyper/Source/Prototypes/Engine/Common/ScenePrototype.h"
//#include "../../Applications/EnginePrototyper/Source/Prototypes/Engine/Multipass/MultipassTest.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine\Graphics\SceneGraph\RenderableEntity.h"

#include "gtest/gtest.h"
#include "VisualTester\WinApplicationTester.h"


namespace bv {




TEST(MultipassAlphaBlending, Multipass)
{
	//RenderLogic renderLogic;

	//RendererInput _rendererInput;
	//Renderer renderer( _rendererInput, 0, 0 );		// Fake renderer
	//
	//ScenePrototype* scene = ScenePrototype::CreateNewPrototype( 3, &renderer );
	//ScenePrototype3* scene3 = dynamic_cast<ScenePrototype3*>( scene );

	//SceneNode* root = scene->GetRootNode();

	//RenderableEntity* ent = dynamic_cast<RenderableEntity*>( root->GetTransformable() );

	//ASSERT_FALSE( ent );		// Dynamic_cast failed

	//auto effect = ent->GetRenderableEffect();

	//ASSERT_TRUE( effect == nullptr );
	//ASSERT_FALSE( effect->NumPasses() != (unsigned int)scene3->getNumPasses() );
}



}//bv

//int main( int argc, char **argv )
//{
//	::testing::InitGoogleTest( &argc, argv );
//
//	return RUN_ALL_TESTS();
//}

// enable intrinsic functions - w konfiguracji Common (dla Release na true)
