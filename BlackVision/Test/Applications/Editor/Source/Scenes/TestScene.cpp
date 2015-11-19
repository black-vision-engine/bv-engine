#include "TestScene.h"

#include <cassert>

#include "TestSceneUtils.h"

#include "Engine\Models\Plugins\Parameters\ParametersFactory.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"

#include "System/Path.h"
#include "IO/FileIO.h"
#include "IO/DirIO.h"

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
						TestScene::TestScene				( Renderer * renderer, model::TimelineManagerPtr timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
		: m_renderer( renderer )
		, m_timelineManager( timelineManager )
		, m_timeEvaluator( timeEvaluator )
		, m_lastStep( -1 )
		, m_stepOffset( 0 )
{
	auto size = TestSceneUtils::IMG_SIZE;

	auto dir = TestSceneUtils::ANIM_PATH;
	if( ( !Path::Exists( dir ) ) && ( !dir.empty() ) )
            Dir::CreateDir( dir );

	TestSceneUtils::GenerateCheckboardTex( TestSceneUtils::TEXTURE_PATH, size, size, glm::uvec3( 32 ) );
	TestSceneUtils::GenerateCheckboardAlphaMaskTex( TestSceneUtils::ALPHA_MASK_PATH, TestSceneUtils::AM_SIZE, TestSceneUtils::AM_SIZE );
	TestSceneUtils::GenerateCheckboardAlphaMaskTex( TestSceneUtils::ALPHA_MASK0_PATH, TestSceneUtils::AM_SIZE, TestSceneUtils::AM_SIZE, 128 );
	TestSceneUtils::GenerateCheckboardAnim( TestSceneUtils::ANIM_PATH, size, size, TestSceneUtils::ANIM_NUM );

	m_scene = ColoredRectanglesScene();

	InitTestEditor();
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
	InitTestEditor();
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
void					TestScene::InitTestModelSceneEditor	()
{
	m_testSteps.push_back([&] 
	{ 
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );

		for( unsigned int i = 0; i < 3; ++i )
		{
			auto child = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "child" + toString( i ), 0.3f, 0.3f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
			auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
			SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )0.5*i, -0.5f, 0.f ) );
			editor->AddChildNode( root, child );
		}
		assert( root->GetNumChildren() == 3 );

		for( unsigned int i = 0; i < 3; ++i )
		{
			auto child = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "child0" + toString( i ), 0.2f, 0.2f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
			auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
			SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )i, -0.5f, 0.f ) );
			editor->AddChildNode( root->GetChild( "child0" ), child );
		}
		assert( root->GetChild( "child0" )->GetNumChildren() == 3 );
	});

	m_testSteps.push_back([&] 
	{ 
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		bool success = true;
		success &= ( root->GetName() == "root" );
		success &= ( root == std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) ) );
		
		assert( success );

		auto child = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "newChild", 0.2f, 0.2f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )1.5f, -0.5f, 0.f ) );

		editor->AddChildNode( root, child );
	});

	m_testSteps.push_back([&] 
	{ 
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;

		auto child0 = root->GetChild( "child0" );
		success &= ( child0->GetName() == "child0" );

		success &= ( child0->GetChild( "child00" ) != nullptr );
		success &= ( editor->DetachChildNode( child0, "child00" ) );
		success &= ( !child0->GetChild( "child00" ) );
		success &= ( !editor->DetachChildNode( child0, "child00" ) );

		assert( success );
	});
	
	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;
		auto child0 = root->GetChild( "child0" );
		success &= ( child0->GetName() == "child0" );

		success &= ( child0->GetChild( "child02" ) != nullptr  );
		success &= ( editor->DetachChildNode( child0, "child02" ) );
		success &= ( !child0->GetChild( "child02" ) );
		success &= ( !editor->DetachChildNode( child0, "child02" ) );
	
		assert( success );
	});
	
	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;
		auto child0 = root->GetChild( "child0" );
		success &= ( child0->GetName() == "child0" );

		success &= ( child0->GetChild( "child01" ) != nullptr  );
		success &= ( editor->DetachChildNode( child0, "child01" ) );
		success &= ( !child0->GetChild( "child01" ) );
		success &= ( !editor->DetachChildNode( child0, "child01" ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;

		success &= ( editor->DetachChildNode( root, "child0" ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;

		success &= ( editor->AttachChildNode( root ) );
		success &= ( root->GetChild( "child0" ) != nullptr );
		success &= ( !editor->AttachChildNode( root ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;

		success &= ( root->GetChild( "child0" ) != nullptr );
		success &= ( editor->DeleteChildNode( root, "child0" ) );
		success &= ( !root->GetChild( "child0" ) );
		success &= ( !editor->DetachChildNode( root, "child0" ) );
		success &= ( !editor->AttachChildNode( root ) );
		success &= ( !editor->DeleteChildNode( root, "child0" ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;

		success &= ( root->GetChild( "child1" ) != nullptr );
		success &= ( editor->DeleteChildNode( root, "child1" ) );
		success &= ( !root->GetChild( "child1" ) );
		success &= ( !editor->DetachChildNode( root, "child1" ) );
		success &= ( !editor->AttachChildNode( root ) );
		success &= ( !editor->DeleteChildNode( root, "child1" ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;

		editor->DeleteDetachedNodes();
		editor->DeleteDetachedNodes();

		success &= ( !editor->AttachChildNode( root ) );
		success &= ( !root->GetChild( "child1" ) );
		success &= ( !editor->DetachChildNode( root, "child1" ) );
		success &= ( !editor->DeleteChildNode( root, "child1" ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;

		success &= ( editor->GetRootNode()->GetChild( "root" ) != nullptr );
		success &= ( editor->DetachRootNode() );
		success &= ( !editor->GetRootNode()->GetChild( "root" ) );
		success &= ( !editor->DeleteRootNode() );
		success &= ( !editor->DetachRootNode() );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		bool success = true;
		editor->AttachRootNode();
		assert( std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) ) );
		editor->AttachRootNode();
		auto root = editor->GetRootNode()->GetChild( "root" );
		success &= ( root == std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) ) );
		success &= ( root->GetChild( "child2" ) != nullptr );
		success &= ( root->GetChild( "newChild" ) != nullptr );

		assert( success );
	});
	
	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;

		success &= ( editor->DetachRootNode() );
		editor->DeleteDetachedNodes();
		editor->AttachRootNode();
		success &= ( !editor->GetRootNode()->GetChild( "root" ) );
		success &= ( !editor->DeleteRootNode() );
		success &= ( !editor->DetachRootNode() );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		
		bool success = true;
		auto newRoot = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "newRoot", 0.5f, 0.5f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK0_PATH );
		auto rootTransform = newRoot->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( rootTransform, 0, 0.0f, glm::vec3( -1.f, 0.5f, -1.f ) );

		editor->SetRootNode( newRoot );
		success &= ( editor->GetRootNode()->GetChild( "root" ) != nullptr );
		auto root = editor->GetRootNode()->GetChild( "root" );
		success &= ( root == std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) ) );

		assert( success );
	});
	
	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );
		
		bool success = true;

		success &= ( editor->DeleteRootNode() );
		success &= ( !editor->GetRootNode()->GetChild( "root" ) );
		success &= ( !editor->DeleteRootNode() );
		success &= ( !editor->DetachRootNode() );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		
		bool success = true;

		auto newRoot = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "newRoot", 0.5f, 0.5f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK0_PATH );
		auto rootTransform = newRoot->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( rootTransform, 0, 0.0f, glm::vec3( -1.f, 0.5f, -1.f ) );

		editor->SetRootNode( newRoot );
		success &= ( editor->GetRootNode()->GetChild( "root" ) != nullptr );
		auto root = editor->GetRootNode()->GetChild( "root" );
		success &= ( root == std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		
		bool success = true;

		auto newChild = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "newChild", 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		auto childTransform = newChild->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( childTransform, 0, 0.0f, glm::vec3( ( float )0.5, -0.5f, 0.2f ) );

		auto root = editor->GetRootNode()->GetChild( "root" );
		editor->AddChildNode( root, newChild );
		success &= ( root->GetChild( "newChild" ) != nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto root = editor->GetRootNode()->GetChild( "root" );

		for( UInt32 i = 0; i < root->GetNumChildren(); ++i )
		{
			editor->DeleteChildNode( root, std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) )->GetChild( 0 )->GetName() );
		}
	});
}

// ****************************
//
void					TestScene::InitTestEditor			()
{
	//InitTestModelSceneEditor();

	//InitBasicColorPluginTest();
	//InitOrderColorPluginTest();

	//InitBasicTexturePluginTest();
	//InitOrderTexturePluginTest();

	//InitBasicAnimationPluginTest();
	//InitOrderAnimationPluginTest();

	//InitBasicGradientPluginTest();
	//InitOrderGradientPluginTest();

	//InitColoredTextTest();
	InitGradientTextTest();
	
	//InitColoredTimerTest();
	//InitGradientTimerTest();

	//InitColoredGeometryTest();
	//InitTexturedGeometryTest();
	//InitAnimatedGeometryTest();
	//InitGradientGeometryTest();
}

// ****************************
//
void					TestScene::InitBasicColorPluginTest	()
{
	auto add = [&] 
	{ 
		auto editor = m_scene->GetSceneEditor();
		auto col = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, COL_NODE, 0.3f, 0.3f, glm::vec4( 0.f, 1.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( col->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.5f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->AddChildNode( root, col );

		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "rChild", 0.1f, 0.1f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( col, lChild );
		editor->AddChildNode( col, rChild );
		success &= ( col->GetNumChildren() == 2 );

		assert( success ); { success; }
	};

	m_testSteps.push_back( add );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, COL_NODE, "solid color", 2 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, COL_NODE, "solid color", 2 ); } );

	m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, COL_NODE, "alpha_mask", 3 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, COL_NODE, "alpha_mask", 3 ); } );
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
		auto col = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, COL_NODE, 0.3f, 0.3f, glm::vec4( 0.f, 1.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( col->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.5f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
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
	auto add0 = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto tex = TestSceneUtils::TexturedRectangle( m_timelineManager.get(), m_timeEvaluator, TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, TEX_NODE );
		editor->AddChildNode( root, tex );
	};

	auto add1 = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto tex = TestSceneUtils::TexturedRectangle( m_timelineManager.get(), m_timeEvaluator, TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, TEX_NODE );
		editor->AddChildNode( root, tex );
		
		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager.get(), m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( tex, lChild );
		editor->AddChildNode( tex, rChild );

		success &= ( tex->GetNumChildren() == 2 );
		
		assert( success );
	};

	m_testSteps.push_back( add0 );
	m_testSteps.push_back( add1 );
	
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TEX_NODE, "texture", 2 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "texture", 2, TEX_NODE, "solid color", 2 ); } );

	m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, TEX_NODE, "alpha_mask", 3 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, TEX_NODE, "alpha_mask", 3 ); } );

	m_testSteps.push_back( [&]
	{
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto child = root->GetChild( TEX_NODE );
		
		auto time = m_timeEvaluator->GetLocalTime();
		model::SetParameter( child->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "filteringMode" ), time, static_cast< Int32 >( TextureFilteringMode::TFM_POINT ) );
		model::SetParameter( child->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "filteringMode" ), time + TimeType( 1.0f ), static_cast< Int32 >( TextureFilteringMode::TFM_LINEAR ) );
	});
	m_testSteps.push_back( [&]{} );
	m_testSteps.push_back( [&]{} );

	m_testSteps.push_back( [&]{
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto child = root->GetChild( TEX_NODE );
		LoadTexture( child->GetPlugin( "texture" ), TestSceneUtils::ANIM_PATH + "/f0.bmp" );
	});
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
		auto tex = TestSceneUtils::TexturedRectangle( m_timelineManager.get(), m_timeEvaluator, TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
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
	auto add0 = [&] 
	{
		auto editor = m_scene->GetSceneEditor();

		auto anim = TestSceneUtils::AnimatedRectangle( m_timelineManager.get(), m_timeEvaluator, ANIM_NODE, 0.3f, 0.3f, TestSceneUtils::ANIM_PATH );
		SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
		
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, ANIM_NODE );
		editor->AddChildNode( root, anim );
	};

	auto add1 = [&] 
	{
		auto editor = m_scene->GetSceneEditor();

		auto anim = TestSceneUtils::AnimatedRectangle( m_timelineManager.get(), m_timeEvaluator, ANIM_NODE, 0.3f, 0.3f, TestSceneUtils::ANIM_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
		
		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, ANIM_NODE );
		editor->AddChildNode( root, anim );
		
		auto lChild = TestSceneUtils::TexturedRectangle( m_timelineManager.get(), m_timeEvaluator, "lChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "rChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( anim, lChild );
		editor->AddChildNode( anim, rChild );

		success &= ( anim->GetNumChildren() == 2 );
		
		assert( success );
	};

	m_testSteps.push_back( add0 );
	m_testSteps.push_back( add1 );

	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, ANIM_NODE, "animation", 2 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "animation", 2, ANIM_NODE, "solid color", 2 ); } );

	m_testSteps.push_back( [&]
	{
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto child = root->GetChild( ANIM_NODE );
		model::SetParameter( child->GetPlugin( "animation" )->GetResourceStateModel( "Tex0" )->GetParameter( "filteringMode" ), 0.0, static_cast< Int32 >( TextureFilteringMode::TFM_POINT ) );
	});

	m_testSteps.push_back( [&]{} );

	m_testSteps.push_back( [&]{
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto child = root->GetChild( ANIM_NODE );
		LoadAnimation( child->GetPlugin( "animation" ), TestSceneUtils::ANIM_PATH, "*.bmp" );

		auto time = m_timeEvaluator->GetLocalTime();
		SetParameter( child->GetPlugin( "animation" )->GetParameter( "frameNum" ), time, 0.f );
	    SetParameter( child->GetPlugin( "animation" )->GetParameter( "frameNum" ), time + TimeType( 0.25f * ( Float32 )TestSceneUtils::ANIM_NUM /TestSceneUtils:: SPEED ), ( Float32 )( TestSceneUtils::ANIM_NUM ) );
	});
	
	m_testSteps.push_back( [&]{} );
	m_testSteps.push_back( [&]{} );
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
		auto anim = TestSceneUtils::AnimatedRectangle( m_timelineManager.get(), m_timeEvaluator, ANIM_NODE, 0.3f, 0.3f, TestSceneUtils::ANIM_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
		
		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
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
		m_testSteps.push_back( [&]{} );
		m_testSteps.push_back( [&]{} );
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

		auto grad = TestSceneUtils::GradientRectangle( m_timelineManager.get(), m_timeEvaluator, GRAD_NODE, 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( grad->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->AddChildNode( root, grad );
		
		auto lChild = TestSceneUtils::AnimatedRectangle( m_timelineManager.get(), m_timeEvaluator, "lChild", 0.1f, 0.1f, TestSceneUtils::ANIM_PATH );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager.get(), m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( grad, lChild );
		editor->AddChildNode( grad, rChild );

		success &= ( grad->GetNumChildren() == 2 );

		assert( success );
	};

	m_testSteps.push_back( add );
	m_testSteps.push_back( [&]
	{
		auto child = std::static_pointer_cast< model::BasicNode >( std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) )->GetChild( GRAD_NODE ) );
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
		auto grad = TestSceneUtils::GradientRectangle( m_timelineManager.get(), m_timeEvaluator, GRAD_NODE, 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( grad->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 1.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
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
	auto add0 = [&] 
	{
		auto editor = m_scene->GetSceneEditor();

		auto txt = TestSceneUtils::ColoredText( m_timelineManager.get(), m_timeEvaluator, TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 60 );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );
		
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, TXT_NODE );
		editor->AddChildNode( root, txt );
	};

	auto add1 = [&] 
	{
		auto editor = m_scene->GetSceneEditor();

		auto txt = TestSceneUtils::ColoredText( m_timelineManager.get(), m_timeEvaluator, TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 60, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, TXT_NODE );
		editor->AddChildNode( root, txt );
		
		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager.get(), m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
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
		auto txt = TestSceneUtils::ColoredText( m_timelineManager.get(), m_timeEvaluator, TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 60, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
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


	m_testSteps.push_back( add0 );
	m_testSteps.push_back( add1 );
	
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TXT_NODE, "solid color", 1 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TXT_NODE, "solid color", 1 ); } );

	m_testSteps.push_back( [&]
	{ 
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto child = root->GetChild( TXT_NODE );
		model::DefaultTextPlugin::SetText( child->GetPlugin( "text" ), L"test0\n4321" );
	});

	m_testSteps.push_back( [&]
	{ 
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto child = root->GetChild( TXT_NODE );
		LoadFont( child->GetPlugin( "text" ), "fonts/couri.TTF", 30, 0, 0, true );
	});

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
		InitOrderTest( test );
	}
	m_testSteps.push_back( recoverScene );
}

// ****************************
//
void					TestScene::InitGradientTextTest			()
{
	auto add0 = [&]
	{
		auto editor = m_scene->GetSceneEditor();

		auto txt = TestSceneUtils::GradientText( m_timelineManager.get(), m_timeEvaluator, TXT_NODE, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 0.f, 1.f, 0.f, 1.f ), 60 );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, TXT_NODE );
		editor->AddChildNode( root, txt );
	};

	auto add1 = [&] 
	{
		auto editor = m_scene->GetSceneEditor();

		auto txt = TestSceneUtils::GradientText( m_timelineManager.get(), m_timeEvaluator, TXT_NODE, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 0.f, 1.f, 0.f, 1.f ), 60, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, TXT_NODE );
		editor->AddChildNode( root, txt );
		
		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager.get(), m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( txt, lChild );
		editor->AddChildNode( txt, rChild );

		success &= ( txt->GetNumChildren() == 2 );

		assert( success );
	};

	std::string test0[] = { "text", "alpha_mask" };
	std::string test1[] = { "alpha_mask", "text" };
	std::string test2[] = { "alpha_mask", "transform", "linear_gradient", "text" };
	std::string test3[] = { "transform", "alpha_mask", "linear_gradient", "text" };
	std::string test4[] = { "transform", "linear_gradient", "alpha_mask", "text" };
	std::string test5[] = { "alpha_mask", "linear_gradient", "transform", "text" };
	std::string test6[] = { "linear_gradient", "alpha_mask", "transform", "text" };
	std::string test7[] = { "linear_gradient", "transform", "alpha_mask", "text" };
	std::string test8[] = { "transform", "text", "alpha_mask", "linear_gradient" };
	std::string test9[] = { "transform", "alpha_mask", "text", "linear_gradient" };
	std::string test10[] = { "linear_gradient", "text", "alpha_mask", "transform" };
	std::string test11[] = { "linear_gradient", "alpha_mask", "text", "transform" };

	std::vector < OrderTestCase > tests;
	tests.push_back( OrderTestCase( TXT_NODE, "TxtAm", std::vector< std::string >( test0, test0 + 2 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "AmTxt", std::vector< std::string >( test1, test1 + 2 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "AmTLgTxt", std::vector< std::string >( test2, test2 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "TAmLgTxt", std::vector< std::string >( test3, test3 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "TLgAmTxt", std::vector< std::string >( test4, test4 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "AmLgTTxt", std::vector< std::string >( test5, test5 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "LgAmTTxt", std::vector< std::string >( test6, test6 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "LgTAmTxt", std::vector< std::string >( test7, test7 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "TTxtAmLg", std::vector< std::string >( test8, test8 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "TAmTxtLg", std::vector< std::string >( test9, test9 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "LgTxtAmT", std::vector< std::string >( test10, test10 + 4 ) ) );
	tests.push_back( OrderTestCase( TXT_NODE, "LgAmTxtT", std::vector< std::string >( test11, test11 + 4 ) ) );

	auto recoverScene = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto txt = TestSceneUtils::GradientText( m_timelineManager.get(), m_timeEvaluator, TXT_NODE, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 0.f, 1.f, 0.f, 1.f ), 60, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto lChild = root->GetChild( TXT_NODE )->GetChild( "lChild" );
		auto rChild = root->GetChild( TXT_NODE )->GetChild( "rChild" );
		success &= editor->DeleteChildNode( root, TXT_NODE );
		editor->AddChildNode( root, txt );
		editor->AddChildNode( txt, lChild );
		editor->AddChildNode( txt, rChild );
		success &= ( txt->GetNumChildren() == 2 );
		
		assert( success );
	};


//---------------


	m_testSteps.push_back( add0 );
	m_testSteps.push_back( add1 );

	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TXT_NODE, "linear_gradient", 1 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "linear_gradient", 2, TXT_NODE, "solid color", 2 ); } );

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
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

		auto tmr = TestSceneUtils::ColoredTimer( m_timelineManager.get(), m_timeEvaluator, TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 60, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, TMR_NODE );
		editor->AddChildNode( root, tmr );
		
		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager.get(), m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( tmr, lChild );
		editor->AddChildNode( tmr, rChild );

		success &= ( tmr->GetNumChildren() == 2 );

		assert( success );
	};

	std::string test0[] = { "timer", "alpha_mask" };
	std::string test1[] = { "alpha_mask", "timer" };
	std::string test2[] = { "alpha_mask", "transform", "solid color", "timer" };
	std::string test3[] = { "transform", "alpha_mask", "solid color", "timer" };
	std::string test4[] = { "transform", "solid color", "alpha_mask", "timer" };
	std::string test5[] = { "alpha_mask", "solid color", "transform", "timer" };
	std::string test6[] = { "solid color", "alpha_mask", "transform", "timer" };
	std::string test7[] = { "solid color", "transform", "alpha_mask", "timer" };
	std::string test8[] = { "transform", "timer", "alpha_mask", "solid color" };
	std::string test9[] = { "transform", "alpha_mask", "timer", "solid color" };
	std::string test10[] = { "solid color", "timer", "alpha_mask", "transform" };
	std::string test11[] = { "solid color", "alpha_mask", "timer", "transform" };

	std::vector < OrderTestCase > tests;
	tests.push_back( OrderTestCase( TMR_NODE, "TmAm", std::vector< std::string >( test0, test0 + 2 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "AmTm", std::vector< std::string >( test1, test1 + 2 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "AmTCTm", std::vector< std::string >( test2, test2 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "TAmCTm", std::vector< std::string >( test3, test3 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "TCAmTm", std::vector< std::string >( test4, test4 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "AmCTTm", std::vector< std::string >( test5, test5 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "CAmTTm", std::vector< std::string >( test6, test6 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "CTAmTm", std::vector< std::string >( test7, test7 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "TTmAmC", std::vector< std::string >( test8, test8 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "TAmTmC", std::vector< std::string >( test9, test9 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "CTmAmT", std::vector< std::string >( test10, test10 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "CAmTmT", std::vector< std::string >( test11, test11 + 4 ) ) );

	auto recoverScene = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto tmr = TestSceneUtils::ColoredTimer( m_timelineManager.get(), m_timeEvaluator, TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 60, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
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
	m_testSteps.push_back( []{} ); //empty step
	
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TMR_NODE, "solid color", 1 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TMR_NODE, "solid color", 1 ); } );

	m_testSteps.push_back( [&]
	{ 
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto child = root->GetChild( TMR_NODE );
		model::SetTimeTimerPlugin( child->GetPlugin( "timer" ), 15.0f );
		model::StartTimerPlugin( child->GetPlugin( "timer" ) );
	});
	m_testSteps.push_back( []{} ); //empty step

	m_testSteps.push_back( [&]
	{ 
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto child = root->GetChild( TMR_NODE );
		LoadFont( child->GetPlugin( "timer" ), "Assets/Fonts/couri.TTF", 40, 0, 0, true );
	});
	m_testSteps.push_back( []{} ); //empty step

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
		m_testSteps.push_back( []{} ); //empty step
		InitOrderTest( test );
	}
}

// ****************************
//
void					TestScene::InitGradientTimerTest			()
{
	auto add0 = [&]
	{
		auto editor = m_scene->GetSceneEditor();

		auto tmr = TestSceneUtils::GradientTimer( m_timelineManager.get(), m_timeEvaluator, TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), 60 );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, TMR_NODE );
		editor->AddChildNode( root, tmr );
	};

	auto add1 = [&]
	{
		auto editor = m_scene->GetSceneEditor();

		auto tmr = TestSceneUtils::GradientTimer( m_timelineManager.get(), m_timeEvaluator, TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), 60, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		editor->DeleteChildNode( root, TMR_NODE );
		editor->AddChildNode( root, tmr );
		
		auto lChild = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( m_timelineManager.get(), m_timeEvaluator, "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( tmr, lChild );
		editor->AddChildNode( tmr, rChild );

		success &= ( tmr->GetNumChildren() == 2 );

		assert( success );
	};

	std::string test0[] = { "linear_gradient", "timer" };
	std::string test1[] = { "alpha_mask", "timer" };
	std::string test2[] = { "alpha_mask", "transform", "linear_gradient", "timer" };
	std::string test3[] = { "transform", "alpha_mask", "linear_gradient", "timer" };
	std::string test4[] = { "transform", "linear_gradient", "alpha_mask", "timer" };
	std::string test5[] = { "alpha_mask", "linear_gradient", "transform", "timer" };
	std::string test6[] = { "linear_gradient", "alpha_mask", "transform", "timer" };
	std::string test7[] = { "linear_gradient", "transform", "alpha_mask", "timer" };
	std::string test8[] = { "transform", "timer", "alpha_mask", "linear_gradient" };
	std::string test9[] = { "transform", "alpha_mask", "timer", "linear_gradient" };
	std::string test10[] = { "linear_gradient", "timer", "alpha_mask", "transform" };
	std::string test11[] = { "linear_gradient", "alpha_mask", "timer", "transform" };

	std::vector < OrderTestCase > tests;
	tests.push_back( OrderTestCase( TMR_NODE, "LgTm", std::vector< std::string >( test0, test0 + 2 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "AmTm", std::vector< std::string >( test1, test1 + 2 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "AmTCTm", std::vector< std::string >( test2, test2 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "TAmCTm", std::vector< std::string >( test3, test3 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "TCAmTm", std::vector< std::string >( test4, test4 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "AmCTTm", std::vector< std::string >( test5, test5 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "CAmTTm", std::vector< std::string >( test6, test6 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "CTAmTm", std::vector< std::string >( test7, test7 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "TTmAmC", std::vector< std::string >( test8, test8 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "TAmTmC", std::vector< std::string >( test9, test9 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "CTmAmT", std::vector< std::string >( test10, test10 + 4 ) ) );
	tests.push_back( OrderTestCase( TMR_NODE, "CAmTmT", std::vector< std::string >( test11, test11 + 4 ) ) );

	auto recoverScene = [&] 
	{
		auto editor = m_scene->GetSceneEditor();
		auto tmr = TestSceneUtils::GradientTimer( m_timelineManager.get(), m_timeEvaluator, TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), 60, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		bool success = true;

		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
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

	m_testSteps.push_back( add0 );
	m_testSteps.push_back( add1 );
	m_testSteps.push_back( []{} ); //empty step
	
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TMR_NODE, "linear_gradient", 1 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "linear_gradient", 2, TMR_NODE, "solid color", 2 ); } );

	m_testSteps.push_back( [&]
	{ 
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto child = root->GetChild( TMR_NODE );
		model::SetTimeTimerPlugin( child->GetPlugin( "timer" ), 15.0f );
		model::StartTimerPlugin( child->GetPlugin( "timer" ) );
	});
	m_testSteps.push_back( []{} ); //empty step

	m_testSteps.push_back( [&]
	{ 
		auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
		auto child = root->GetChild( TMR_NODE );
		LoadFont( child->GetPlugin( "timer" ), "Assets/Fonts/couri.TTF", 40, 0, 0, true );
	});
	m_testSteps.push_back( []{} ); //empty step

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
		m_testSteps.push_back( []{} ); //empty step
		InitOrderTest( test );
	}
	m_testSteps.push_back( recoverScene );
}

// ****************************
//
void					TestScene::InitColoredGeometryTest		()
{
	for( unsigned int i = 0; i < TestSceneUtils::GEOM_PLUGINS_NUM; ++i )
	{
		auto plugin = TestSceneUtils::PluginsArr[ i ];
		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_scene->GetSceneEditor();

			auto geom = TestSceneUtils::ColoredGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
			
			auto time = m_timeEvaluator->GetLocalTime(); {time;}
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 1.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
			editor->DeleteChildNode( root, GEOM_NODE );
			editor->AddChildNode( root, geom );
		});
		m_testSteps.push_back( []{} );

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_scene->GetSceneEditor();

			auto geom = TestSceneUtils::ColoredGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
			
			auto time = m_timeEvaluator->GetLocalTime();
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 5.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
			editor->DeleteChildNode( root, GEOM_NODE );
			editor->AddChildNode( root, geom );
		});
		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GEOM_NODE, "solid color", 2 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GEOM_NODE, "solid color", 2 ); } );
				
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );

		auto pluginName = TestSceneUtils::PluginsNameArr[ i ];
		std::string test0[] = { "alpha_mask", pluginName };
		std::string test1[] = { "solid color", pluginName };

		std::vector < OrderTestCase > tests;
		tests.push_back( OrderTestCase( GEOM_NODE, "AmGeom", std::vector< std::string >( test0, test0 + 2 ) ) );
		tests.push_back( OrderTestCase( GEOM_NODE, "CGeom", std::vector< std::string >( test1, test1 + 2 ) ) );

		for( auto & test : tests )
		{
			m_testSteps.push_back( [ plugin, this ]
			{
				auto editor = m_scene->GetSceneEditor();

				auto geom = TestSceneUtils::ColoredGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
				
				auto time = m_timeEvaluator->GetLocalTime();
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 3.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

				auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
				editor->DeleteChildNode( root, GEOM_NODE );
				editor->AddChildNode( root, geom );
			});
			InitOrderTest( test );
		}
	}
}

// ****************************
//
void					TestScene::InitTexturedGeometryTest		()
{
	for( unsigned int i = 0; i < TestSceneUtils::GEOM_PLUGINS_NUM; ++i )
	{
		auto plugin = TestSceneUtils::PluginsArr[ i ];

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_scene->GetSceneEditor();

			auto geom = TestSceneUtils::TexturedGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, TestSceneUtils::TEXTURE_PATH );
			
			auto time = m_timeEvaluator->GetLocalTime();
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 1.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
			editor->DeleteChildNode( root, GEOM_NODE );
			editor->AddChildNode( root, geom );
		});
		m_testSteps.push_back( []{} );

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_scene->GetSceneEditor();

			auto geom = TestSceneUtils::TexturedGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
			
			auto time = m_timeEvaluator->GetLocalTime();
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 5.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
			editor->DeleteChildNode( root, GEOM_NODE );
			editor->AddChildNode( root, geom );
		});
		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GEOM_NODE, "texture", 2 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "texture", 2, GEOM_NODE, "solid color", 2 ); } );
				
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );

		m_testSteps.push_back( [&]
		{
			auto editor = m_scene->GetSceneEditor();
			auto geom = editor->GetRootNode()->GetChild( "root" )->GetChild( GEOM_NODE );
			model::LoadTexture( geom->GetPlugin( "texture" ), TestSceneUtils::ANIM_PATH + "/f0.bmp" );
		});

		auto pluginName = TestSceneUtils::PluginsNameArr[ i ];
		std::string test0[] = { "alpha_mask", pluginName };
		std::string test1[] = { "texture", pluginName };

		std::vector < OrderTestCase > tests;
		tests.push_back( OrderTestCase( GEOM_NODE, "AmGeom", std::vector< std::string >( test0, test0 + 2 ) ) );
		tests.push_back( OrderTestCase( GEOM_NODE, "TexGeom", std::vector< std::string >( test1, test1 + 2 ) ) );

		for( auto & test : tests )
		{
			m_testSteps.push_back( [ plugin, this ]
			{
				auto editor = m_scene->GetSceneEditor();

				auto geom = TestSceneUtils::TexturedGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );

				auto time = m_timeEvaluator->GetLocalTime();
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 3.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

				auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
				editor->DeleteChildNode( root, GEOM_NODE );
				editor->AddChildNode( root, geom );
			});
			InitOrderTest( test );
		}
	}
}

// ****************************
//
void					TestScene::InitAnimatedGeometryTest		()
{
	for( unsigned int i = 0; i < TestSceneUtils::GEOM_PLUGINS_NUM; ++i )
	{
		auto plugin = TestSceneUtils::PluginsArr[ i ];

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_scene->GetSceneEditor();

			auto geom = TestSceneUtils::AnimatedGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, TestSceneUtils::ANIM_PATH );
			
			auto time = m_timeEvaluator->GetLocalTime();
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 1.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
			editor->DeleteChildNode( root, GEOM_NODE );
			editor->AddChildNode( root, geom );
		});
		m_testSteps.push_back( []{} );

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_scene->GetSceneEditor();

			auto geom = TestSceneUtils::AnimatedGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, TestSceneUtils::ANIM_PATH, TestSceneUtils::ALPHA_MASK_PATH );
			
			auto time = m_timeEvaluator->GetLocalTime();
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 5.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
			editor->DeleteChildNode( root, GEOM_NODE );
			editor->AddChildNode( root, geom );
		});
		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GEOM_NODE, "animation", 2 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "animation", 2, GEOM_NODE, "solid color", 2 ); } );
				
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );

		auto pluginName = TestSceneUtils::PluginsNameArr[ i ];
		std::string test0[] = { "alpha_mask", pluginName };
		std::string test1[] = { "animation", pluginName };

		std::vector < OrderTestCase > tests;
		tests.push_back( OrderTestCase( GEOM_NODE, "AmGeom", std::vector< std::string >( test0, test0 + 2 ) ) );
		tests.push_back( OrderTestCase( GEOM_NODE, "AGeom", std::vector< std::string >( test1, test1 + 2 ) ) );

		for( auto & test : tests )
		{
			m_testSteps.push_back( [ plugin, this ]
			{
				auto editor = m_scene->GetSceneEditor();

				auto geom = TestSceneUtils::AnimatedGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, TestSceneUtils::ANIM_PATH, TestSceneUtils::ALPHA_MASK_PATH );

				auto time = m_timeEvaluator->GetLocalTime();
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 3.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

				auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
				editor->DeleteChildNode( root, GEOM_NODE );
				editor->AddChildNode( root, geom );
			});
			InitOrderTest( test );
		}
	}
}

// ****************************
//
void					TestScene::InitGradientGeometryTest		()
{
	for( unsigned int i = 0; i < TestSceneUtils::GEOM_PLUGINS_NUM; ++i )
	{
		auto plugin = TestSceneUtils::PluginsArr[ i ];

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_scene->GetSceneEditor();

			auto time = m_timeEvaluator->GetLocalTime();
			auto geom = TestSceneUtils::GradientGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
			
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 1.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
			editor->DeleteChildNode( root, GEOM_NODE );
			editor->AddChildNode( root, geom );
		});
		m_testSteps.push_back( []{} );

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_scene->GetSceneEditor();

			auto time = m_timeEvaluator->GetLocalTime();
			auto geom = TestSceneUtils::GradientGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
			
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 5.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
			editor->DeleteChildNode( root, GEOM_NODE );
			editor->AddChildNode( root, geom );
		});

		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GEOM_NODE, "linear_gradient", 2 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "linear_gradient", 2, GEOM_NODE, "solid color", 2 ); } );
				
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );

		auto pluginName = TestSceneUtils::PluginsNameArr[ i ];
		std::string test0[] = { "alpha_mask", pluginName };
		std::string test1[] = { "linear_gradient", pluginName };

		std::vector < OrderTestCase > tests;
		tests.push_back( OrderTestCase( GEOM_NODE, "AmGeom", std::vector< std::string >( test0, test0 + 2 ) ) );
		tests.push_back( OrderTestCase( GEOM_NODE, "LgGeom", std::vector< std::string >( test1, test1 + 2 ) ) );

		for( auto & test : tests )
		{
			m_testSteps.push_back( [ plugin, this ]
			{
				auto editor = m_scene->GetSceneEditor();

				auto geom = TestSceneUtils::GradientGeometry( m_timelineManager.get(), m_timeEvaluator, GEOM_NODE, plugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
			
				auto time = m_timeEvaluator->GetLocalTime();
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, time + 3.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

				auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
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
			auto child = std::static_pointer_cast< model::BasicNode >( std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) )->GetChild( node ) );
			
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

	auto root = std::static_pointer_cast< model::BasicNode >( m_scene->GetModelSceneRoot()->GetChild( "root" ) );
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
void					TestScene::TestEditor				( TimeType time )
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
	auto root = TestSceneUtils::ColoredRectangle( m_timelineManager.get(), m_timeEvaluator, "root", 0.5f, 0.5f, glm::vec4( 0.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK0_PATH );
    auto rootTransform  = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    SetParameterTranslation( rootTransform, 0, 0.0f, glm::vec3( -1.f, 0.5f, -1.f ) );
	
    return BVScene::Create( model::SceneModel::Create( "root", m_timelineManager, root ), new Camera(), m_timeEvaluator, m_renderer );
}

} // bv