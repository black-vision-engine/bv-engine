#include "TestScene.h"

#include <cassert>

#include "TestSceneUtils.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/NodeEffects/ModelNodeEffectFactory.h"

#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Tools/PrefixHelper.h"

#include "Assets/Assets.h"

#include "System/Path.h"
#include "IO/FileIO.h"
#include "IO/DirIO.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/DefaultAVDecoderPlugin.h"
#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"

#include "ProjectManager.h"

#include "Serialization/BV/XML/BVXMLSerializer.h"


#include "UnitTest++.h"


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

    Path::Copy( "Assets/Fonts/SupportedChars.txt", "DefaultPMDir/fonts/SupportedChars.txt" );
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
    InitTestModelSceneEditor();

    InitTimelinesTest();

    //InitAssetsTest();

    //InitCopyNodeTest();

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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        auto timeline = editor->GetSceneDefaultTimeline( scene );
        for( unsigned int i = 0; i < 3; ++i )
        {
            auto child = TestSceneUtils::ColoredRectangle( timeline, "child" + toString( i ), 0.3f, 0.3f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
            auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
            SetParameterTranslation( childTransform, 0.0f, glm::vec3( ( float )0.5*i, -0.5f, 0.f ) );
            editor->AddChildNode( scene, root, child );
        }
        CHECK( root->GetNumChildren() == 3 );

        for( unsigned int i = 0; i < root->GetNumChildren(); ++i )
        {
            auto child = TestSceneUtils::ColoredRectangle( timeline, "child0" + toString( i ), 0.2f, 0.2f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
            auto childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
            SetParameterTranslation( childTransform, 0.0f, glm::vec3( ( float )i, -0.5f, 0.f ) );
            editor->AddChildNode( scene, root->GetChild( "child0" ), child );
        }
        CHECK( root->GetChild( "child0" )->GetNumChildren() == 3 );
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

        CHECK( success );
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

        CHECK( node0->GetName() == "child01" );
        CHECK( node1 == nullptr );
        CHECK( node2->GetName() == "child0" );
        CHECK( node3->GetName() == "child2" );
        CHECK( node4 == nullptr );
        CHECK( node5->GetName() == "child01" );
        CHECK( node6->GetName() == "child02" );
        CHECK( node6 == node7 );
        CHECK( node0 == node8 );
        CHECK( node9 == nullptr );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;    { success; }

        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        auto child = root->GetChild( 0 );

        auto effect = SerializationHelper::String2T< NodeEffectType >( "alpha mask", NodeEffectType::NET_DEFAULT );
        auto newEffect = model::ModelNodeEffectFactory::CreateModelNodeEffect( effect, "alpha mask", m_timeEvaluator );

        editor->SetNodeEffect( child, newEffect );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;    { success; }

        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        auto child = root->GetChild( 0 );

        auto nodeEffect = editor->GetNodeEffect( child );
        REQUIRE CHECK( nodeEffect != nullptr );

        success = model::SetParameter( nodeEffect->GetParameter( "alpha" ), 0.f, 0.5f );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

        editor->AddScene( EMPTY_SCENE );

        auto scene = editor->GetModelScene( EMPTY_SCENE );
        auto root = scene->GetRootNode();

        success &= ( scene != nullptr );
        success &= ( root != nullptr );
        success &= ( root->GetName() == "root" );
        success &= ( root->GetNumPlugins() == 1 );
        success &= ( root->GetPlugin( "transform" ) != nullptr );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

        success = editor->DetachScene( EMPTY_SCENE );

        CHECK( success );
    });

    
    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

         success = editor->AttachScene( EMPTY_SCENE, 0 );

         CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( EMPTY_SCENE );
        bool success = true;

        auto root = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "newRoot", 0.4f, 0.4f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
        auto rootTransform = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );
        SetParameterTranslation( rootTransform, 0.0f, glm::vec3( ( float )0.0f, 0.0f, -1.0f ) );

        editor->AddChildNode( scene, nullptr, root );

        success &= ( editor->GetModelScene( EMPTY_SCENE ) != nullptr );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        editor->MoveScene( EMPTY_SCENE, 0 );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( EMPTY_SCENE );
        bool success = true;

        auto root = model::BasicNode::Create( "root", nullptr );
        success &= editor->AddChildNode( scene, nullptr, root );

        CHECK( success );
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

        success &= ( editor->GetModelScene( SCENE_NAME ) != nullptr );
        success &= ( editor->GetModelScene( "Copy_" + SCENE_NAME ) != nullptr );

        CHECK( success );
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

        success &= ( editor->GetModelScene( SCENE_NAME ) != nullptr );
        success &= ( editor->GetModelScene( "Copy_" + SCENE_NAME ) == nullptr );
        success &= ( editor->GetModelScene( "Copy_Copy_" + SCENE_NAME ) != nullptr );

        editor->RenameScene( "Copy_Copy_" + SCENE_NAME, "Copy1_" + SCENE_NAME );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

        editor->RenameScene( "Copy1_" + SCENE_NAME, SCENE_NAME1 );
        success &= ( editor->GetModelScene( SCENE_NAME1 ) != nullptr );
        success &= ( editor->GetModelScene( "Copy1_" + SCENE_NAME ) == nullptr );
        //editor->AddScene( model::SceneModel::CreateEmptyScene( SCENE_NAME1 ) );

        auto scene = editor->GetModelScene( SCENE_NAME1 );
        auto copied = editor->AddNodeCopy( scene, scene->GetRootNode(), scene, scene->GetRootNode() );
        SetParameterTranslation( copied->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.5f, -1.f ) );

        success &= ( editor->GetModelScene( SCENE_NAME ) != nullptr );
        success &= ( editor->GetModelScene( SCENE_NAME1 ) != nullptr );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

        auto scene = editor->GetModelScene( SCENE_NAME1 );
        auto root = scene->GetRootNode();
        SetParameterTranslation( root->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.3f, 0.5f, -1.f ) );

        for( UInt32 i = 0; i < root->GetNumChildren(); ++i )
        {
            model::SetParameter( root->GetChild( i )->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
        }

        success &= ( editor->GetModelScene( SCENE_NAME ) != nullptr );
        success &= ( editor->GetModelScene( SCENE_NAME1 ) != nullptr );

        CHECK( success );
    });
    
    m_testSteps.push_back([&] 
    { 
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        bool success = true;
        success &= ( root->GetName() == "root" );
        success &= ( root == std::static_pointer_cast< model::BasicNode >( m_project->GetModelSceneRoot()->GetChild( "root" ) ) );
        
        CHECK( success );
        
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();

        bool success = true;
        
        success &= ( root->GetChild( 0 )->GetName() != "newChild" );

        editor->MoveNode( SCENE_NAME, "root", 0, SCENE_NAME, "root/newChild" );

        success &= ( root->GetChild( 0 )->GetName() == "newChild" );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    { 
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        bool success = true;

        auto child0 = root->GetChild( "child0" );
        success &= ( child0->GetName() == "child0" );

        success &= ( child0->GetChild( "child00" ) != nullptr );
        success &= ( editor->DetachChildNode( SCENE_NAME, "/root/child0/child00" ) );
        success &= ( !child0->GetChild( "child00" ) );
        success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child0/child00" ) );

        CHECK( success );
    });
    
    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        bool success = true;
        auto child0 = root->GetChild( "child0" );
        success &= ( child0->GetName() == "child0" );

        success &= ( child0->GetChild( "child02" ) != nullptr  );
        success &= ( editor->DetachChildNode( SCENE_NAME, "/root/child0/child02" ) );
        success &= ( !child0->GetChild( "child02" ) );
        success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child0/child02" ) );
    
        CHECK( success );
    });
    
    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        bool success = true;
        auto child0 = root->GetChild( "child0" );
        success &= ( child0->GetName() == "child0" );

        success &= ( child0->GetChild( "child01" ) != nullptr  );
        success &= ( editor->DetachChildNode( SCENE_NAME, "/root/child0/child01" ) );
        success &= ( !child0->GetChild( "child01" ) );
        success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child0/child01" ) );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        bool success = true;

        success &= ( editor->DetachChildNode( SCENE_NAME, "/root/child0" ) );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        bool success = true;

        success &= ( editor->AttachChildNode( scene, root ) );
        success &= ( root->GetChild( "child0" ) != nullptr );
        success &= ( !editor->AttachChildNode( scene, root ) );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        bool success = true;

        success &= ( root->GetChild( "child0" ) != nullptr );
        success &= ( editor->DeleteChildNode( SCENE_NAME, "#0/#3" ) );

        //success &= ( editor->DeleteChildNode( scene, root, "child0" ) );
        success &= ( !root->GetChild( "child0" ) );
        success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child0" ) );
        success &= ( !editor->AttachChildNode( scene, root ) );
        success &= ( !editor->DeleteChildNode( SCENE_NAME, "/root/child0" ) );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        bool success = true;

        success &= ( root->GetChild( "child1" ) != nullptr );
        success &= ( editor->DeleteChildNode( SCENE_NAME, "/root/child1" ) );
        success &= ( !root->GetChild( "child1" ) );
        success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child1" ) );
        success &= ( !editor->AttachChildNode( scene, root ) );
        success &= ( !editor->DeleteChildNode( SCENE_NAME, "/root/child1" ) );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        
        bool success = true;

        editor->DeleteDetachedNodes( SCENE_NAME );
        editor->DeleteDetachedNodes( SCENE_NAME );

        success &= ( !editor->AttachChildNode( scene, root ) );
        success &= ( !root->GetChild( "child1" ) );
        success &= ( !editor->DetachChildNode( SCENE_NAME, "/root/child1" ) );
        success &= ( !editor->DeleteChildNode( SCENE_NAME, "/root/child1" ) );

        CHECK( success );
    });

    /*m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

        success &= ( editor->GetModelScene( SCENE_NAME ) != nullptr );
        success &= ( editor->RemoveScene( SCENE_NAME ) );
        success &= ( !editor->GetModelScene( SCENE_NAME ) );

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
        success &= ( !editor->GetModelScene( SCENE_NAME )->GetRootNode() );
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
        success &= ( editor->GetModelScene( SCENE_NAME )->GetRootNode() != nullptr );
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
    //	success &= ( !editor->GetModelScene( SCENE_NAME )->GetRootNode() );
    //	success &= ( !editor->DeleteRootNode() );
    //	success &= ( !editor->DetachRootNode() );

    //	assert( success );
    //});

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        
        bool success = true;

        auto newRoot = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "newRoot", 0.5f, 0.5f, glm::vec4( 0.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK0_PATH );
        auto rootTransform = newRoot->GetPlugin( "transform" )->GetParameter( "simple_transform" );
        SetParameterTranslation( rootTransform, 0.0f, glm::vec3( -1.f, 0.5f, -1.f ) );

        editor->AddChildNode( scene, nullptr, newRoot );
        success &= ( editor->GetModelScene( SCENE_NAME )->GetRootNode() != nullptr );
        auto root = scene->GetRootNode();
        success &= ( root == newRoot );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        
        bool success = true;

        auto newChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "newChild", 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
        auto childTransform = newChild->GetPlugin( "transform" )->GetParameter( "simple_transform" );
        SetParameterTranslation( childTransform, 0.0f, glm::vec3( ( float )0.5, -0.5f, 0.2f ) );

        auto root = scene->GetRootNode();
        editor->AddChildNode( scene, root, newChild );
        success &= ( root->GetChild( "newChild" ) != nullptr );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
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

    //	success &= ( editor->GetModelScene( SCENE_NAME1 ) != nullptr );

    //	editor->RemoveScene( SCENE_NAME1 );

    //	success &= ( !editor->GetModelScene( SCENE_NAME1 ) );

    //	assert( success );
    //});

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

        TestSceneUtils::AddColoredRectangleScene( editor, SCENE_NAME1, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec3( -0.7f, 0.5f, -1.f ) );

        success &= ( editor->GetModelScene( SCENE_NAME ) != nullptr );
        success &= ( editor->GetModelScene( SCENE_NAME1 ) != nullptr );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

        success &= ( editor->GetModelScene( SCENE_NAME1 ) != nullptr );

        editor->RemoveScene( SCENE_NAME1 );

        success &= ( !editor->GetModelScene( SCENE_NAME1 ) );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

        TestSceneUtils::AddColoredRectangleScene( editor, SCENE_NAME1, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec3( -0.7f, 0.5f, -1.f ) );

        success &= ( editor->GetModelScene( SCENE_NAME ) != nullptr );
        success &= ( editor->GetModelScene( SCENE_NAME1 ) != nullptr );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

        editor->RemoveAllScenes();

        success &= ( editor->GetModelScene( SCENE_NAME ) == nullptr );
        success &= ( editor->GetModelScene( SCENE_NAME1 ) == nullptr );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        bool success = true;

        TestSceneUtils::AddColoredRectangleScene( editor, SCENE_NAME, glm::vec4( 0.f, 0.f, 1.f, 1.f ), glm::vec3( -1.f, 0.5f, -1.f ) );

        success &= ( editor->GetModelScene( SCENE_NAME ) != nullptr );
        success &= ( editor->GetModelScene( SCENE_NAME1 ) == nullptr );

        CHECK( success );
    });

}

// ****************************
//
void					TestScene::InitTimelinesTest		()
{
    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

        editor->AddTimeline( SCENE_NAME, TIMELINE_NAME, TimelineType::TT_DEFAULT );

        CHECK( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" ) ) != nullptr );
        CHECK( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) ) != nullptr );

        auto root = scene->GetRootNode();
        CHECK( root->GetName() == "root" );
        CHECK( root->GetPlugin( "transform" ) != nullptr );

        auto defaultTimeline = editor->GetSceneDefaultTimeline( scene );
        auto defaultTimelinePath = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" );
        for( auto param : root->GetPlugin( "transform" )->GetParameters() )
        {
            CHECK( param->GetTimeEvaluator() == defaultTimeline );
        }

        CHECK( root->GetPlugin( "transform" )->GetPluginParamValModel()->GetTimeEvaluator() == defaultTimeline );
        
        CHECK( !editor->DeleteTimeline( defaultTimelinePath ) );
        CHECK( editor->GetTimeline( defaultTimelinePath ) != nullptr );

        CHECK( !editor->ForceDeleteTimeline( defaultTimelinePath ) );
        CHECK( editor->GetTimeline( defaultTimelinePath ) != nullptr );

        CHECK( !editor->RenameTimeline( defaultTimelinePath, "test" ) );
        CHECK( editor->GetTimeline( defaultTimelinePath ) != nullptr );
        CHECK( editor->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "test" ) ) == nullptr );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();

        editor->RenameTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ), TIMELINE_NAME1 );

        CHECK( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" ) ) != nullptr );
        CHECK( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) ) == nullptr );
        CHECK( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME1 ) ) != nullptr );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();

        CHECK( m_timelineManager->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" ) ) != nullptr );
        bool success = editor->DeleteTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME1 ) );

        CHECK( success );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto timeline = editor->GetSceneDefaultTimeline( scene );

        auto anim = TestSceneUtils::AnimatedRectangle( editor->GetSceneDefaultTimeline( scene ), ANIM_NODE, 0.3f, 0.3f, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
        SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->AddChildNode( scene, root, anim );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto timeline = editor->GetSceneDefaultTimeline( scene );
        timeline->Play();
    });

    Wait( 5 );

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto timeline = editor->GetSceneDefaultTimeline( scene );

        auto defaultTimelinePath = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" );

        CHECK( !editor->DeleteTimeline( defaultTimelinePath ) );
        CHECK( editor->GetTimeline( defaultTimelinePath ) != nullptr );

        CHECK( !editor->ForceDeleteTimeline( defaultTimelinePath ) );
        CHECK( editor->GetTimeline( defaultTimelinePath ) != nullptr );

        CHECK( !editor->RenameTimeline( defaultTimelinePath, "test" ) );
        CHECK( editor->GetTimeline( defaultTimelinePath ) != nullptr );
        CHECK( editor->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "test" ) ) == nullptr );
        
        timeline->Stop();
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto child = scene->GetRootNode()->GetChild( ANIM_NODE );

        auto timeline = model::TimelineHelper::CreateDefaultTimeline( TIMELINE_NAME, 10000.0, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
        editor->AddTimeline( scene->GetTimeline(), timeline );

        CHECK( editor->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) ) != nullptr );

        for( auto param : child->GetParameters() )
        {
            param->SetTimeEvaluator( timeline );
        }
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto timeline = editor->GetSceneDefaultTimeline( scene );
        timeline->SetTimeAndPlay( 0.0f );
    });

    Wait( 5 );

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto timeline = editor->GetTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) );
        timeline->SetTimeAndPlay( 0.0f );
    });

    Wait( 5 );

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto timeline = editor->GetSceneDefaultTimeline( scene );
        auto timelinePath = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
        auto defaultTimelinePath = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "default" );
        
        CHECK( !editor->DeleteTimeline( timelinePath ) );
        CHECK( editor->GetTimeline( timelinePath ) != nullptr );

        CHECK( editor->ForceDeleteTimeline( timelinePath ) );
        CHECK( editor->GetTimeline( timelinePath ) == nullptr );
        CHECK( editor->GetTimeline( defaultTimelinePath ) != nullptr );

        auto child = scene->GetRootNode()->GetChild( ANIM_NODE );
        for( auto param : child->GetParameters() )
        {
            CHECK( param->GetTimeEvaluator() == timeline );
        }

        timeline->SetTimeAndPlay( 0.0f );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();

        CHECK( editor->DeleteChildNode( scene, root, root->GetChild( ANIM_NODE ) ) );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

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

        CHECK( !editor->DeleteTimeline( model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME ) ) );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto oldTimeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
        auto newTimeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME1 );
        
        auto time = m_timeEvaluator->GetLocalTime();
        auto timeline = model::TimelineHelper::CreateDefaultTimeline( TIMELINE_NAME1, time+2.0f, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
        editor->AddTimeline( scene->GetTimeline(), timeline );
        timeline->Play();
        
        CHECK( !editor->DeleteTimeline( oldTimeline ) );
        
        editor->ForceDeleteTimeline( oldTimeline, newTimeline );

        CHECK( m_timelineManager->GetTimeline( oldTimeline ) == nullptr );
        CHECK( m_timelineManager->GetTimeline( newTimeline ) != nullptr );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto child = scene->GetRootNode()->GetChild( TEX_NODE );
        auto time = m_timeEvaluator->GetLocalTime();
        SetParameter( child->GetPlugin( "texture" )->GetParameter( "alpha" ), time, 1.f );
        SetParameter( child->GetPlugin( "texture" )->GetParameter( "alpha" ), time+2.f, 0.f );
    });
    
    Wait( 1 );

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto newTimeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
        auto oldTimeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME1 );
        
        auto timeline = model::TimelineHelper::CreateDefaultTimeline( TIMELINE_NAME, 2.0f, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_REPEAT );
        editor->AddTimeline( scene->GetTimeline(), timeline );
        timeline->Play();
        
        auto child = scene->GetRootNode()->GetChild( TEX_NODE );
        auto model = std::static_pointer_cast< model::DefaultPluginParamValModel >( child->GetPlugin( "texture" )->GetPluginParamValModel() );
        
        CHECK( model->GetTimeEvaluator()->GetName() == TIMELINE_NAME1 );
        CHECK( model->GetTimeEvaluator() == m_timelineManager->GetTimeline( oldTimeline ) );


        CHECK(!editor->DeleteTimeline( oldTimeline ) );
        
        CHECK( editor->ForceDeleteTimeline( oldTimeline, newTimeline ) );

        CHECK( m_timelineManager->GetTimeline( oldTimeline ) == nullptr );
        CHECK( m_timelineManager->GetTimeline( newTimeline ) != nullptr );

        SetParameter( child->GetPlugin( "texture" )->GetParameter( "alpha" ), 0.f, 1.f );
        SetParameter( child->GetPlugin( "texture" )->GetParameter( "alpha" ), 2.f, 0.f );
        CHECK( child->GetPlugin( "texture" )->GetParameter( "alpha" )->GetTimeEvaluator()->GetName() == TIMELINE_NAME );
        
        // Note: ForceDeleteTimeline should change default timeline in ParamValModel too.
        CHECK( model->GetTimeEvaluator()->GetName() == TIMELINE_NAME );
        CHECK( model->GetTimeEvaluator() == m_timelineManager->GetTimeline( newTimeline ) );
    });

    Wait( 2 );

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto timeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
        editor->SetTimelineDuration( timeline, 3.f );
        
        CHECK( m_timelineManager->GetTimeline( timeline ) != nullptr );
        CHECK( m_timelineManager->GetTimeline( timeline )->GetDuration() == 3.f );
    });

    Wait( 3 );

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto timeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
        editor->SetTimelineWrapPostBehavior( timeline, TimelineWrapMethod::TWM_CLAMP );
        
        CHECK( m_timelineManager->GetTimeline( timeline ) != nullptr );
        CHECK( m_timelineManager->GetTimeline( timeline )->GetDuration() == 3.f );
        CHECK( m_timelineManager->GetTimeline( timeline )->GetWrapBehaviorPost() == TimelineWrapMethod::TWM_CLAMP );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto timeline = model::TimelineHelper::CombineTimelinePath( SCENE_NAME, TIMELINE_NAME );
        editor->SetTimelineDuration( timeline, 0.5f );
        
        CHECK( m_timelineManager->GetTimeline( timeline ) != nullptr );
        CHECK( m_timelineManager->GetTimeline( timeline )->GetDuration() == 0.5f );
        CHECK( m_timelineManager->GetTimeline( timeline )->GetWrapBehaviorPost() == TimelineWrapMethod::TWM_CLAMP );
    });

    m_testSteps.push_back([&] 
    {
        auto editor = m_project->GetProjectEditor();
        editor->AddTimeline( SCENE_NAME, TIMELINE_NAME, TimelineType::TT_DEFAULT );

        auto scene = editor->GetModelScene( SCENE_NAME );
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto tex = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "tex0", 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH );
        SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        CHECK( editor->AddChildNode( scene, root, tex ) );
    };

    auto add1 = [&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto tex = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "tex1", 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH );
        SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        CHECK( editor->AddChildNode( scene, root, tex ) );
    };

    m_testSteps.push_back( add0 );
    m_testSteps.push_back( add1 );

    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( "tex0" );

        auto desc = TextureAssetDesc::Create( "sequences/" + ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() + "/f0.bmp", false );
        CHECK( editor->LoadAsset( child->GetPlugin( "texture" ), desc ) );
    });

    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( "tex0" );

        auto desc = TextureAssetDesc::Create( "sequences/" + ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() + "/f1.bmp", false );
        auto prevDesc = child->GetPlugin( "texture" )->GetPixelShaderChannel()->GetTexturesData()->GetTextures()[ 0 ];
        
        CHECK( editor->LoadAsset( child->GetPlugin( "texture" ), desc ) );
    });

    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );
        editor->DeletePlugin( child, "texture" );
    });

    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );

        auto anim = model::PluginsManager::DefaultInstance().CreatePlugin( "DEFAULT_ANIMATION", "animation", editor->GetSceneDefaultTimeline( scene ) );
        
        UInt32 idx = 2;
        CHECK( editor->AddPlugin( child, anim, idx ) );
    });

    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );
        auto desc = ProjectManager::GetInstance()->GetAssetDesc( "", "sequences", ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
        CHECK( editor->LoadAsset( child->GetPlugin( "animation" ), desc ) );

        auto time = editor->GetModelScene( SCENE_NAME )->GetTimeline()->GetLocalTime();
        model::SetParameter( child->GetPlugin( "animation" )->GetParameter( "frameNum" ), time, 0.f );
        model::SetParameter( child->GetPlugin( "animation" )->GetParameter( "frameNum" ), time + 2.f, ( Float32 )TestSceneUtils::ANIM_NUM );
    });

    Wait( 2 );

    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );
        editor->DeletePlugin( child, "animation" );
    });

    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto root = scene->GetRootNode();
        auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );

        UInt32 idx = 2;

        auto color = model::PluginsManager::DefaultInstance().CreatePlugin( "DEFAULT_COLOR", "color", editor->GetSceneDefaultTimeline( scene ) );
        CHECK( editor->AddPlugin( child, color, idx ) );
        SetParameter( child->GetPlugin( "color" )->GetParameter( "color" ), 0.0, glm::vec4( 1.f, 0.f, 0.f, 1.f ) );

        idx = 3;

        auto text = model::PluginsManager::DefaultInstance().CreatePlugin( "DEFAULT_TEXT", "text", editor->GetSceneDefaultTimeline( scene ) );
        CHECK( editor->AddPlugin( child, text, idx ) );
        SetParameter( child->GetPlugin( "text" )->GetParameter( "text" ), 0.0, std::wstring( L"tekst" ) );
    });

    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( "tex0" ) );
        auto desc = FontAssetDesc::Create( "Assets/Fonts/couri.TTF", 30, 0, 0, 0, true );
        CHECK( editor->LoadAsset( child->GetPlugin( "text" ), desc ) );
    });
}

// ****************************
//
void					TestScene::InitCopyNodeTest	()
{
    m_testSteps.push_back( [&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
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
            child = TestSceneUtils::ColoredRectangle( timeline1, "child0" + toString( i ), 0.2f, 0.2f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
            childTransform = child->GetPlugin( "transform" )->GetParameter( "simple_transform" );
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

        auto srcScene = editor->GetModelScene( SCENE_NAME );
        auto destScene = editor->GetModelScene( SCENE_NAME1 );
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

        CHECK( success );
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
        auto scene = editor->GetModelScene( SCENE_NAME1 );

        editor->AddPluginCopy( SCENE_NAME1, "/root/child0", 2, SCENE_NAME, "/root/child0/child00/child0/child00", "solid color" );

        auto srcTimelinePath = model::TimelineManager::GetInstance()->GetTimelinePath( editor->GetNode( SCENE_NAME, "/root/child0/child00/child0/child00" )->GetPlugin( "solid color" )->GetParameter( "color" )->GetTimeEvaluator() );
        auto destTimelinePath = model::TimelineManager::GetInstance()->GetTimelinePath( editor->GetNode( SCENE_NAME1, "/root/child0" )->GetPlugin( "solid color" )->GetParameter( "color" )->GetTimeEvaluator() );

        bool success = true;

        success &= ( srcTimelinePath == model::TimelineHelper::CombineTimelinePath( SCENE_NAME, "Copy_Copy1_timeline_1" ) );
        success &= ( destTimelinePath == model::TimelineHelper::CombineTimelinePath( SCENE_NAME1, "Copy_Copy_Copy1_timeline_1" ) );

        BVXMLSerializer ser;
        scene->Serialize( ser );
        ser.Save( "test.xml" );

        CHECK( success );
    });
}

} // bv