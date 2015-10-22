#include "TestScene.h"

#include <cassert>

#include "TestSceneUtils.h"

namespace bv {

const std::string	TestScene::COL_NODE		= "col";
const std::string	TestScene::TEX_NODE		= "tex";
const std::string	TestScene::ANIM_NODE	= "anim";

// ****************************
//	
OrderTestCase::OrderTestCase	( const std::string & node, const std::string & test, const std::vector< std::string > & plugins )
	: node( node )
	, test( test )
	, plugins( plugins )
{
}

// ****************************
//
						TestScene::TestScene				( Renderer * renderer, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
		: m_renderer( renderer )
		, m_timelineManager( timelineManager )
		, m_timeEvaluator( timeEvaluator )
		, m_lastStep( -1 )
		, m_stepOffset( 0 )
{
	auto size = TestSceneUtils::IMG_SIZE;
	TestSceneUtils::GenerateCheckboardTex( TestSceneUtils::TEXTURE_PATH, size, size, glm::uvec3( 32 ) );
	TestSceneUtils::GenerateCheckboardAlphaMaskTex( TestSceneUtils::ALPHA_MASK_PATH, size, size );
	TestSceneUtils::GenerateCheckboardAnim( TestSceneUtils::ANIM_PATH, size, size, TestSceneUtils::ANIM_NUM );

	m_scene = ColoredRectanglesScene();

	InitTestModelNodeEditor();
}

// ****************************
//
						TestScene::~TestScene				()
{
}

// ****************************
//
void					TestScene::Restart					()
{
	m_scene = ColoredRectanglesScene();
	TestModelSceneEditor();
	m_stepOffset = m_lastStep;
}

// ****************************
//
BVScenePtr				TestScene::GetScene					()
{
	return m_scene;
}

// ****************************
//
void					TestScene::TestModelSceneEditor		()
{
	auto editor = m_scene->GetSceneEditor();

	auto root = editor->GetRootNode();
	assert( root->GetName() == "root" );
	assert( root == m_scene->GetModelSceneRoot() );
	auto rootChildrenNum = root->GetNumChildren();

	auto child = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "added0", 0.2f, 0.2f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
	auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
	model::SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )1.5f, -0.5f, 0.f ) );

	editor->AddChildNode( root, child );
	rootChildrenNum++;
	assert( root->GetNumChildren() == rootChildrenNum );

	auto child0 = root->GetChild( "child0" );
	assert( child0->GetName() == "child0" );
	auto child0ChildrenNum = child0->GetNumChildren();

	assert( child0->GetChild( "child00" ) );
	assert( editor->DetachChildNode( child0, "child00" ) );
	child0ChildrenNum--;
	assert( child0->GetNumChildren() == child0ChildrenNum );
	assert( !child0->GetChild( "child00" ) );
	
	assert( child0->GetChild( "child02" ) );
	assert( editor->DetachChildNode( child0, "child02" ) );
	child0ChildrenNum--;
	assert( child0->GetNumChildren() == child0ChildrenNum );
	assert( !child0->GetChild( "child02" ) );
	
	assert( child0->GetChild( "child01" ) );
	assert( editor->DetachChildNode( child0, "child01" ) );
	child0ChildrenNum--;
	assert( child0->GetNumChildren() == child0ChildrenNum );
	assert( !child0->GetChild( "child01" ) );
	assert( !editor->DetachChildNode( child0, "child01" ) );

	assert( editor->AttachChildNode( root ) );
	rootChildrenNum++;
	assert( root->GetNumChildren() == rootChildrenNum );
	assert( root->GetChild( "child01" ) );
	assert( !editor->AttachChildNode( root ) );

	assert( root->GetChild( "child0" ) );
	assert( editor->DeleteChildNode( root, "child0" ) );
	rootChildrenNum--;
	assert( root->GetNumChildren() == rootChildrenNum );
	assert( !root->GetChild( "child0" ) );
	assert( !editor->DetachChildNode( root, "child0" ) );
	assert( !editor->AttachChildNode( root ) );
	assert( !editor->DeleteChildNode( root, "child0" ) );

	assert( root->GetChild( "child1" ) );
	assert( editor->DetachChildNode( root, "child1" ) );
	rootChildrenNum--;
	assert( root->GetNumChildren() == rootChildrenNum );
	assert( !root->GetChild( "child1" ) );
	assert( !editor->DetachChildNode( root, "child1" ) );
	assert( !editor->DeleteChildNode( root, "child1" ) );

	editor->DeleteDetachedNodes();
	editor->DeleteDetachedNodes();
	assert( !editor->AttachChildNode( root ) );
	assert( root->GetNumChildren() == rootChildrenNum );
	assert( !root->GetChild( "child1" ) );
	assert( !editor->DetachChildNode( root, "child1" ) );
	assert( !editor->DeleteChildNode( root, "child1" ) );

	assert( editor->GetRootNode() );
	assert( editor->DetachRootNode() );
	assert( !editor->GetRootNode() );
	assert( !editor->DeleteRootNode() );
	assert( !editor->DetachRootNode() );

	editor->AttachRootNode();
	assert( m_scene->GetModelSceneRoot() );
	editor->AttachRootNode();
	root = editor->GetRootNode();
	assert( root == m_scene->GetModelSceneRoot() );
	assert( root->GetNumChildren() == rootChildrenNum );
	assert( root->GetChild( "child2" ) );
	assert( root->GetChild( "added0" ) );

	assert( editor->DetachRootNode() );
	editor->DeleteDetachedNodes();
	editor->AttachRootNode();
	assert( !editor->GetRootNode() );
	assert( !editor->DeleteRootNode() );
	assert( !editor->DetachRootNode() );

	auto newRoot = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "newRoot", 0.5f, 0.5f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
	auto rootTransform = newRoot->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    model::SetParameterTranslation( rootTransform, 0, 0.0f, glm::vec3( -1.f, 0.5f, -1.f ) );
	
	editor->SetRootNode( newRoot );
	assert( editor->GetRootNode() );
	root = editor->GetRootNode();
	assert( root == m_scene->GetModelSceneRoot() );
	rootChildrenNum = root->GetNumChildren();
	assert( root->GetNumChildren() == rootChildrenNum );

	assert( editor->DeleteRootNode() );
	assert( !editor->GetRootNode() );
	assert( !editor->DeleteRootNode() );
	assert( !editor->DetachRootNode() );

	editor->SetRootNode( newRoot );
	root = editor->GetRootNode();
	assert( root == m_scene->GetModelSceneRoot() );
	rootChildrenNum = root->GetNumChildren();
	
	assert( editor->GetRootNode() );
	assert( root->GetNumChildren() == rootChildrenNum );

	auto newChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "newChild", 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
	childTransform = newChild->GetPlugin( "transform" )->GetParameter( "simple_transform" );
	model::SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )0.5, -0.5f, 0.2f ) );

	editor->AddChildNode( root, newChild );
	rootChildrenNum++;
	assert( root->GetChild( "newChild" ) );
	assert( root->GetNumChildren() == rootChildrenNum );

	for( UInt32 i = 0; i < root->GetNumChildren(); ++i )
	{
		editor->DeleteChildNode( root, m_scene->GetModelSceneRoot()->GetChild( 0 )->GetName() );
	}
}

// ****************************
//
void					TestScene::InitTestModelNodeEditor	()
{
	//InitBasicColorPluginTest();
	//InitOrderColorPluginTest();

	InitBasicTexturePluginTest();
	InitOrderTexturePluginTest();

	InitBasicAnimationPluginTest();
	InitOrderAnimationPluginTest();
}

// ****************************
//
void					TestScene::InitBasicColorPluginTest	()
{
	m_testSteps.push_back( [&] 
	{ 
		// DETACH COLOR PLUGIN FROM ROOT NODE ( WHITE ROOT & GREEN CHILD )
		auto editor = m_scene->GetSceneEditor();
		auto root = m_scene->GetModelSceneRoot();
		assert( root->GetNumPlugins() == 3 );

		assert( editor->DetachPlugin( root, "solid color" ) );
		assert( !editor->DetachPlugin( root, "solid color" ) );
		assert( root->GetNumPlugins() == 2 );
		assert( editor->GetDetachedPlugin( root ) );
		assert( editor->GetDetachedPlugin( root )->GetName() == "solid color" );
	});

	m_testSteps.push_back( [&] 
	{ 
		// DETACH COLOR PLUGIN FROM CHILD NODE ( WHITE ROOT & WHITE CHILD )
		auto editor = m_scene->GetSceneEditor();
		auto child = m_scene->GetModelSceneRoot()->GetChild( 0 );

		assert( editor->DetachPlugin( child, "solid color" ) );
		assert( !editor->DetachPlugin( child, "solid color" ) );
		assert( child->GetNumPlugins() == 2 );
		assert( editor->GetDetachedPlugin( child ) );
		assert( editor->GetDetachedPlugin( child )->GetName() == "solid color" );
	});

	m_testSteps.push_back( [&] 
	{ 
		// ATTACH COLOR PLUGIN TO ROOT NODE ( GREEN ROOT & WHITE CHILD )
		auto editor = m_scene->GetSceneEditor();
		auto root = m_scene->GetModelSceneRoot();

		assert( editor->AttachPlugin( root, 2 ) );
		assert( !editor->AttachPlugin( root, 2 ) );
		assert( root->GetNumPlugins() == 3 );
		assert( !editor->GetDetachedPlugin( root ) );
	});
	
	m_testSteps.push_back( [&] 
	{ 
		// ATTACH COLOR PLUGIN FROM ROOT NODE TO CHILD NODE ( WHITE ROOT & GREEN CHILD )
		auto editor = m_scene->GetSceneEditor();
		auto root = m_scene->GetModelSceneRoot();
		auto child = root->GetChild( 0 );

		assert( editor->DetachPlugin( root, "solid color" ) );
		assert( editor->GetDetachedPlugin( root ) );
		assert( root->GetNumPlugins() == 2 );
		assert( editor->AttachPlugin( root, child, 2 ) );
		assert( !editor->GetDetachedPlugin( root ) );
		assert( editor->GetDetachedPlugin( child ) );
		assert( child->GetNumPlugins() == 3 );
	});
	
	m_testSteps.push_back( [&] 
	{ 
		// DELETE DETACHED COLOR PLUGIN FROM CHILD NODE ( WHITE ROOT & GREEN CHILD )
		auto editor = m_scene->GetSceneEditor();
		auto child = m_scene->GetModelSceneRoot()->GetChild( 0 );

		editor->ResetDetachedPlugin( child );
		assert( !editor->GetDetachedPlugin( child ) );
		assert( !editor->AttachPlugin( child, 2 ) );
	});

	m_testSteps.push_back( [&] 
	{ 
		// ADD RED COLOR PLUGIN TO ROOT NODE
		auto editor = m_scene->GetSceneEditor();
		auto newChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "newChild", 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
		model::SetParameterTranslation( newChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.5f, -0.5f, 0.0f ) );

		auto root = m_scene->GetModelSceneRoot();

		assert( editor->AddPlugin( root, newChild->GetPlugin( "solid color" ), 2 ) );
		assert( root->GetNumPlugins() == 3 );

		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "lChild", 0.2f, 0.2f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
		model::SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.5f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager, m_timeEvaluator, "rChild", 0.2f, 0.2f, TestSceneUtils::TEXTURE_PATH );
		model::SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.5f, 0.0f ) );

		editor->AddChildNode( root->GetChild( 0 ), lChild );
		editor->AddChildNode( root->GetChild( 0 ), rChild );
	});
}

// ****************************
//
void					TestScene::InitOrderColorPluginTest	()
{
	std::string test0[] = { "transform", "rectangle", "solid color", "alpha_mask" };
	std::string test1[] = { "rectangle", "transform", "solid color" };
	std::string test2[] = { "rectangle", "solid color", "transform", "alpha_mask" };
	std::string test3[] = { "transform" , "solid color", "rectangle", "alpha_mask" };
	std::string test4[] = { "solid color", "transform" , "rectangle" };
	std::string test5[] = { "solid color", "rectangle" , "transform" };
	std::string test6[] = { "alpha_mask", "solid color" };

	std::vector < OrderTestCase > tests;
	tests.push_back( OrderTestCase( COL_NODE, "TRCAm", std::vector< std::string >( test0, test0 + 4 ) ) );
	tests.push_back( OrderTestCase( COL_NODE, "RTC", std::vector< std::string >( test1, test1 + 3 ) ) );
	tests.push_back( OrderTestCase( COL_NODE, "RCTAm", std::vector< std::string >( test2, test2 + 4 ) ) );
	tests.push_back( OrderTestCase( COL_NODE, "TCRAm", std::vector< std::string >( test3, test3 + 4 ) ) );
	tests.push_back( OrderTestCase( COL_NODE, "CTR", std::vector< std::string >( test4, test4 + 3 ) ) );
	tests.push_back( OrderTestCase( COL_NODE, "CRT", std::vector< std::string >( test5, test5 + 3 ) ) );
	tests.push_back( OrderTestCase( COL_NODE, "AmC", std::vector< std::string >( test6, test6 + 2 ) ) );

	auto recoverScene = [&] 
	{ 
		auto editor = m_scene->GetSceneEditor();
		auto newChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "newChild", 0.3f, 0.3f, glm::vec4( 0.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		auto childTransform = newChild->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		model::SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )0.5, -0.5f, 0.f ) );

		auto root = m_scene->GetModelSceneRoot();
		auto child = root->GetChild( 0 );

		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );

		assert( editor->DeleteChildNode( root, child->GetName() ) );
		editor->AddChildNode( root, newChild );
		assert( root->GetNumChildren() == 1 );
		editor->AddChildNode( newChild, lChild );
		editor->AddChildNode( newChild, rChild );
		assert( newChild->GetNumChildren() == 2 );
		child = root->GetChild( 0 );
		//assert( child->GetNumPlugins() == 4 );
	};

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
		InitOrderTest( test );
	}
}

// ****************************
//
void					TestScene::InitBasicTexturePluginTest	()
{
	auto add = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		// ADD TEXTURED ROOT NODE
		auto tex = TestSceneUtils::TexturedRectangle( m_timelineManager, m_timeEvaluator, TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		model::SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		auto root = m_scene->GetModelSceneRoot();
		editor->AddChildNode( root, tex );
		
		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
		model::SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager, m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		model::SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( tex, lChild );
		editor->AddChildNode( tex, rChild );

		assert( tex->GetNumChildren() == 2 );
	};

	m_testSteps.push_back( add );
	m_testSteps.push_back( [&]{ SwapLastPlugin( TEX_NODE ); } );
	m_testSteps.push_back( [&]{ SwapLastPlugin( TEX_NODE ); } );
}

// ****************************
//
void					TestScene::InitOrderTexturePluginTest	()
{
	std::string test0[] = { "texture", "rectangle" , "transform", "alpha_mask" };
	std::string test1[] = { "transform", "texture" , "rectangle", "alpha_mask" };
	std::string test2[] = { "rectangle", "texture", "transform", "alpha_mask" };
	std::string test3[] = { "rectangle", "transform" , "texture", "alpha_mask" };
	std::string test4[] = { "alpha_mask", "texture" };
	std::string test5[] = { "texture", "alpha_mask" };

	std::vector < OrderTestCase > tests;
	tests.push_back( OrderTestCase( TEX_NODE, "TxRTAm", std::vector< std::string >( test0, test0 + 4 ) ) );
	tests.push_back( OrderTestCase( TEX_NODE, "TTxRAm", std::vector< std::string >( test1, test1 + 4 ) ) );
	tests.push_back( OrderTestCase( TEX_NODE, "TRTxAm", std::vector< std::string >( test2, test2 + 4 ) ) );
	tests.push_back( OrderTestCase( TEX_NODE, "RTTx", std::vector< std::string >( test3, test3 + 4 ) ) );
	tests.push_back( OrderTestCase( TEX_NODE, "AmTx", std::vector< std::string >( test4, test4 + 2 ) ) );
	tests.push_back( OrderTestCase( TEX_NODE, "TxAm", std::vector< std::string >( test5, test5 + 2 ) ) );

	auto recoverScene = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto tex = TestSceneUtils::TexturedRectangle( m_timelineManager, m_timeEvaluator, TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		model::SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		auto root = m_scene->GetModelSceneRoot();
		auto child = root->GetChild( TEX_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		assert( editor->DeleteChildNode( root, child->GetName() ) );
		editor->AddChildNode( root, tex );
		editor->AddChildNode( tex, lChild );
		editor->AddChildNode( tex, rChild );
		assert( tex->GetNumChildren() == 2 );
	};

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
		InitOrderTest( test );
	}
	m_testSteps.push_back( recoverScene );
}

// ****************************
//
void					TestScene::InitBasicAnimationPluginTest	()
{
	auto add = [&] 
	{
		auto editor = m_scene->GetSceneEditor();

		auto anim = TestSceneUtils::AnimatedRectangle( m_timelineManager, m_timeEvaluator, ANIM_NODE, 0.3f, 0.3f, TestSceneUtils::ANIM_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		model::SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
		
		auto root = m_scene->GetModelSceneRoot();
		editor->AddChildNode( root, anim );
		
		auto lChild = TestSceneUtils::TexturedRectangle( m_timelineManager, m_timeEvaluator, "lChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		model::SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "rChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		model::SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( anim, lChild );
		editor->AddChildNode( anim, rChild );

		assert( anim->GetNumChildren() == 2 );
	};

	m_testSteps.push_back( add );
	m_testSteps.push_back( [&]{ SwapLastPlugin( ANIM_NODE ); } );
	m_testSteps.push_back( [&]{ SwapLastPlugin( ANIM_NODE ); } );
}

// ****************************
//
void					TestScene::InitOrderAnimationPluginTest	()
{
	std::string test0[] = { "animation", "rectangle" , "transform", "alpha_mask" };
	std::string test1[] = { "transform", "animation" , "rectangle", "alpha_mask" };
	std::string test2[] = { "rectangle", "animation", "transform", "alpha_mask" };
	std::string test3[] = { "rectangle", "transform" , "animation", "alpha_mask" };
	std::string test4[] = { "alpha_mask", "animation" };
	std::string test5[] = { "animation", "alpha_mask" };

	std::vector < OrderTestCase > tests;
	tests.push_back( OrderTestCase( ANIM_NODE, "ARTAm", std::vector< std::string >( test0, test0 + 4 ) ) );
	tests.push_back( OrderTestCase( ANIM_NODE, "TARAm", std::vector< std::string >( test1, test1 + 4 ) ) );
	tests.push_back( OrderTestCase( ANIM_NODE, "TRAAm", std::vector< std::string >( test2, test2 + 4 ) ) );
	tests.push_back( OrderTestCase( ANIM_NODE, "RTA", std::vector< std::string >( test3, test3 + 4 ) ) );
	tests.push_back( OrderTestCase( ANIM_NODE, "AAm", std::vector< std::string >( test4, test4 + 2 ) ) );
	tests.push_back( OrderTestCase( ANIM_NODE, "AAm", std::vector< std::string >( test5, test5 + 2 ) ) );

	auto recoverScene = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto anim = TestSceneUtils::AnimatedRectangle( m_timelineManager, m_timeEvaluator, ANIM_NODE, 0.3f, 0.3f, TestSceneUtils::ANIM_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		model::SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );

		auto root = m_scene->GetModelSceneRoot();
		auto child = root->GetChild( ANIM_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		assert( editor->DeleteChildNode( root, child->GetName() ) );
		editor->AddChildNode( root, anim );
		editor->AddChildNode( anim, lChild );
		editor->AddChildNode( anim, rChild );
		assert( anim->GetNumChildren() == 2 );
	};

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
		InitOrderTest( test );
	}
	m_testSteps.push_back( recoverScene );
}

// ****************************
//
void					TestScene::InitOrderTest			( const OrderTestCase & testCase )
{
	auto name = testCase.test;
	auto node = testCase.node;
	for( auto plugin : testCase.plugins )
	{
		m_testSteps.push_back( [ plugin, node, name, this ] 
		{ 
			printf( "\n TEST: %s %s \n removing %s plugin \n\n", node.c_str(), name.c_str(), plugin.c_str() );

			auto editor = m_scene->GetSceneEditor();
			auto child = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( node ) );
			assert( editor->DeletePlugin( child, plugin ) );
			assert( !editor->DeletePlugin( child, plugin ) );
			assert( !child->GetPlugin( plugin ) );

		});
	}
}

// ****************************
//
void					TestScene::SwapLastPlugin		( const std::string & childName )
{
	auto editor = m_scene->GetSceneEditor();

	auto root = m_scene->GetModelSceneRoot();
	auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( childName ) );

	std::string rootLastPlugin, childLastPlugin;
	UInt32 rootLastIdx, childLastIdx;
	if( root->GetFinalizePlugin()->GetPrevPlugin()->GetName() == "alpha_mask" )
	{
		rootLastPlugin = root->GetFinalizePlugin()->GetPrevPlugin()->GetPrevPlugin()->GetName();
		rootLastIdx = root->GetNumPlugins() - 2;
	}
	else
	{
		rootLastPlugin = root->GetFinalizePlugin()->GetPrevPlugin()->GetName();
		rootLastIdx = root->GetNumPlugins() - 1;
	}

	if( child->GetFinalizePlugin()->GetPrevPlugin()->GetName() == "alpha_mask" )
	{
		childLastPlugin = child->GetFinalizePlugin()->GetPrevPlugin()->GetPrevPlugin()->GetName();
		childLastIdx = child->GetNumPlugins() - 2;
	}
	else
	{
		childLastPlugin = child->GetFinalizePlugin()->GetPrevPlugin()->GetName();
		childLastIdx = child->GetNumPlugins() - 1;
	}

	assert( editor->DetachPlugin( root, rootLastPlugin ) );
	assert( editor->DetachPlugin( child, childLastPlugin ) );

	assert( editor->GetDetachedPlugin( child ) );
	assert( editor->GetDetachedPlugin( child )->GetName() == childLastPlugin );

	assert( editor->AddPlugin( root, editor->GetDetachedPlugin( child ), rootLastIdx ) );

	assert( editor->GetDetachedPlugin( root ) );
	assert( editor->GetDetachedPlugin( root )->GetName() == rootLastPlugin );

	assert( editor->AddPlugin( child, editor->GetDetachedPlugin( root ), childLastIdx ) );
}

// ****************************
//
void					TestScene::TestModelNodeEditor	( TimeType time )
{
	auto step = ( Int32 )std::floor( TestSceneUtils::SPEED * time );
	
	if( m_lastStep != step && step - m_stepOffset < m_testSteps.size() )
	{
		m_testSteps[ step - m_stepOffset ]();
	}
	
	m_lastStep = step;
}

// ****************************
//
BVScenePtr				TestScene::ColoredRectanglesScene	()
{
	auto root = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "root", 0.5f, 0.5f, glm::vec4( 0.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
    auto rootTransform  = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    model::SetParameterTranslation( rootTransform, 0, 0.0f, glm::vec3( -1.f, 0.5f, -1.f ) );
	
	for( unsigned int i = 0; i < 3; ++i )
	{
		auto child = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "child" + toString( i ), 0.3f, 0.3f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		model::SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )0.5*i, -0.5f, 0.f ) );
		root->AddChildToModelOnly( child );
	}

	for( unsigned int i = 0; i < 3; ++i )
	{
		auto child = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "child0" + toString( i ), 0.2f, 0.2f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		model::SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )i, -0.5f, 0.f ) );
		root->GetChild( 0 )->AddChildToModelOnly( child );
	}

    return BVScene::Create( root, new Camera(), "BasicScene", m_timeEvaluator, m_renderer );
}

} // bv