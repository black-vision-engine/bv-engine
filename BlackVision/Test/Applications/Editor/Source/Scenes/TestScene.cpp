#include "TestScene.h"

#include <cassert>

#include "TestSceneUtils.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Tools/PrefixHelper.h"

#include "Assets/Assets.h"

#include "System/Path.h"
#include "IO/FileIO.h"
#include "IO/DirIO.h"

#include "Engine/Models/Plugins/Simple/DefaultVideoStreamDecoderPlugin.h"
#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"

#include "ProjectManager.h"

#include "Serialization/BV/XML/BVXMLSerializer.h"

namespace bv {

const std::string	TestScene::COL_NODE		= "col";
const std::string	TestScene::TEX_NODE		= "tex";
const std::string	TestScene::ANIM_NODE	= "anim";
const std::string	TestScene::GRAD_NODE	= "grad";
const std::string	TestScene::TXT_NODE		= "txt";
const std::string	TestScene::TMR_NODE		= "tmr";
const std::string	TestScene::GEOM_NODE	= "geom";
const std::string	TestScene::VSD_NODE		= "vsd";

const std::string	TestScene::SCENE_NAME	= "scene_0";
const std::string	TestScene::SCENE_NAME1	= "scene_1";
const std::string	TestScene::EMPTY_SCENE	= "empty_scene";

const std::string	TestScene::TIMELINE_NAME = "timeline_0";
const std::string	TestScene::TIMELINE_NAME1 = "timeline_1";

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
						TestScene::TestScene				( BVProject * scene, Renderer * renderer )
		: m_project( scene )
		, m_renderer( renderer )
		, m_lastStep( -1 )
		, m_stepOffset( 0 )
{
	auto size = TestSceneUtils::IMG_SIZE;

	auto dir = "DefaultPMDir/sequences/" + ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str();
	if( ( !Path::Exists( dir ) ) && ( !dir.empty() ) )
            Dir::CreateDir( dir, true );

	TestSceneUtils::GenerateCheckboardTex( TestSceneUtils::TEXTURE_PATH, size, size, glm::uvec3( 32 ) );
	TestSceneUtils::GenerateCheckboardAlphaMaskTex( TestSceneUtils::ALPHA_MASK_PATH, TestSceneUtils::AM_SIZE, TestSceneUtils::AM_SIZE );
	TestSceneUtils::GenerateCheckboardAlphaMaskTex( TestSceneUtils::ALPHA_MASK0_PATH, TestSceneUtils::AM_SIZE, TestSceneUtils::AM_SIZE, 64 );
	TestSceneUtils::GenerateCheckboardAnim( dir, size, size, TestSceneUtils::ANIM_NUM );

    auto editor = m_project->GetProjectEditor();

    TestSceneUtils::AddColoredRectangleScene( editor, SCENE_NAME, glm::vec4( 0.f, 0.f, 1.f, 1.f ), glm::vec3( -1.f, 0.5f, -1.f ) );

	m_timelineManager = model::TimelineManager::GetInstance();
	m_timeEvaluator = m_timelineManager->GetRootTimeline();

	InitTestEditor();
}

// ****************************
//
						TestScene::~TestScene				()
{
}

// ****************************
//
void					TestScene::InitTestEditor			()
{
	//InitTestModelSceneEditor();

	//InitTimelinesTest();

	//InitAssetsTest();

    InitCopyNodeTest();

	//InitBasicColorPluginTest();
	//InitOrderColorPluginTest();

    //InitBasicTexturePluginTest();
	//InitOrderTexturePluginTest();

	//InitBasicAnimationPluginTest();
	//InitOrderAnimationPluginTest();

	//InitBasicGradientPluginTest();
	//InitOrderGradientPluginTest();

	//InitColoredTextTest();
	//InitGradientTextTest();
	
	//InitColoredTimerTest();
	//InitGradientTimerTest();

	//InitColoredGeometryTest();
	//InitTexturedGeometryTest();
	//InitAnimatedGeometryTest();
	//InitGradientGeometryTest();

	//InitVideoStreamDecoderTest();
}

// ****************************
//
void                    TestScene::InitTestModelSceneEditor ()
{
    m_testSteps.push_back([&] 
    { 
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        auto timeline = editor->GetSceneDefaultTimeline( scene );
        for( unsigned int i = 0; i < 3; ++i )
        {
            auto child = TestSceneUtils::ColoredRectangle( timeline, "child" + toString( i ), 0.3f, 0.3f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
            auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
            SetParameterTranslation( childTransform, 0.0f, glm::vec3( ( float )0.5*i, -0.5f, 0.f ) );
            editor->AddChildNode( scene, root, child );
        }
        assert( root->GetNumChildren() == 3 );

        for( unsigned int i = 0; i < 3; ++i )
        {
            auto child = TestSceneUtils::ColoredRectangle( timeline, "child0" + toString( i ), 0.2f, 0.2f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
            auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
            SetParameterTranslation( childTransform, 0.0f, glm::vec3( ( float )i, -0.5f, 0.f ) );
            editor->AddChildNode( scene, root->GetChild( "child0" ), child );
        }
        assert( root->GetChild( "child0" )->GetNumChildren() == 3 );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto node0 = editor->GetNode( SCENE_NAME, "/root/child0/child01" );
        auto node1 = editor->GetNode( SCENE_NAME, "/root/child01" );
        auto node2 = editor->GetNode( SCENE_NAME, "/root/child0" );
        auto node3 = editor->GetNode( SCENE_NAME, "/root/child2" );
        auto node4 = editor->GetNode( SCENE_NAME, "/root/child2/child00" );
        bool success = true;

        success &= ( node0->GetName() == "child01" );
        success &= ( node1 == nullptr );
        success &= ( node2->GetName() == "child0" );
        success &= ( node3->GetName() == "child2" );
        success &= ( node4 == nullptr );

        assert( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto node0 = editor->GetNode( SCENE_NAME, "/root/child0/child01" );
        auto node1 = editor->GetNode( SCENE_NAME, "/root/#5" );
        auto node2 = editor->GetNode( SCENE_NAME, "/root/#0" );
        auto node3 = editor->GetNode( SCENE_NAME, "/root/#2" );
        auto node4 = editor->GetNode( SCENE_NAME, "/root/#2/child00" );
        auto node5 = editor->GetNode( SCENE_NAME, "/root/#0/child01" );
        auto node6 = editor->GetNode( SCENE_NAME, "/root/child0/#2" );
        auto node7 = editor->GetNode( SCENE_NAME, "/#0/#0/#2" );
        auto node8 = editor->GetNode( SCENE_NAME, "/#0/#0/#1" );
        auto node9 = editor->GetNode( SCENE_NAME, "/#1/child0/#2" );
        bool success = true;

        success &= ( node0->GetName() == "child01" );
        success &= ( node1 == nullptr );
        success &= ( node2->GetName() == "child0" );
        success &= ( node3->GetName() == "child2" );
        success &= ( node4 == nullptr );
        success &= ( node5->GetName() == "child01" );
        success &= ( node6->GetName() == "child02" );
        success &= ( node6 == node7 );
        success &= ( node0 == node8 );
        success &= ( node9 == nullptr );

        assert( success );
    });

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
        bool success = true;    { success; }

		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		auto child = root->GetChild( 0 );

		auto effect = std::make_shared< model::ModelNodeEffectAlphaMask >( m_timeEvaluator );

		editor->SetNodeEffect( child, effect ); 

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
        bool success = true;    { success; }

		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		auto child = root->GetChild( 0 );

        model::SetParameter( editor->GetNodeEffect( child )->GetParameter( "alpha" ), 0.f, 0.5f );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		editor->AddScene( EMPTY_SCENE );

        auto scene = editor->GetScene( EMPTY_SCENE );
        auto root = scene->GetRootNode();

		success &= ( scene != nullptr );
        success &= ( root != nullptr );
        success &= ( root->GetName() == "root" );
        success &= ( root->GetNumPlugins() == 1 );
        success &= ( root->GetPlugin( "transform" ) != nullptr );

		assert( success );
	});

    m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

        success = editor->DetachScene( EMPTY_SCENE );

		assert( success );
	});

    
    m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

         success = editor->AttachScene( EMPTY_SCENE, 0 );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( EMPTY_SCENE );
		bool success = true;

		auto root = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "newRoot", 0.4f, 0.4f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		auto rootTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( rootTransform, 0.0f, glm::vec3( ( float )0.0f, 0.0f, -1.0f ) );

		editor->AddChildNode( scene, nullptr, root );

		success &= ( editor->GetScene( EMPTY_SCENE ) != nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		editor->MoveScene( EMPTY_SCENE, 0 );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( EMPTY_SCENE );
        bool success = true;

        auto root = model::BasicNode::Create( "root", nullptr );
        success &= editor->AddChildNode( scene, nullptr, root );

        assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		auto copied = editor->AddSceneCopy( SCENE_NAME );
		auto root = copied->GetRootNode();
		SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.5f, -1.f ) );

		for( UInt32 i = 0; i < root->GetNumChildren(); ++i )
		{
			model::SetParameter( root->GetChild( i )->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		}

		success &= ( editor->GetScene( SCENE_NAME ) != nullptr );
		success &= ( editor->GetScene( "Copy_" + SCENE_NAME ) != nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		auto copiedName = "Copy_" + SCENE_NAME;

		auto copied = editor->AddSceneCopy( copiedName );
		auto root = copied->GetRootNode();
		SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, 0.5f, -1.f ) );

		editor->RemoveScene( "Copy_" + SCENE_NAME );

		success &= ( editor->GetScene( SCENE_NAME ) != nullptr );
		success &= ( editor->GetScene( "Copy_" + SCENE_NAME ) == nullptr );
		success &= ( editor->GetScene( "Copy1_" + SCENE_NAME ) != nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		editor->RenameScene( "Copy1_" + SCENE_NAME, SCENE_NAME1 );
		success &= ( editor->GetScene( SCENE_NAME1 ) != nullptr );
		success &= ( editor->GetScene( "Copy1_" + SCENE_NAME ) == nullptr );
		//editor->AddScene( model::SceneModel::CreateEmptyScene( SCENE_NAME1 ) );

		auto scene = editor->GetScene( SCENE_NAME1 );
		auto copied = editor->AddNodeCopy( scene, scene->GetRootNode(), scene, scene->GetRootNode() );
		SetParameterTranslation( copied->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.5f, -1.f ) );

		success &= ( editor->GetScene( SCENE_NAME ) != nullptr );
		success &= ( editor->GetScene( SCENE_NAME1 ) != nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		auto scene = editor->GetScene( SCENE_NAME1 );
		auto root = scene->GetRootNode();
		SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.3f, 0.5f, -1.f ) );

		for( UInt32 i = 0; i < root->GetNumChildren(); ++i )
		{
			model::SetParameter( root->GetChild( i )->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		}

		success &= ( editor->GetScene( SCENE_NAME ) != nullptr );
		success &= ( editor->GetScene( SCENE_NAME1 ) != nullptr );

		assert( success );
	});
	
	m_testSteps.push_back([&] 
	{ 
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		bool success = true;
		success &= ( root->GetName() == "root" );
		success &= ( root == std::static_pointer_cast< model::BasicNode >( m_project->GetModelSceneRoot()->GetChild( "root" ) ) );
		
		assert( success );
        
		auto child = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "newChild", 0.2f, 0.2f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( childTransform, 0.0f, glm::vec3( ( float )1.5f, -0.5f, 0.f ) );

		editor->AddChildNode( scene, root, child );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		editor->SetSceneVisible( SCENE_NAME, false );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		editor->SetSceneVisible( SCENE_NAME, true );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		editor->DetachScene( SCENE_NAME );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		editor->DetachScene( EMPTY_SCENE );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		editor->AttachScene( SCENE_NAME );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		editor->AttachScene( EMPTY_SCENE );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();

		bool success = true;
		
		success &= ( root->GetChild( 0 )->GetName() != "newChild" );

		editor->MoveNode( SCENE_NAME, "root", 0, SCENE_NAME, "root/newChild" );

		success &= ( root->GetChild( 0 )->GetName() == "newChild" );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{ 
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		
		bool success = true;

		auto child0 = root->GetChild( "child0" );
		success &= ( child0->GetName() == "child0" );

		success &= ( child0->GetChild( "child00" ) != nullptr );
		success &= ( editor->DetachChildNode( SCENE_NAME, "/root/child0/child00" ) );
		success &= ( !child0->GetChild( "child00" ) );
		success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child0/child00" ) );

		assert( success );
	});
	
	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		
		bool success = true;
		auto child0 = root->GetChild( "child0" );
		success &= ( child0->GetName() == "child0" );

		success &= ( child0->GetChild( "child02" ) != nullptr  );
		success &= ( editor->DetachChildNode( SCENE_NAME, "/root/child0/child02" ) );
		success &= ( !child0->GetChild( "child02" ) );
		success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child0/child02" ) );
	
		assert( success );
	});
	
	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		
		bool success = true;
		auto child0 = root->GetChild( "child0" );
		success &= ( child0->GetName() == "child0" );

		success &= ( child0->GetChild( "child01" ) != nullptr  );
		success &= ( editor->DetachChildNode( SCENE_NAME, "/root/child0/child01" ) );
		success &= ( !child0->GetChild( "child01" ) );
		success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child0/child01" ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		
		bool success = true;

		success &= ( editor->DetachChildNode( SCENE_NAME, "/root/child0" ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		
		bool success = true;

		success &= ( editor->AttachChildNode( scene, root ) );
		success &= ( root->GetChild( "child0" ) != nullptr );
		success &= ( !editor->AttachChildNode( scene, root ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		
		bool success = true;

		success &= ( root->GetChild( "child0" ) != nullptr );
        success &= ( editor->DeleteChildNode( SCENE_NAME, "#0/#3" ) );

		//success &= ( editor->DeleteChildNode( scene, root, "child0" ) );
		success &= ( !root->GetChild( "child0" ) );
		success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child0" ) );
		success &= ( !editor->AttachChildNode( scene, root ) );
		success &= ( !editor->DeleteChildNode( SCENE_NAME, "/root/child0" ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		
		bool success = true;

		success &= ( root->GetChild( "child1" ) != nullptr );
		success &= ( editor->DeleteChildNode( SCENE_NAME, "/root/child1" ) );
		success &= ( !root->GetChild( "child1" ) );
		success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child1" ) );
		success &= ( !editor->AttachChildNode( scene, root ) );
		success &= ( !editor->DeleteChildNode( SCENE_NAME, "/root/child1" ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		
		bool success = true;

		editor->DeleteDetachedNodes( SCENE_NAME );
		editor->DeleteDetachedNodes( SCENE_NAME );

		success &= ( !editor->AttachChildNode( scene, root ) );
		success &= ( !root->GetChild( "child1" ) );
		success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child1" ) );
		success &= ( !editor->DeleteChildNode( SCENE_NAME, "/root/child1" ) );

		assert( success );
	});

	/*m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		success &= ( editor->GetScene( SCENE_NAME ) != nullptr );
		success &= ( editor->RemoveScene( SCENE_NAME ) );
		success &= ( !editor->GetScene( SCENE_NAME ) );

		assert( success );
	});


	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto root = scene->GetRootNode();
		
		bool success = true;

		success &= ( editor->DetachRootNode() );
		editor->DeleteDetachedNodes( SCENE_NAME );
		editor->AttachRootNode();
		success &= ( !editor->GetScene( SCENE_NAME )->GetRootNode() );
		success &= ( !editor->DeleteRootNode() );
		success &= ( !editor->DetachRootNode() );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		
		bool success = true;
		auto newRoot = TestSceneUtils::ColoredRectangle( "newRoot", 0.5f, 0.5f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK0_PATH );
		auto rootTransform = newRoot->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( rootTransform, 0, 0.0f, glm::vec3( -1.f, 0.5f, -1.f ) );

		editor->SetRootNode( newRoot );
		success &= ( editor->GetScene( SCENE_NAME )->GetRootNode() != nullptr );
		auto root = scene->GetRootNode();
		success &= ( root == std::static_pointer_cast< model::BasicNode >( m_project->GetModelSceneRoot()->GetChild( "root" ) ) );

		assert( success );
	});*/
	
	//m_testSteps.push_back([&] 
	//{
	//	auto editor = m_project->GetProjectEditor();
	//	auto root = scene->GetRootNode();
	//	
	//	bool success = true;

	//	success &= ( editor->DeleteRootNode() );
	//	success &= ( !editor->GetScene( SCENE_NAME )->GetRootNode() );
	//	success &= ( !editor->DeleteRootNode() );
	//	success &= ( !editor->DetachRootNode() );

	//	assert( success );
	//});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		
		bool success = true;

		auto newRoot = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "newRoot", 0.5f, 0.5f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK0_PATH );
		auto rootTransform = newRoot->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( rootTransform, 0.0f, glm::vec3( -1.f, 0.5f, -1.f ) );

		editor->AddChildNode( scene, nullptr, newRoot );
		success &= ( editor->GetScene( SCENE_NAME )->GetRootNode() != nullptr );
		auto root = scene->GetRootNode();
		success &= ( root == newRoot );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		
		bool success = true;

		auto newChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "newChild", 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		auto childTransform = newChild->GetPlugin( "transform" )->GetParameter( "simple_transform" );
		SetParameterTranslation( childTransform, 0.0f, glm::vec3( ( float )0.5, -0.5f, 0.2f ) );

		auto root = scene->GetRootNode();
		editor->AddChildNode( scene, root, newChild );
		success &= ( root->GetChild( "newChild" ) != nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();

		while( root->GetNumChildren() > 0 )
		{
			editor->DeleteChildNode( scene, root, root->GetChild( 0 ) );
		}
	});

	//m_testSteps.push_back([&] 
	//{
	//	auto editor = m_project->GetProjectEditor();
	//	bool success = true;

	//	success &= ( editor->GetScene( SCENE_NAME1 ) != nullptr );

	//	editor->RemoveScene( SCENE_NAME1 );

	//	success &= ( !editor->GetScene( SCENE_NAME1 ) );

	//	assert( success );
	//});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		TestSceneUtils::AddColoredRectangleScene( editor, SCENE_NAME1, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec3( -0.7f, 0.5f, -1.f ) );

		success &= ( editor->GetScene( SCENE_NAME ) != nullptr );
		success &= ( editor->GetScene( SCENE_NAME1 ) != nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		success &= ( editor->GetScene( SCENE_NAME1 ) != nullptr );

		editor->RemoveScene( SCENE_NAME1 );

		success &= ( !editor->GetScene( SCENE_NAME1 ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		TestSceneUtils::AddColoredRectangleScene( editor, SCENE_NAME1, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec3( -0.7f, 0.5f, -1.f ) );

		success &= ( editor->GetScene( SCENE_NAME ) != nullptr );
		success &= ( editor->GetScene( SCENE_NAME1 ) != nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		editor->RemoveAllScenes();

		success &= ( editor->GetScene( SCENE_NAME ) == nullptr );
		success &= ( editor->GetScene( SCENE_NAME1 ) == nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		TestSceneUtils::AddColoredRectangleScene( editor, SCENE_NAME, glm::vec4( 0.f, 0.f, 1.f, 1.f ), glm::vec3( -1.f, 0.5f, -1.f ) );

		success &= ( editor->GetScene( SCENE_NAME ) != nullptr );
		success &= ( editor->GetScene( SCENE_NAME1 ) == nullptr );

		assert( success );
	});

}

// ****************************
//
void					TestScene::InitTimelinesTest		()
{
	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetScene( SCENE_NAME );
        
		bool success = true;

        editor->AddTimeline( SCENE_NAME, TIMELINE_NAME, TimelineType::TT_DEFAULT );

        success &= ( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" ) ) != nullptr );
        success &= ( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) ) != nullptr );

        auto root = scene->GetRootNode();
        success &= ( root->GetName() == "root" );
        success &= ( root->GetPlugin( "transform" ) != nullptr );

        auto defaultTimeline = editor->GetSceneDefaultTimeline( scene );
        auto defaultTimelinePath = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" );
        for( auto param : root->GetPlugin( "transform" )->GetParameters() )
        {
            success &= ( param->GetTimeEvaluator() == defaultTimeline );
        }

        success &= ( root->GetPlugin( "transform" )->GetPluginParamValModel()->GetTimeEvaluator() == defaultTimeline );
        
        success &= ( !editor->DeleteTimeline( defaultTimelinePath ) );
        success &= ( editor->GetTimeline( defaultTimelinePath ) != nullptr );

        success &= ( !editor->ForceDeleteTimeline( defaultTimelinePath ) );
        success &= ( editor->GetTimeline( defaultTimelinePath ) != nullptr );

        success &= ( !editor->RenameTimeline( defaultTimelinePath, "test" ) );
        success &= ( editor->GetTimeline( defaultTimelinePath ) != nullptr );
        success &= ( editor->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "test" ) ) == nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		editor->RenameTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ), TIMELINE_NAME1 );

        success &= ( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" ) ) != nullptr );
		success &= ( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) ) == nullptr );
		success &= ( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME1 ) ) != nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

        success &= ( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" ) ) != nullptr );
		success &= editor->DeleteTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME1 ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
        auto timeline = editor->GetSceneDefaultTimeline( scene );

        auto anim = TestSceneUtils::AnimatedRectangle( editor->GetSceneDefaultTimeline( scene ), ANIM_NODE, 0.3f, 0.3f, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
		SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		auto root = scene->GetRootNode();
		editor->AddChildNode( scene, root, anim );
	});

    m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
        auto timeline = editor->GetSceneDefaultTimeline( scene );
        timeline->Play();
	});

    Wait( 5 );

    m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
        auto timeline = editor->GetSceneDefaultTimeline( scene );
		bool success = true;

        auto defaultTimelinePath = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" );

        success &= ( !editor->DeleteTimeline( defaultTimelinePath ) );
        success &= ( editor->GetTimeline( defaultTimelinePath ) != nullptr );

        success &= ( !editor->ForceDeleteTimeline( defaultTimelinePath ) );
        success &= ( editor->GetTimeline( defaultTimelinePath ) != nullptr );

        success &= ( !editor->RenameTimeline( defaultTimelinePath, "test" ) );
        success &= ( editor->GetTimeline( defaultTimelinePath ) != nullptr );
        success &= ( editor->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "test" ) ) == nullptr );
        
        timeline->Stop();

		assert( success );
	});

    m_testSteps.push_back([&] 
	{
        auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto child = scene->GetRootNode()->GetChild( ANIM_NODE );
		bool success = true;

		auto timeline = model::TimelineHelper::CreateDefaultTimeline( TIMELINE_NAME, 10000.0, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
        editor->AddTimeline( scene->GetTimeline(), timeline );

        success &= ( editor->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) ) != nullptr );

        for( auto param : child->GetParameters() )
        {
            param->SetTimeEvaluator( timeline );
        }
	});

    m_testSteps.push_back([&] 
	{
        auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
        auto timeline = editor->GetSceneDefaultTimeline( scene );
        timeline->SetTimeAndPlay( 0.0f );
	});

    Wait( 5 );

    m_testSteps.push_back([&] 
	{
        auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
        auto timeline = editor->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) );
        timeline->SetTimeAndPlay( 0.0f );
	});

    Wait( 5 );

    m_testSteps.push_back([&] 
	{
        auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
        auto timeline = editor->GetSceneDefaultTimeline( scene );
        auto timelinePath = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
        auto defaultTimelinePath = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" );
		bool success = true;
        
        success &= ( !editor->DeleteTimeline( timelinePath ) );
        success &= ( editor->GetTimeline( timelinePath ) != nullptr );

        success &= ( editor->ForceDeleteTimeline( timelinePath ) );
        success &= ( editor->GetTimeline( timelinePath ) == nullptr );
        success &= ( editor->GetTimeline( defaultTimelinePath ) != nullptr );

		auto child = scene->GetRootNode()->GetChild( ANIM_NODE );
        for( auto param : child->GetParameters() )
        {
            success &= ( param->GetTimeEvaluator() == timeline );
        }

        timeline->SetTimeAndPlay( 0.0f );

		assert( success );
	});

    m_testSteps.push_back([&] 
	{
        auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		bool success = true;

        success &= ( editor->DeleteChildNode( scene, root, root->GetChild( ANIM_NODE ) ) );
		
        assert( success );
    });

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto timeline = model::TimelineHelper::CreateDefaultTimeline( TIMELINE_NAME, 1.0, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
        editor->AddTimeline( scene->GetTimeline(), timeline );

		auto tex = TestSceneUtils::TexturedRectangle( timeline, TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		auto root = scene->GetRootNode();
		editor->AddChildNode( scene, root, tex );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		bool success = true;

		success &= ( !editor->DeleteTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) ) );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		bool success = true;

		auto oldTimeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
		auto newTimeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME1 );
		
		auto time = m_timeEvaluator->GetLocalTime();
		auto timeline = model::TimelineHelper::CreateDefaultTimeline( TIMELINE_NAME1, time+2.0f, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
		editor->AddTimeline( scene->GetTimeline(), timeline );
		timeline->Play();
		
		success &= ( !editor->DeleteTimeline( oldTimeline ) );
		
		editor->ForceDeleteTimeline( oldTimeline, newTimeline );

		success &= ( m_timelineManager->GetTimeline( oldTimeline ) == nullptr );
		success &= ( m_timelineManager->GetTimeline( newTimeline ) != nullptr );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto child = scene->GetRootNode()->GetChild( TEX_NODE );
		auto time = m_timeEvaluator->GetLocalTime();
		SetParameter( child->GetPlugin( "texture" )->GetParameter( "alpha" ), time, 1.f );
		SetParameter( child->GetPlugin( "texture" )->GetParameter( "alpha" ), time+2.f, 0.f );
	});
	
	Wait( 1 );

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		bool success = true;

		auto newTimeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
		auto oldTimeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME1 );
		
		auto timeline = model::TimelineHelper::CreateDefaultTimeline( TIMELINE_NAME, 2.0f, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_REPEAT );
		editor->AddTimeline( scene->GetTimeline(), timeline );
		timeline->Play();
		
		success &= (!editor->DeleteTimeline( oldTimeline ) );
		
		editor->ForceDeleteTimeline( oldTimeline, newTimeline );

		success &= ( m_timelineManager->GetTimeline( oldTimeline ) == nullptr );
		success &= ( m_timelineManager->GetTimeline( newTimeline ) != nullptr );

		auto child = scene->GetRootNode()->GetChild( TEX_NODE );
		SetParameter( child->GetPlugin( "texture" )->GetParameter( "alpha" ), 0.f, 1.f );
		SetParameter( child->GetPlugin( "texture" )->GetParameter( "alpha" ), 2.f, 0.f );
		success &= ( child->GetPlugin( "texture" )->GetParameter( "alpha" )->GetTimeEvaluator()->GetName() == TIMELINE_NAME );

		assert( success );
	});

	Wait( 2 );

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		bool success = true;

		auto timeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
		editor->SetTimelineDuration( timeline, 3.f );
		
		success &= ( m_timelineManager->GetTimeline( timeline ) != nullptr );
		success &= ( m_timelineManager->GetTimeline( timeline )->GetDuration() == 3.f );

		assert( success );
	});

	Wait( 3 );

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		bool success = true;

		auto timeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
		editor->SetTimelineWrapPostBehavior( timeline, TimelineWrapMethod::TWM_CLAMP );
		
		success &= ( m_timelineManager->GetTimeline( timeline ) != nullptr );
		success &= ( m_timelineManager->GetTimeline( timeline )->GetDuration() == 3.f );
		success &= ( m_timelineManager->GetTimeline( timeline )->GetWrapBehaviorPost() == TimelineWrapMethod::TWM_CLAMP );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		bool success = true;

		auto timeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
		editor->SetTimelineDuration( timeline, 0.5f );
		
		success &= ( m_timelineManager->GetTimeline( timeline ) != nullptr );
		success &= ( m_timelineManager->GetTimeline( timeline )->GetDuration() == 0.5f );
		success &= ( m_timelineManager->GetTimeline( timeline )->GetWrapBehaviorPost() == TimelineWrapMethod::TWM_CLAMP );

		assert( success );
	});

	m_testSteps.push_back([&] 
	{
        auto editor = m_project->GetProjectEditor();
        editor->AddTimeline( SCENE_NAME, TIMELINE_NAME, TimelineType::TT_DEFAULT );

        auto scene = editor->GetScene( SCENE_NAME );
        auto copy = editor->AddSceneCopy( SCENE_NAME );
        model::TimelineHelper::CopyTimelines( copy->GetTimeline(), scene->GetTimeline()->GetChildren() );        
        model::TimelineHelper::CopyTimelines( copy->GetTimeline(), scene->GetTimeline()->GetChildren() );        
    });
}

// ****************************
//
void					TestScene::InitAssetsTest		()
{
	auto add0 = [&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto tex = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "tex0", 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		auto root = scene->GetRootNode();
		editor->AddChildNode( scene, root, tex );
	};

	auto add1 = [&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto tex = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "tex1", 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );

		auto root = scene->GetRootNode();
		editor->AddChildNode( scene, root, tex );
	};

	m_testSteps.push_back( add0 );
	m_testSteps.push_back( add1 );

	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( "tex0" );

		auto desc = TextureAssetDesc::Create( ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() + "/f0.bmp", false );
        editor->LoadAsset( child->GetPlugin( "texture" ), desc );
	});

	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( "tex0" );

		auto desc = TextureAssetDesc::Create( ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() + "/f1.bmp", false );
        auto prevDesc = child->GetPlugin( "texture" )->GetPixelShaderChannel()->GetTexturesData()->GetTextures()[ 0 ];
		
        editor->LoadAsset( child->GetPlugin( "texture" ), desc );
	});

	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );
		editor->DeletePlugin( child, "texture" );
	});

    m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );

        auto anim = model::PluginsManager::DefaultInstance().CreatePlugin( "DEFAULT_ANIMATION", "animation", editor->GetSceneDefaultTimeline( scene ) );
        editor->AddPlugin( child, anim, 2 );
	});

    m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );
        auto desc = ProjectManager::GetInstance()->GetAssetDesc( "", "sequences", ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
		editor->LoadAsset( child->GetPlugin( "animation" ), desc );

        auto time = editor->GetScene( SCENE_NAME )->GetTimeline()->GetLocalTime();
        model::SetParameter( child->GetPlugin( "animation" )->GetParameter( "frameNum" ), time, 0.f );
        model::SetParameter( child->GetPlugin( "animation" )->GetParameter( "frameNum" ), time + 2.f, ( Float32 )TestSceneUtils::ANIM_NUM );
    });

    Wait( 2 );

	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );
		editor->DeletePlugin( child, "animation" );
	});

    m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetScene( SCENE_NAME );
		auto root = scene->GetRootNode();
		auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );

        auto color = model::PluginsManager::DefaultInstance().CreatePlugin( "DEFAULT_COLOR", "color", editor->GetSceneDefaultTimeline( scene ) );
        editor->AddPlugin( child, color, 2 );
        SetParameter( child->GetPlugin( "color" )->GetParameter( "color" ), 0.0, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );

        auto text = model::PluginsManager::DefaultInstance().CreatePlugin( "DEFAULT_TEXT", "text", editor->GetSceneDefaultTimeline( scene ) );
        editor->AddPlugin( child, text, 3 );
        SetParameter( child->GetPlugin( "text" )->GetParameter( "text" ), 0.0, std::wstring( L"tekst" ) );
	});

    m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );
    	auto desc = FontAssetDesc::Create( "fonts/couri.TTF", 30, 0, 0, true );
		editor->LoadAsset( child->GetPlugin( "text" ), desc );
    });
}

// ****************************
//
void					TestScene::InitCopyNodeTest	()
{
	m_testSteps.push_back( [&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        editor->AddTimeline( SCENE_NAME, TIMELINE_NAME, TimelineType::TT_DEFAULT );
        editor->AddTimeline( SCENE_NAME, TIMELINE_NAME1, TimelineType::TT_DEFAULT );

        auto timeline = editor->GetTimeEvaluator( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) );
        auto timeline1 = editor->GetTimeEvaluator( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME1 ) );

        auto child = TestSceneUtils::ColoredRectangle( timeline, "child0", 0.3f, 0.3f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
        auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
        SetParameterTranslation( childTransform, 0.0f, glm::vec3( 0.f, -0.5f, 0.f ) );
        editor->AddChildNode( scene, root, child );

        for( unsigned int i = 0; i < 2; ++i )
        {
            auto child = TestSceneUtils::ColoredRectangle( timeline1, "child0" + toString( i ), 0.2f, 0.2f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
            auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
            SetParameterTranslation( childTransform, 0.0f, glm::vec3( ( float )i * 0.25f, -0.5f, 0.f ) );
            editor->AddChildNode( scene, root->GetChild( "child0" ), child );
        }

        editor->AddScene( SCENE_NAME1 );
    });

	m_testSteps.push_back( [&] 
	{ 
		auto editor = m_project->GetProjectEditor();

        auto srcTimeline = editor->GetTimeEvaluator( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) );
        auto srcTimeline1 = editor->GetTimeEvaluator( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME1 ) );

		auto srcScene = editor->GetScene( SCENE_NAME );
		auto destScene = editor->GetScene( SCENE_NAME1 );
        bool success = true;

        success &= ( editor->AddNodeCopy( SCENE_NAME1, "", SCENE_NAME, srcScene->GetRootNode()->GetName() ) != nullptr );

		auto destTimeline = editor->GetTimeEvaluator( model::TimelineHelper::CombineTimelinePath( SCENE_NAME1, PrefixHelper::PrefixCopy( 0 ) + TIMELINE_NAME ) );
        auto destTimeline1 = editor->GetTimeEvaluator( model::TimelineHelper::CombineTimelinePath( SCENE_NAME1, PrefixHelper::PrefixCopy( 0 ) + TIMELINE_NAME1 ) );

		auto srcRoot = srcScene->GetRootNode();
        auto destRoot = destScene->GetRootNode();
		SetParameterTranslation( destRoot->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.5f, -1.f ) );

        success &= ( srcRoot->GetPlugin( "transform" )->GetParameter( "simple_transform" )->GetTimeEvaluator() == editor->GetSceneDefaultTimeline( srcScene ) );
        success &= ( destRoot->GetPlugin( "transform" )->GetParameter( "simple_transform" )->GetTimeEvaluator()->GetName() == "Copy_default" );

        auto destTimelines = destScene->GetTimeline()->GetChildren();

        std::string timelines0 [ 4 ] = { "default", "Copy_default", "Copy_timeline_0", "Copy_timeline_1" };
        std::vector< std::string > vtimelines0( timelines0, timelines0 + 4 );
        success &= ( destTimelines.size() == 4 );
        for( auto i = 0; i < destTimelines.size(); ++i )
        {
            success &= ( std::find( vtimelines0.begin(), vtimelines0.end(), destTimelines[ i ]->GetName() ) != vtimelines0.end() );
        }

        auto srcChild0Node = srcRoot->GetChild( "child0" );
        auto destChild0Node = destRoot->GetChild( "child0" );

        success &= ( srcChild0Node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->GetTimeEvaluator() == srcTimeline );
        success &= ( destChild0Node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->GetTimeEvaluator() == destTimeline );

        auto srcChild00Node = srcChild0Node->GetChild( "child00" );
        auto destChild00Node = destChild0Node->GetChild( "child00" );

        success &= ( srcChild00Node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->GetTimeEvaluator() == srcTimeline1 );
        success &= ( destChild00Node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->GetTimeEvaluator() == destTimeline1 );

        auto srcChild01Node = srcChild0Node->GetChild( "child01" );
        auto destChild01Node = destChild0Node->GetChild( "child01" );

        success &= ( srcChild01Node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->GetTimeEvaluator() == srcTimeline1 );
        success &= ( destChild01Node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->GetTimeEvaluator() == destTimeline1 );


        success &= ( editor->AddNodeCopy( SCENE_NAME1, "/root/child0/child00", SCENE_NAME, "/root/child0" ) != nullptr );

        destTimelines = destScene->GetTimeline()->GetChildren();

        std::string timelines1[ 6 ] = { "default", "Copy_default", "Copy_timeline_0", "Copy_timeline_1", "Copy1_timeline_0", "Copy1_timeline_1" };
        std::vector< std::string > vtimelines1( timelines1, timelines1 + 6 );
        success &= ( destTimelines.size() == 6 );
        for( auto i = 0; i < destTimelines.size(); ++i )
        {
            success &= ( std::find( vtimelines1.begin(), vtimelines1.end(), destTimelines[ i ]->GetName() ) != vtimelines1.end() );
        }

        
        success &= ( editor->AddNodeCopy( SCENE_NAME1, "/root/child0/child00/child0/child00", SCENE_NAME, "/root/child0" ) != nullptr );

        destTimelines = destScene->GetTimeline()->GetChildren();

        std::string timelines2[ 8 ] = { "default", "Copy_default", "Copy_timeline_0", "Copy_timeline_1", "Copy1_timeline_0", "Copy1_timeline_1", "Copy2_timeline_0", "Copy2_timeline_1" };
        std::vector< std::string > vtimelines2( timelines2, timelines2 + 8 );
        success &= ( destTimelines.size() == 8 );
        for( auto i = 0; i < destTimelines.size(); ++i )
        {
            success &= ( std::find( vtimelines2.begin(), vtimelines2.end(), destTimelines[ i ]->GetName() ) != vtimelines2.end() );
        }


        success &= ( editor->AddNodeCopy( SCENE_NAME1, "/root/child0/child00/child0/child00/child0/child00", SCENE_NAME, "/root/child0" ) != nullptr );

        destTimelines = destScene->GetTimeline()->GetChildren();

        std::string timelines3[ 10 ] = { "default", "Copy_default", "Copy_timeline_0", "Copy_timeline_1", "Copy1_timeline_0", "Copy1_timeline_1", "Copy2_timeline_0", "Copy2_timeline_1", "Copy3_timeline_0", "Copy3_timeline_1" };
        std::vector< std::string > vtimelines3( timelines3, timelines3 + 10 );
        success &= ( destTimelines.size() == 10 );
        for( auto i = 0; i < destTimelines.size(); ++i )
        {
            success &= ( std::find( vtimelines3.begin(), vtimelines3.end(), destTimelines[ i ]->GetName() ) != vtimelines3.end() );
        }


        success &= ( editor->AddNodeCopy( SCENE_NAME, "/root/child0/child00", SCENE_NAME1, "/root/child0/child00/child0" ) != nullptr );
        
        auto srcTimelines = srcScene->GetTimeline()->GetChildren();
        
        std::string timelines4[ 9 ] = { "default", "timeline_0", "timeline_1", "Copy_Copy1_timeline_0", "Copy_Copy1_timeline_1", "Copy_Copy2_timeline_0", "Copy_Copy2_timeline_1", "Copy_Copy3_timeline_0", "Copy_Copy3_timeline_1" };
        std::vector< std::string > vtimelines4( timelines4, timelines4 + 9 );
        success &= ( srcTimelines.size() == 9 );
        for( auto i = 0; i < srcTimelines.size(); ++i )
        {
            success &= ( std::find( vtimelines4.begin(), vtimelines4.end(), srcTimelines[ i ]->GetName() ) != vtimelines4.end() );
        }


        success &= ( editor->AddNodeCopy( SCENE_NAME, "/root/child0/child00", SCENE_NAME1, "/root/child0/child00/child0" ) != nullptr );

        srcTimelines = srcScene->GetTimeline()->GetChildren();
        
        std::string timelines5[ 15 ] = { "default", "timeline_0", "timeline_1", "Copy_Copy1_timeline_0", "Copy_Copy1_timeline_1", "Copy_Copy2_timeline_0", "Copy_Copy2_timeline_1", "Copy_Copy3_timeline_0", "Copy_Copy3_timeline_1", "Copy1_Copy1_timeline_0", "Copy1_Copy1_timeline_1", "Copy1_Copy2_timeline_0", "Copy1_Copy2_timeline_1", "Copy1_Copy3_timeline_0", "Copy1_Copy3_timeline_1" };
        std::vector< std::string > vtimelines5( timelines5, timelines5 + 15 );
        success &= ( srcTimelines.size() == 15 );
        for( auto i = 0; i < srcTimelines.size(); ++i )
        {
            success &= ( std::find( vtimelines5.begin(), vtimelines5.end(), srcTimelines[ i ]->GetName() ) != vtimelines5.end() );
        }


        BVXMLSerializer ser;
        srcScene->Serialize( ser );
        destScene->Serialize( ser );
        ser.Save( "test.xml" );

        assert( success );
	});

    m_testSteps.push_back( [&] 
	{
		auto editor = m_project->GetProjectEditor();

        auto node = editor->GetNode( SCENE_NAME, "/root/child0/child00/child0/child00" );

        auto timeEval = node->GetPlugin( "solid color" )->GetParameter( "color" )->GetTimeEvaluator();
        std::static_pointer_cast< model::ITimeline >( timeEval )->Play();
    });

    Wait( 5 );

    m_testSteps.push_back( [&] 
	{
		auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetScene( SCENE_NAME1 );

        editor->AddPluginCopy( SCENE_NAME1, "/root/child0", 2, SCENE_NAME, "/root/child0/child00/child0/child00", "solid color" );

        auto srcTimelinePath = model::TimelineManager::GetInstance()->GetTimelinePath( editor->GetNode( SCENE_NAME, "/root/child0/child00/child0/child00" )->GetPlugin( "solid color" )->GetParameter( "color" )->GetTimeEvaluator() );
        auto destTimelinePath = model::TimelineManager::GetInstance()->GetTimelinePath( editor->GetNode( SCENE_NAME1, "/root/child0" )->GetPlugin( "solid color" )->GetParameter( "color" )->GetTimeEvaluator() );

        bool success = true;

        success &= ( srcTimelinePath == model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "Copy_Copy1_timeline_1" ) );
        success &= ( destTimelinePath == model::TimelineHelper::CombineTimelinePath( SCENE_NAME1, "Copy_Copy_Copy1_timeline_1" ) );

        BVXMLSerializer ser;
        scene->Serialize( ser );
        ser.Save( "test.xml" );

        assert( success );
    });
}

// ****************************
//
void					TestScene::InitBasicColorPluginTest	()
{
	auto add = [&] 
	{ 
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto col = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), COL_NODE, 0.3f, 0.3f, glm::vec4( 0.f, 1.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( col->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.5f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		editor->AddChildNode( scene, root, col );

		auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( scene, col, lChild );
		editor->AddChildNode( scene, col, rChild );
		success &= ( col->GetNumChildren() == 2 );

		assert( success ); { success; }
	};

	m_testSteps.push_back( add );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, COL_NODE, "solid color", 2 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, COL_NODE, "solid color", 2 ); } );

	m_testSteps.push_back( [&]{ CopyPlugin( 2, "solid color", COL_NODE, "solid color" ); } );
	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		
		model::SetParameter( root->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
	});
	m_testSteps.push_back( [&]{ RestoreRoot( 2, "solid color" ); } );

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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto col = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), COL_NODE, 0.3f, 0.3f, glm::vec4( 0.f, 1.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( col->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.5f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		auto child = root->GetChild( COL_NODE );

		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );

		success &= editor->DeleteChildNode( scene, root, child );
		editor->AddChildNode( scene, root, col );
		editor->AddChildNode( scene, col, lChild );
		editor->AddChildNode( scene, col, rChild );
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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto tex = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TEX_NODE ) );
		editor->AddChildNode( scene, root, tex );
	};

	auto add1 = [&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto tex = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TEX_NODE ) );
		editor->AddChildNode( scene, root, tex );
		
		auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( scene, tex, lChild );
		editor->AddChildNode( scene, tex, rChild );

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
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( TEX_NODE );
		
		//auto time = m_timeEvaluator->GetLocalTime();
		model::SetParameter( child->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "filteringMode" ), 0.f, static_cast< Int32 >( TextureFilteringMode::TFM_POINT ) );
		//model::SetParameter( child->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "filteringMode" ), time + TimeType( 1.0f ), static_cast< Int32 >( TextureFilteringMode::TFM_LINEAR ) );
	});

	m_testSteps.push_back( [&]{ CopyPlugin( 2, "solid color", TEX_NODE, "texture" ); } );
	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( TEX_NODE );

		bool success = true;

		auto fm0 = QueryTypedValue< ValueIntPtr >( root->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetValue( "filteringMode" ) )->GetValue();
		auto fm1 = QueryTypedValue< ValueIntPtr >( child->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetValue( "filteringMode" ) )->GetValue();

		success &= ( fm0 == ( int )TextureFilteringMode::TFM_POINT );
		success &= ( fm0 == fm1 );

		assert( success );
	} );
	m_testSteps.push_back( [&]{} );
	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		
		auto desc = TextureAssetDesc::Create( ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() + "/f0.bmp", false );
		editor->LoadAsset( root->GetPlugin( "texture" ), desc );
	});
	m_testSteps.push_back( [&]{ RestoreRoot( 2, "texture" ); } );

	m_testSteps.push_back( [&]{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( TEX_NODE );

		auto desc = TextureAssetDesc::Create( ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() + "/f0.bmp", false );
		editor->LoadAsset( child->GetPlugin( "texture" ), desc );
	});

	//m_testSteps.push_back( [&]{
	//	auto editor = m_project->GetProjectEditor();
	//	auto root = scene->GetRootNode();
	//	auto child = root->GetChild( TEX_NODE );

	//	auto desc = TextureAssetDesc::Create( ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() + "/f1.bmp", false );
	//	editor->LoadAsset( child->GetPlugin( "texture" ), desc );
	//});
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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto tex = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		auto child = root->GetChild( TEX_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( scene, root, child );
		editor->AddChildNode( scene, root, tex );
		editor->AddChildNode( scene, tex, lChild );
		editor->AddChildNode( scene, tex, rChild );
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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto anim = TestSceneUtils::AnimatedRectangle( editor->GetSceneDefaultTimeline( scene ), ANIM_NODE, 0.3f, 0.3f, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
		SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
		
		auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( ANIM_NODE ) );
		editor->AddChildNode( scene, root, anim );
	};

	auto add1 = [&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto anim = TestSceneUtils::AnimatedRectangle( editor->GetSceneDefaultTimeline( scene ), ANIM_NODE, 0.3f, 0.3f, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str(), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
		
		bool success = true;

		auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( ANIM_NODE ) );
		editor->AddChildNode( scene, root, anim );
		
		auto lChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( scene, anim, lChild );
		editor->AddChildNode( scene, anim, rChild );

		success &= ( anim->GetNumChildren() == 2 );
		
		assert( success );
	};

	m_testSteps.push_back( add0 );
	m_testSteps.push_back( add1 );

	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, ANIM_NODE, "animation", 2 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "animation", 2, ANIM_NODE, "solid color", 2 ); } );

	m_testSteps.push_back( [&]{ CopyPlugin( 2, "solid color", ANIM_NODE, "animation" ); } );
	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		
		auto desc =  ProjectManager::GetInstance()->GetAssetDesc( "", "sequences", ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
		editor->LoadAsset( root->GetPlugin( "animation" ), desc );
	});
	m_testSteps.push_back( [&]{ RestoreRoot( 2, "animation" ); } );

	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( ANIM_NODE );
		model::SetParameter( child->GetPlugin( "animation" )->GetResourceStateModel( "Tex0" )->GetParameter( "filteringMode" ), 0.0, static_cast< Int32 >( TextureFilteringMode::TFM_POINT ) );
	});

	m_testSteps.push_back( [&]{} );

	m_testSteps.push_back( [&]{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( ANIM_NODE );

		auto desc =  ProjectManager::GetInstance()->GetAssetDesc( "", "sequences", ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
		editor->LoadAsset( root->GetPlugin( "animation" ), desc );

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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto anim = TestSceneUtils::AnimatedRectangle( editor->GetSceneDefaultTimeline( scene ), ANIM_NODE, 0.3f, 0.3f, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str(), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
		
		bool success = true;

		auto root = scene->GetRootNode();
		auto child = root->GetChild( ANIM_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( scene, root, child );
		editor->AddChildNode( scene, root, anim );
		editor->AddChildNode( scene, anim, lChild );
		editor->AddChildNode( scene, anim, rChild );
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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto grad = TestSceneUtils::GradientRectangle( editor->GetSceneDefaultTimeline( scene ), GRAD_NODE, 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( grad->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		editor->AddChildNode( scene, root, grad );
		
		auto lChild = TestSceneUtils::AnimatedRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( scene, grad, lChild );
		editor->AddChildNode( scene, grad, rChild );

		success &= ( grad->GetNumChildren() == 2 );

		assert( success );
	};

	m_testSteps.push_back( add );
	m_testSteps.push_back( [&]
	{
		auto child = std::static_pointer_cast< model::BasicNode >( std::static_pointer_cast< model::BasicNode >( m_project->GetModelSceneRoot()->GetChild( "root" ) )->GetChild( GRAD_NODE ) );
		SetParameter( child->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), TimeType( 0.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
	});

	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GRAD_NODE, "linear_gradient", 2 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "linear_gradient", 2, GRAD_NODE, "solid color", 2 ); } );

	m_testSteps.push_back( [&]
	{
		auto time = m_timeEvaluator->GetLocalTime();
		auto child = std::static_pointer_cast< model::BasicNode >( std::static_pointer_cast< model::BasicNode >( m_project->GetModelSceneRoot()->GetChild( "root" ) )->GetChild( GRAD_NODE ) );
		SetParameter( child->GetPlugin( "linear_gradient" )->GetParameter( "alpha" ), time, 1.f );
		SetParameter( child->GetPlugin( "linear_gradient" )->GetParameter( "alpha" ), time+1.0f, 0.f );
		SetParameter( child->GetPlugin( "linear_gradient" )->GetParameter( "alpha" ), time+1.1f, 1.f );
	});

	m_testSteps.push_back( [&] {} );
	m_testSteps.push_back( [&] {} );

	m_testSteps.push_back( [&]{ CopyPlugin( 2, "solid color", GRAD_NODE, "linear_gradient" ); } );
	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		
		SetParameter( root->GetPlugin( "linear_gradient" )->GetParameter( "color1" ), TimeType( 0.f ), glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
	});
	m_testSteps.push_back( [&]{ RestoreRoot( 2, "linear_gradient" ); } );
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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto grad = TestSceneUtils::GradientRectangle( editor->GetSceneDefaultTimeline( scene ), GRAD_NODE, 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( grad->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.5f, -0.5f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		auto child = root->GetChild( GRAD_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( scene, root, child );
		editor->AddChildNode( scene, root, grad );
		editor->AddChildNode( scene, grad, lChild );
		editor->AddChildNode( scene, grad, rChild );
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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto txt = TestSceneUtils::ColoredText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 40 );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );
		
		auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TXT_NODE ) );
		editor->AddChildNode( scene, root, txt );
	};

	auto add1 = [&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto txt = TestSceneUtils::ColoredText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TXT_NODE ) );
		editor->AddChildNode( scene, root, txt );
		
		auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( scene, txt, lChild );
		editor->AddChildNode( scene, txt, rChild );

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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto txt = TestSceneUtils::ColoredText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		auto child = root->GetChild( TXT_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( scene, root, child );
		editor->AddChildNode( scene, root, txt );
		editor->AddChildNode( scene, txt, lChild );
		editor->AddChildNode( scene, txt, rChild );
		success &= ( txt->GetNumChildren() == 2 );

		assert( success );
	};


//---------------


	m_testSteps.push_back( add0 );
	m_testSteps.push_back( add1 );
	
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TXT_NODE, "solid color", 1 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TXT_NODE, "solid color", 1 ); } );

	m_testSteps.push_back( [&]{ CopyPlugin( 2, "rectangle", TXT_NODE, "text" ); } );
	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		
		auto desc = FontAssetDesc::Create( "fonts/couri.TTF", 30, 0, 0, true );
		editor->LoadAsset( root->GetPlugin( "text" ), desc );
	});
	m_testSteps.push_back( [&]{ RestoreRoot( 1, "text" ); } );

	m_testSteps.push_back( [&]
	{ 
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( TXT_NODE );
        SetParameter( child->GetPlugin( "text" )->GetParameter("text"), 0.0, std::wstring( L"test0\n4321" ) );
	});

	m_testSteps.push_back( [&]
	{ 
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( TXT_NODE );

		auto desc = FontAssetDesc::Create( "fonts/couri.TTF", 30, 0, 0, true );
		editor->LoadAsset( root->GetPlugin( "text" ), desc );
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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto txt = TestSceneUtils::GradientText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 0.f, 1.f, 0.f, 1.f ), 40 );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TXT_NODE ) );
		editor->AddChildNode( scene, root, txt );
	};

	auto add1 = [&] 
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto txt = TestSceneUtils::GradientText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 0.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TXT_NODE ) );
		editor->AddChildNode( scene, root, txt );
		
		auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( scene, txt, lChild );
		editor->AddChildNode( scene, txt, rChild );

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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto txt = TestSceneUtils::GradientText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 0.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		auto lChild = root->GetChild( TXT_NODE )->GetChild( "lChild" );
		auto rChild = root->GetChild( TXT_NODE )->GetChild( "rChild" );
		success &= editor->DeleteChildNode( scene, root, root->GetChild( TXT_NODE ) );
		editor->AddChildNode( scene, root, txt );
		editor->AddChildNode( scene, txt, lChild );
		editor->AddChildNode( scene, txt, rChild );
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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto tmr = TestSceneUtils::ColoredTimer( editor->GetSceneDefaultTimeline( scene ), TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		editor->DeleteChildNode( scene, root, root->GetChild( TMR_NODE ) );
		editor->AddChildNode( scene, root, tmr );
		
		auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( scene, tmr, lChild );
		editor->AddChildNode( scene, tmr, rChild );

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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto tmr = TestSceneUtils::ColoredTimer( editor->GetSceneDefaultTimeline( scene ), TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		auto child = root->GetChild( TMR_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( scene, root, child );
		editor->AddChildNode( scene, root, tmr );
		editor->AddChildNode( scene, tmr, lChild );
		editor->AddChildNode( scene, tmr, rChild );
		success &= ( tmr->GetNumChildren() == 2 );

		assert( success );
	};


//---------------

	m_testSteps.push_back( add );
    Wait( 1 );
	
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TMR_NODE, "solid color", 1 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TMR_NODE, "solid color", 1 ); } );

	m_testSteps.push_back( [&]
	{ 
		CopyPlugin( 2, "rectangle", TMR_NODE, "timer" ); 

		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		model::SetTimeTimerPlugin( root->GetPlugin( "timer" ), 12333.0f );
		model::StartTimerPlugin( root->GetPlugin( "timer" ) );
	} );
    Wait( 2 );
	m_testSteps.push_back( [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		
		auto desc = FontAssetDesc::Create( "Assets/Fonts/couri.TTF", 40, 0, 0, true );
		editor->LoadAsset( root->GetPlugin( "timer" ), desc );
	});
    Wait( 2 );
	m_testSteps.push_back( [&]{ RestoreRoot( 1, "timer" ); } );


	m_testSteps.push_back( [&]
	{ 
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( TMR_NODE );
		model::SetTimeTimerPlugin( child->GetPlugin( "timer" ), 15.0f );
		model::StartTimerPlugin( child->GetPlugin( "timer" ) );
	});
    Wait( 1 );

	m_testSteps.push_back( [&]
	{ 
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( TMR_NODE );

		auto desc = FontAssetDesc::Create( "Assets/Fonts/couri.TTF", 40, 0, 0, true );
		editor->LoadAsset( root->GetPlugin( "timer" ), desc );
	});
    Wait( 1 );

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
        Wait( 1 );
		InitOrderTest( test );
	}
}

// ****************************
//
void					TestScene::InitGradientTimerTest			()
{
	auto add0 = [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto tmr = TestSceneUtils::GradientTimer( editor->GetSceneDefaultTimeline( scene ), TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), 40 );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		auto root = scene->GetRootNode();
		editor->DeleteChildNode( scene, root, root->GetChild( TMR_NODE ) );
		editor->AddChildNode( scene, root, tmr );
	};

	auto add1 = [&]
	{
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );

		auto tmr = TestSceneUtils::GradientTimer( editor->GetSceneDefaultTimeline( scene ), TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		editor->DeleteChildNode( scene, root, root->GetChild( TMR_NODE ) );
		editor->AddChildNode( scene, root, tmr );
		
		auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
		SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
		auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
		SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
		editor->AddChildNode( scene, tmr, lChild );
		editor->AddChildNode( scene, tmr, rChild );

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
		auto editor = m_project->GetProjectEditor();
		auto scene = editor->GetScene( SCENE_NAME );
		auto tmr = TestSceneUtils::GradientTimer( editor->GetSceneDefaultTimeline( scene ), TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
		SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

		bool success = true;

		auto root = scene->GetRootNode();
		auto child = root->GetChild( TMR_NODE );
		auto lChild = child->GetChild( "lChild" );
		auto rChild = child->GetChild( "rChild" );
		success &= editor->DeleteChildNode( scene, root, child );
		editor->AddChildNode( scene, root, tmr );
		editor->AddChildNode( scene, tmr, lChild );
		editor->AddChildNode( scene, tmr, rChild );
		success &= ( tmr->GetNumChildren() == 2 );

		assert( success );
	};


//---------------

	m_testSteps.push_back( add0 );
	m_testSteps.push_back( add1 );
    Wait( 1 );
	
	m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, TMR_NODE, "linear_gradient", 1 ); } );
	m_testSteps.push_back( [&]{ SwapPlugins( "linear_gradient", 2, TMR_NODE, "solid color", 2 ); } );

	m_testSteps.push_back( [&]
	{ 
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( TMR_NODE );
		model::SetTimeTimerPlugin( child->GetPlugin( "timer" ), 15.0f );
		model::StartTimerPlugin( child->GetPlugin( "timer" ) );
	});
    Wait( 1 );

	m_testSteps.push_back( [&]
	{ 
		auto editor = m_project->GetProjectEditor();
		auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
		auto child = root->GetChild( TMR_NODE );

		auto desc = FontAssetDesc::Create( "Assets/Fonts/couri.TTF", 40, 0, 0, true );
		editor->LoadAsset( root->GetPlugin( "timer" ), desc );
	});
    Wait( 1 );

	for( auto & test : tests )
	{
		m_testSteps.push_back( recoverScene );
        Wait( 1 );
		InitOrderTest( test );
	}
	m_testSteps.push_back( recoverScene );
}

// ****************************
//
void					TestScene::InitColoredGeometryTest		()
{
	for( unsigned int i = 0; i < TestSceneUtils::GEOM_PLUGINS_NUM - 1; ++i )
	{
		auto plugin = TestSceneUtils::PluginsArr[ i ];
		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );

			auto geom = TestSceneUtils::ColoredGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
			
			auto time = m_timeEvaluator->GetLocalTime(); {time;}
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = scene->GetRootNode();
            editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
			editor->AddChildNode( scene, root, geom );
		});
        Wait( 1 );

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );

			auto geom = TestSceneUtils::ColoredGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
			
			auto time = m_timeEvaluator->GetLocalTime();
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
			editor->AddChildNode( scene, root, geom );
		});
		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GEOM_NODE, "solid color", 2 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GEOM_NODE, "solid color", 2 ); } );
				
	
		auto pluginName = TestSceneUtils::PluginsNameArr[ i ];
		m_testSteps.push_back( [ pluginName, this ]{ CopyPlugin( 1, "rectangle", GEOM_NODE, pluginName ); } );
		m_testSteps.push_back( [ pluginName, this ]{ RestoreRoot( 1, pluginName ); } );
	

		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );

		std::string test0[] = { "alpha_mask", pluginName };
		std::string test1[] = { "solid color", pluginName };

		std::vector < OrderTestCase > tests;
		tests.push_back( OrderTestCase( GEOM_NODE, "AmGeom", std::vector< std::string >( test0, test0 + 2 ) ) );
		tests.push_back( OrderTestCase( GEOM_NODE, "CGeom", std::vector< std::string >( test1, test1 + 2 ) ) );

		for( auto & test : tests )
		{
			m_testSteps.push_back( [ plugin, this ]
			{
				auto editor = m_project->GetProjectEditor();
				auto scene = editor->GetScene( SCENE_NAME );

				auto geom = TestSceneUtils::ColoredGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
				
				auto time = m_timeEvaluator->GetLocalTime();
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

				auto root = scene->GetRootNode();
				editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
				editor->AddChildNode( scene, root, geom );
			});
			InitOrderTest( test );
		}
	}
}

// ****************************
//
void					TestScene::InitTexturedGeometryTest		()
{
	for( unsigned int i = 0; i < TestSceneUtils::GEOM_PLUGINS_NUM - 1; ++i )
	{
		auto plugin = TestSceneUtils::PluginsArr[ i ];

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );

			auto geom = TestSceneUtils::TexturedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, TestSceneUtils::TEXTURE_PATH );
			
			auto time = m_timeEvaluator->GetLocalTime();
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
			editor->AddChildNode( scene, root, geom );
		});
        Wait( 1 );

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );

			auto geom = TestSceneUtils::TexturedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
			
			auto time = m_timeEvaluator->GetLocalTime();
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
			editor->AddChildNode( scene, root, geom );
		});
		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, GEOM_NODE, "texture", 2 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "texture", 2, GEOM_NODE, "solid color", 2 ); } );
				
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, GEOM_NODE, "alpha_mask", 3 ); } );

		m_testSteps.push_back( [&]
		{
			auto editor = m_project->GetProjectEditor();
			auto geom = editor->GetScene( SCENE_NAME )->GetRootNode()->GetChild( GEOM_NODE );

			auto desc = TextureAssetDesc::Create( ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() + "/f0.bmp", false );
			editor->LoadAsset( geom->GetPlugin( "texture" ), desc );
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
				auto editor = m_project->GetProjectEditor();
				auto scene = editor->GetScene( SCENE_NAME );

				auto geom = TestSceneUtils::TexturedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );

				auto time = m_timeEvaluator->GetLocalTime();
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

				auto root = scene->GetRootNode();
				editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
				editor->AddChildNode( scene, root, geom );
			});
			InitOrderTest( test );
		}
	}
}

// ****************************
//
void					TestScene::InitAnimatedGeometryTest		()
{
	for( unsigned int i = 0; i < TestSceneUtils::GEOM_PLUGINS_NUM - 1; ++i )
	{
		auto plugin = TestSceneUtils::PluginsArr[ i ];

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );

			auto geom = TestSceneUtils::AnimatedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
			
			auto time = m_timeEvaluator->GetLocalTime();
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
			editor->AddChildNode( scene, root, geom );
		});
        Wait( 1 );

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );

			auto geom = TestSceneUtils::AnimatedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str(), TestSceneUtils::ALPHA_MASK_PATH );
			
			auto time = m_timeEvaluator->GetLocalTime();
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
			editor->AddChildNode( scene, root, geom );
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
				auto editor = m_project->GetProjectEditor();
				auto scene = editor->GetScene( SCENE_NAME );

				auto geom = TestSceneUtils::AnimatedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str(), TestSceneUtils::ALPHA_MASK_PATH );

				auto time = m_timeEvaluator->GetLocalTime();
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

				auto root = scene->GetRootNode();
			    editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
				editor->AddChildNode( scene, root, geom );
			});
			InitOrderTest( test );
		}
	}
}

// ****************************
//
void					TestScene::InitGradientGeometryTest		()
{
	for( unsigned int i = 0; i < TestSceneUtils::GEOM_PLUGINS_NUM - 1; ++i )
	{
		auto plugin = TestSceneUtils::PluginsArr[ i ];

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );

			auto time = m_timeEvaluator->GetLocalTime();
			auto geom = TestSceneUtils::GradientGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
			
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
			editor->AddChildNode( scene, root, geom );
		});
        Wait( 1 );

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );

			auto time = m_timeEvaluator->GetLocalTime();
			auto geom = TestSceneUtils::GradientGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
			
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
			SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
			SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
			editor->AddChildNode( scene, root, geom );
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
				auto editor = m_project->GetProjectEditor();
				auto scene = editor->GetScene( SCENE_NAME );

				auto geom = TestSceneUtils::GradientGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
			
				auto time = m_timeEvaluator->GetLocalTime();
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.f, 0.f, 0.f ), 0.f );
				SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
				SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.f, 1.f, 1.f ), 360.f );

				auto root = scene->GetRootNode();
			    editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
				editor->AddChildNode( scene, root, geom );
			});
			InitOrderTest( test );
		}
	}
}

// ****************************
//
void					TestScene::InitVideoStreamDecoderTest	()
{
	for( unsigned int i = 0; i < TestSceneUtils::GEOM_PLUGINS_NUM; ++i )
	{
		auto plugin = TestSceneUtils::PluginsArr[ i ];

		auto add0 = [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );
			auto vsd = TestSceneUtils::VideoStreamDecoder( editor->GetSceneDefaultTimeline( scene ), VSD_NODE, plugin, TestSceneUtils::VIDEO_PATH0 );
			SetParameterTranslation( vsd->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.0f, -0.1f, -2.f ) );

			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( VSD_NODE ) );
			editor->AddChildNode( scene, root, vsd );

			model::DefaultVideoStreamDecoderPlugin::Start( vsd->GetPlugin( "video_stream_decoder" ) );
		};

		auto add1 = [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );
			auto vsd = TestSceneUtils::VideoStreamDecoder( editor->GetSceneDefaultTimeline( scene ), VSD_NODE, plugin, TestSceneUtils::VIDEO_PATH0, TestSceneUtils::ALPHA_MASK_PATH );
			SetParameterTranslation( vsd->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.0f, -0.1f, -2.f ) );

			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( VSD_NODE ) );
			editor->AddChildNode( scene, root, vsd );

			model::DefaultVideoStreamDecoderPlugin::Start( vsd->GetPlugin( "video_stream_decoder" ) );
		};

		m_testSteps.push_back( add0 );
        Wait( 1 );
		m_testSteps.push_back( add1 );
	
		m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, VSD_NODE, "video_stream_decoder", 2 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "video_stream_decoder", 2, VSD_NODE, "solid color", 2 ); } );

		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, VSD_NODE, "alpha_mask", 3 ); } );
		m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, VSD_NODE, "alpha_mask", 3 ); } );
	
		m_testSteps.push_back( [&]{ 
			CopyPlugin( 2, "solid color", VSD_NODE, "video_stream_decoder" );

			auto editor = m_project->GetProjectEditor();
			auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
			model::DefaultVideoStreamDecoderPlugin::Start( root->GetPlugin( "video_stream_decoder" ) );
		} );
        Wait( 1 );
		m_testSteps.push_back( [&]
		{
			auto editor = m_project->GetProjectEditor();
			auto root = editor->GetScene( SCENE_NAME )->GetRootNode();

			auto desc = VideoStreamAssetDesc::Create( TestSceneUtils::VIDEO_PATH1, TextureFormat::F_A8R8G8B8 );
			editor->LoadAsset( root->GetPlugin( "video_stream_decoder" ), desc );

			model::DefaultVideoStreamDecoderPlugin::Start( root->GetPlugin( "video_stream_decoder" ) );
		});
		m_testSteps.push_back( [&]{ RestoreRoot( 2, "video_stream_decoder" ); } );

		m_testSteps.push_back( [&]{
			auto editor = m_project->GetProjectEditor();
			auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
			auto child = root->GetChild( VSD_NODE );

			auto desc = VideoStreamAssetDesc::Create( TestSceneUtils::VIDEO_PATH1, TextureFormat::F_A8R8G8B8 );
			editor->LoadAsset( root->GetPlugin( "video_stream_decoder" ), desc );

			model::DefaultVideoStreamDecoderPlugin::Start( child->GetPlugin( "video_stream_decoder" ) );
		});

		auto pluginName = TestSceneUtils::PluginsNameArr[ i ];
		std::string test0[] = { "alpha_mask", pluginName };
		std::string test1[] = { "video_stream_decoder", pluginName };

		std::vector < OrderTestCase > tests;
		tests.push_back( OrderTestCase( VSD_NODE, "AmGeom", std::vector< std::string >( test0, test0 + 2 ) ) );
		tests.push_back( OrderTestCase( VSD_NODE, "VsGeom", std::vector< std::string >( test1, test1 + 2 ) ) );

		for( auto & test : tests )
		{
			m_testSteps.push_back( [ plugin, this ]
			{
				auto editor = m_project->GetProjectEditor();
				auto scene = editor->GetScene( SCENE_NAME );
				auto vsd = TestSceneUtils::VideoStreamDecoder( editor->GetSceneDefaultTimeline( scene ), VSD_NODE, plugin, TestSceneUtils::VIDEO_PATH0, TestSceneUtils::ALPHA_MASK_PATH );
				SetParameterTranslation( vsd->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.0f, -0.1f, -2.f ) );

				auto root = scene->GetRootNode();
			    editor->DeleteChildNode( scene, root, root->GetChild( VSD_NODE ) );
				editor->AddChildNode( scene, root, vsd );

				model::DefaultVideoStreamDecoderPlugin::Start( vsd->GetPlugin( "video_stream_decoder" ) );
			});
			InitOrderTest( test );
		}

		m_testSteps.push_back( [ plugin, this ]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );
			auto vsd = TestSceneUtils::VideoStreamDecoder( editor->GetSceneDefaultTimeline( scene ), VSD_NODE, plugin, TestSceneUtils::VIDEO_PATH0, TestSceneUtils::ALPHA_MASK_PATH );
			SetParameterTranslation( vsd->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.0f, -0.1f, -2.f ) );

			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( VSD_NODE ) );
			editor->AddChildNode( scene, root, vsd );

			model::DefaultVideoStreamDecoderPlugin::Start( vsd->GetPlugin( "video_stream_decoder" ) );
		});

		m_testSteps.push_back( [&]{
			auto editor = m_project->GetProjectEditor();
			auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
			auto child = root->GetChild( VSD_NODE );

			auto desc = VideoStreamAssetDesc::Create( TestSceneUtils::VIDEO_PATH1, TextureFormat::F_A8R8G8B8 );
			editor->LoadAsset( child->GetPlugin( "video_stream_decoder" ), desc );

			model::DefaultVideoStreamDecoderPlugin::Start( child->GetPlugin( "video_stream_decoder" ) );
		});

        Wait( 3 );
		
		m_testSteps.push_back( [&]
		{
			auto editor = m_project->GetProjectEditor();
			auto scene = editor->GetScene( SCENE_NAME );
			auto root = scene->GetRootNode();
			editor->DeleteChildNode( scene, root, root->GetChild( VSD_NODE ) );
		} );
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

			auto editor = m_project->GetProjectEditor();
			auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
			auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( node ) );
			
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
	auto editor = m_project->GetProjectEditor();

	auto root = editor->GetScene( SCENE_NAME )->GetRootNode();
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
void						TestScene::CopyPlugin			( UInt32 rootIdx, const std::string & rootPlugin, const std::string & childName, const std::string & childPlugin )
{
	auto editor = m_project->GetProjectEditor();
	auto scene = editor->GetScene( SCENE_NAME );
	auto root = scene->GetRootNode();
	auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( childName ) );

	m_copiedPlugin = root->GetPlugin( rootPlugin );
	editor->DeletePlugin( root, rootPlugin );
	
	editor->AddPluginCopy( scene, root, rootIdx, scene, child, childPlugin );
}

// ****************************
//
void						TestScene::RestoreRoot			( UInt32 rootIdx, const std::string & childPlugin )
{
	auto editor = m_project->GetProjectEditor();
	auto root = editor->GetScene( SCENE_NAME )->GetRootNode();

	editor->DeletePlugin( root, childPlugin );
	editor->AddPlugin( root, m_copiedPlugin, rootIdx );
}

// ****************************
//
void                    TestScene::Wait                     ( UInt32 sec )
{
    for( UInt32 i = 0; i < sec; ++i )
    {
        m_testSteps.push_back( []{} );
    }
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

} // bv