#include "TestScene.h"

#include <cassert>

#include "TestSceneUtils.h"

namespace bv {

const std::string	TestScene::COL_NODE		= "col";
const std::string	TestScene::TEX_NODE		= "tex";
const std::string	TestScene::ANIM_NODE	= "anim";
const std::string	TestScene::GRAD_NODE	= "grad";
const std::string	TestScene::TXT_NODE		= "txt";
const std::string	TestScene::TMR_NODE		= "tmr";
const std::string	TestScene::GEOM_NODE	= "geom";

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
	TestSceneUtils::GenerateCheckboardAlphaMaskTex( TestSceneUtils::ALPHA_MASK_PATH, TestSceneUtils::AM_SIZE, TestSceneUtils::AM_SIZE );
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
	SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )1.5f, -0.5f, 0.f ) );

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
    SetParameterTranslation( rootTransform, 0, 0.0f, glm::vec3( -1.f, 0.5f, -1.f ) );
	
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
	SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )0.5, -0.5f, 0.2f ) );

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

	//InitBasicTexturePluginTest();
	//InitOrderTexturePluginTest();

	//InitBasicAnimationPluginTest();
	//InitOrderAnimationPluginTest();

	//InitBasicGradientPluginTest();
	//InitOrderGradientPluginTest();

	//InitColoredTextTest();
	//InitColoredTimerTest();

	InitColoredGeometryTest();
}

// ****************************
//
void					TestScene::InitBasicColorPluginTest	()
{
	auto add = [&] 
	{ 
		auto editor = m_scene->GetSceneEditor();
		auto col = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, COL_NODE, 0.3f, 0.3f, glm::vec4( 0.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( col->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.5f, 0.f ) );

		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		editor->AddChildNode( root, col );

		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "rChild", 0.1f, 0.1f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( col, lChild );
		editor->AddChildNode( col, rChild );
		success &= ( col->GetNumChildren() == 2 );

		assert( success ); { success; }
	};

	m_testSteps.push_back( add );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, COL_NODE, "solid color", 2 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, COL_NODE, "solid color", 2 ); } );
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
		auto col = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, COL_NODE, 0.3f, 0.3f, glm::vec4( 0.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( col->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.5f, 0.f ) );

		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		auto child = root->GetChild( COL_NODE );

		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );

		success &= editor->DeleteChildNode( root, child->GetName() );
		editor->AddChildNode( root, col );
		editor->AddChildNode( col, lChild );
		editor->AddChildNode( col, rChild );
		success &= ( col->GetNumChildren() == 2 );
		
		assert( success );
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
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		editor->AddChildNode( root, tex );
		
		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager, m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( tex, lChild );
		editor->AddChildNode( tex, rChild );

		success &= ( tex->GetNumChildren() == 2 );
		
		assert( success );
	};

	m_testSteps.push_back( add );
	
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TEX_NODE, "texture", 2 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "texture", 2, TEX_NODE, "solid color", 2 ); } );
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
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		auto child = root->GetChild( TEX_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( root, child->GetName() );
		editor->AddChildNode( root, tex );
		editor->AddChildNode( tex, lChild );
		editor->AddChildNode( tex, rChild );
		success &= ( tex->GetNumChildren() == 2 );

		assert( success );
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
		SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
		
		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		editor->AddChildNode( root, anim );
		
		auto lChild = TestSceneUtils::TexturedRectangle( m_timelineManager, m_timeEvaluator, "lChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "rChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( anim, lChild );
		editor->AddChildNode( anim, rChild );

		success &= ( anim->GetNumChildren() == 2 );
		
		assert( success );
	};

	m_testSteps.push_back( add );

	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, ANIM_NODE, "animation", 2 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "animation", 2, ANIM_NODE, "solid color", 2 ); } );
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
	tests.push_back( OrderTestCase( ANIM_NODE, "RATAm", std::vector< std::string >( test2, test2 + 4 ) ) );
	tests.push_back( OrderTestCase( ANIM_NODE, "RTAAm", std::vector< std::string >( test3, test3 + 4 ) ) );
	tests.push_back( OrderTestCase( ANIM_NODE, "AAm", std::vector< std::string >( test4, test4 + 2 ) ) );
	tests.push_back( OrderTestCase( ANIM_NODE, "AAm", std::vector< std::string >( test5, test5 + 2 ) ) );

	auto recoverScene = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto anim = TestSceneUtils::AnimatedRectangle( m_timelineManager, m_timeEvaluator, ANIM_NODE, 0.3f, 0.3f, TestSceneUtils::ANIM_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
		
		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		auto child = root->GetChild( ANIM_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( root, child->GetName() );
		editor->AddChildNode( root, anim );
		editor->AddChildNode( anim, lChild );
		editor->AddChildNode( anim, rChild );
		success &= ( anim->GetNumChildren() == 2 );

		assert( success );
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
void					TestScene::InitBasicGradientPluginTest	()
{
	auto add = [&] 
	{
		auto editor = m_scene->GetSceneEditor();

		auto grad = TestSceneUtils::GradientRectangle( m_timelineManager, m_timeEvaluator, GRAD_NODE, 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( grad->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		editor->AddChildNode( root, grad );
		
		auto lChild = TestSceneUtils::AnimatedRectangle( m_timelineManager, m_timeEvaluator, "lChild", 0.1f, 0.1f, TestSceneUtils::ANIM_PATH );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager, m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( grad, lChild );
		editor->AddChildNode( grad, rChild );

		success &= ( grad->GetNumChildren() == 2 );

		assert( success );
	};

	m_testSteps.push_back( add );
	m_testSteps.push_back( [&]
	{
		auto child = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( GRAD_NODE ) );
		SetParameter( child->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), TimeType( 0.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
	});

	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GRAD_NODE, "linear_gradient", 2 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "linear_gradient", 2, GRAD_NODE, "solid color", 2 ); } );
}

// ****************************
//
void					TestScene::InitOrderGradientPluginTest	()
{
	std::string test0[] = { "linear_gradient", "rectangle" , "transform" };
	std::string test1[] = { "transform", "linear_gradient" , "rectangle" };
	std::string test2[] = { "rectangle", "linear_gradient", "transform" };
	std::string test3[] = { "rectangle", "transform" , "linear_gradient" };
	std::string test4[] = { "alpha_mask", "linear_gradient" };
	std::string test5[] = { "linear_gradient", "alpha_mask" };

	std::vector < OrderTestCase > tests;
	tests.push_back( OrderTestCase( GRAD_NODE, "LgRT", std::vector< std::string >( test0, test0 + 3 ) ) );
	tests.push_back( OrderTestCase( GRAD_NODE, "TLgR", std::vector< std::string >( test1, test1 + 3 ) ) );
	tests.push_back( OrderTestCase( GRAD_NODE, "TLgT", std::vector< std::string >( test2, test2 + 3 ) ) );
	tests.push_back( OrderTestCase( GRAD_NODE, "RTLg", std::vector< std::string >( test3, test3 + 3 ) ) );
	tests.push_back( OrderTestCase( GRAD_NODE, "AmLg", std::vector< std::string >( test4, test4 + 2 ) ) );
	tests.push_back( OrderTestCase( GRAD_NODE, "LgAm", std::vector< std::string >( test5, test5 + 2 ) ) );

	auto recoverScene = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto grad = TestSceneUtils::GradientRectangle( m_timelineManager, m_timeEvaluator, GRAD_NODE, 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( grad->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		auto child = root->GetChild( GRAD_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( root, child->GetName() );
		editor->AddChildNode( root, grad );
		editor->AddChildNode( grad, lChild );
		editor->AddChildNode( grad, rChild );
		success &= ( grad->GetNumChildren() == 2 );

		assert( success );
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
void					TestScene::InitColoredTextTest			()
{
	auto add = [&] 
	{
		auto editor = m_scene->GetSceneEditor();

		auto txt = TestSceneUtils::ColoredText( m_timelineManager, m_timeEvaluator, TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 60, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		editor->AddChildNode( root, txt );
		
		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager, m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( txt, lChild );
		editor->AddChildNode( txt, rChild );

		success &= ( txt->GetNumChildren() == 2 );

		assert( success );
	};

	std::string test0[] = { "text", "alpha_mask" };
	std::string test1[] = { "alpha_mask", "text" };
	std::string test2[] = { "alpha_mask", "transform", "solid color", "text" };
	std::string test3[] = { "transform", "alpha_mask", "solid color", "text" };
	std::string test4[] = { "transform", "solid color", "alpha_mask", "text" };
	std::string test5[] = { "alpha_mask", "solid color", "transform", "text" };
	std::string test6[] = { "solid color", "alpha_mask", "transform", "text" };
	std::string test7[] = { "solid color", "transform", "alpha_mask", "text" };
	std::string test8[] = { "transform", "text", "alpha_mask", "solid color" };
	std::string test9[] = { "transform", "alpha_mask", "text", "solid color" };
	std::string test10[] = { "solid color", "text", "alpha_mask", "transform" };
	std::string test11[] = { "solid color", "alpha_mask", "text", "transform" };

	std::vector < OrderTestCase > tests;
	tests.push_back( OrderTestCase( TXT_NODE, "TxtAm", std::vector< std::string >( test0, test0 + 2 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "AmTxt", std::vector< std::string >( test1, test1 + 2 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "AmTCTxt", std::vector< std::string >( test2, test2 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "TAmCTxt", std::vector< std::string >( test3, test3 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "TCAmTxt", std::vector< std::string >( test4, test4 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "AmCTTxt", std::vector< std::string >( test5, test5 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "CAmTTxt", std::vector< std::string >( test6, test6 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "CTAmTxt", std::vector< std::string >( test7, test7 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "TTxtAmC", std::vector< std::string >( test8, test8 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "TAmTxtC", std::vector< std::string >( test9, test9 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "CTxtAmT", std::vector< std::string >( test10, test10 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "CAmTxtT", std::vector< std::string >( test11, test11 + 4 ) ) );

	auto recoverScene = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto txt = TestSceneUtils::ColoredText( m_timelineManager, m_timeEvaluator, TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 60, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		auto child = root->GetChild( TXT_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( root, child->GetName() );
		editor->AddChildNode( root, txt );
		editor->AddChildNode( txt, lChild );
		editor->AddChildNode( txt, rChild );
		success &= ( txt->GetNumChildren() == 2 );

		assert( success );
	};


//---------------


	m_testSteps.push_back( add );
	m_testSteps.push_back( []{} ); //empty step because creating new text plugin takes so long..
	
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TXT_NODE, "solid color", 1 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TXT_NODE, "solid color", 1 ); } );

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
		m_testSteps.push_back( []{} ); //empty step because creating new text plugin takes so long..
		InitOrderTest( test );
	}
	m_testSteps.push_back( recoverScene );
}


// ****************************
//
void					TestScene::InitColoredTimerTest			()
{
	auto add = [&] 
	{
		auto editor = m_scene->GetSceneEditor();

		auto tmr = TestSceneUtils::ColoredTimer( m_timelineManager, m_timeEvaluator, TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 60 );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		editor->AddChildNode( root, tmr );
		
		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager, m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( tmr, lChild );
		editor->AddChildNode( tmr, rChild );

		success &= ( tmr->GetNumChildren() == 2 );

		assert( success );
	};

	std::string test0[] = { "timer" };
	std::string test1[] = { "solid color", "transform", "timer" };
	std::string test2[] = { "transform", "solid color", "timer" };
	std::string test3[] = { "transform", "timer" };
	std::string test4[] = { "solid color", "timer" };

	std::vector < OrderTestCase > tests;
	tests.push_back( OrderTestCase( TMR_NODE, "Tm", std::vector< std::string >( test0, test0 + 1 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "CTTm", std::vector< std::string >( test1, test1 + 3 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "TCTm", std::vector< std::string >( test2, test2 + 3 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "TTm", std::vector< std::string >( test3, test3 + 2 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "CTm", std::vector< std::string >( test4, test4 + 2 ) ) );

	auto recoverScene = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto tmr = TestSceneUtils::ColoredTimer( m_timelineManager, m_timeEvaluator, TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 60 );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		bool success = true;

		auto root = m_scene->GetModelSceneRoot();
		auto child = root->GetChild( TMR_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( root, child->GetName() );
		editor->AddChildNode( root, tmr );
		editor->AddChildNode( tmr, lChild );
		editor->AddChildNode( tmr, rChild );
		success &= ( tmr->GetNumChildren() == 2 );

		assert( success );
	};


//---------------


	m_testSteps.push_back( add );
	m_testSteps.push_back( []{} ); //empty step because creating new text plugin takes so long..
	
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TMR_NODE, "solid color", 1 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TMR_NODE, "solid color", 1 ); } );

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
		m_testSteps.push_back( []{} ); //empty step because creating new text plugin takes so long..
		InitOrderTest( test );
	}
	m_testSteps.push_back( recoverScene );
}

// ****************************
//
void					TestScene::InitColoredGeometryTest		()
{
	unsigned int pluginsNum = 1;
	std::string pluginsArr[] = { /*"DEFAULT_TRIANGLE", "DEFAULT_CIRCLE", "DEFAULT_ELLIPSE", "DEFAULT_ROUNDEDRECT",*/ "DEFAULT_CUBE" };
	std::string pluginsNameArr[] = {/* "triangle", "circle", "ellipse", "rounded rect",*/ "cube" };
	
	for( unsigned int i = 0; i < pluginsNum; ++i )
	{
		auto plugin = pluginsArr[ i ];
		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_scene->GetSceneEditor();

			auto geom = TestSceneUtils::ColoredGeometry( m_timelineManager, m_timeEvaluator, GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.0f, -0.5f, -2.f ) );

			auto root = m_scene->GetModelSceneRoot();
			editor->DeleteChildNode( root, GEOM_NODE );
			editor->AddChildNode( root, geom );
		});
		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GEOM_NODE, "solid color", 2 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GEOM_NODE, "solid color", 2 ); } );

		auto pluginName = pluginsNameArr[ i ];
		std::string test0[] = { "alpha_mask", pluginName };
		std::string test1[] = { "transform", "solid color", pluginName };
		std::string test2[] = { "solid color", "transform", pluginName };

		std::vector < OrderTestCase > tests;
		tests.push_back( OrderTestCase( GEOM_NODE, "Geom", std::vector< std::string >( test0, test0 + 2 ) ) );
		tests.push_back( OrderTestCase( GEOM_NODE, "TCGeom", std::vector< std::string >( test1, test1 + 3 ) ) );
		tests.push_back( OrderTestCase( GEOM_NODE, "CTGeom", std::vector< std::string >( test2, test2 + 3 ) ) );

		for( auto & test : tests )
		{
			m_testSteps.push_back( [ plugin, this ]
			{
				auto editor = m_scene->GetSceneEditor();

				auto geom = TestSceneUtils::ColoredGeometry( m_timelineManager, m_timeEvaluator, GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.0f, -0.5f, -2.f ) );

				auto root = m_scene->GetModelSceneRoot();
				editor->DeleteChildNode( root, GEOM_NODE );
				editor->AddChildNode( root, geom );
			});
			InitOrderTest( test );
		}
	}
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
			
			bool success = true;
			
			success &= editor->DeletePlugin( child, plugin );
			success &= !editor->DeletePlugin( child, plugin );
			success &= ( child->GetPlugin( plugin ) == nullptr );

			assert( success );
		});
	}
}

// ****************************
//
void					TestScene::SwapPlugins			( const std::string & rootPlugin, UInt32 rootIdx, const std::string & childName, const std::string & childPlugin,  UInt32 childIdx )
{
	auto editor = m_scene->GetSceneEditor();

	auto root = m_scene->GetModelSceneRoot();
	auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( childName ) );

	bool success = true;

	success &= editor->DetachPlugin( root, rootPlugin );
	success &= editor->DetachPlugin( child, childPlugin );

	success &= ( editor->GetDetachedPlugin( root ) != nullptr );
	success &= ( editor->GetDetachedPlugin( root )->GetName() == rootPlugin );

	success &= editor->AddPlugin( child, editor->GetDetachedPlugin( root ), childIdx );

	success &= ( editor->GetDetachedPlugin( child ) != nullptr );
	success &= ( editor->GetDetachedPlugin( child )->GetName() == childPlugin );

	success &= editor->AddPlugin( root, editor->GetDetachedPlugin( child ), rootIdx );

	assert( success );
}

// ****************************
//
void					TestScene::TestModelNodeEditor	( TimeType time )
{
	auto step = ( Int32 )std::floor( TestSceneUtils::SPEED * time );
	
	if( m_lastStep != step && step - m_stepOffset < ( Int32 )m_testSteps.size() )
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
    SetParameterTranslation( rootTransform, 0, 0.0f, glm::vec3( -1.f, 0.5f, -1.f ) );
	
	for( unsigned int i = 0; i < 3; ++i )
	{
		auto child = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "child" + toString( i ), 0.3f, 0.3f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )0.5*i, -0.5f, 0.f ) );
		root->AddChildToModelOnly( child );
	}

	for( unsigned int i = 0; i < 3; ++i )
	{
		auto child = TestSceneUtils::ColoredRectangle( m_timelineManager, m_timeEvaluator, "child0" + toString( i ), 0.2f, 0.2f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )i, -0.5f, 0.f ) );
		root->GetChild( 0 )->AddChildToModelOnly( child );
	}

    return BVScene::Create( root, new Camera(), "BasicScene", m_timeEvaluator, m_renderer );
}

} // bv