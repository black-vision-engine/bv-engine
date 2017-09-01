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

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/DefaultAVDecoderPlugin.h"
#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"

#include "ProjectManager.h"

#include "Serialization/BV/XML/BVXMLSerializer.h"

#include "UnitTest++.h"


namespace bv {


// ****************************
//
void					TestScene::InitBasicColorPluginTest	()
{
    auto add = [&] 
    { 
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto col = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), COL_NODE, 0.3f, 0.3f, glm::vec4( 0.f, 1.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( col->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->AddChildNode( scene, root, col );

        auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
        SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
        auto rChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
        SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
        editor->AddChildNode( scene, col, lChild );
        editor->AddChildNode( scene, col, rChild );
        CHECK( col->GetNumChildren() == 2 );
    };

    m_testSteps.push_back( add );
    m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, COL_NODE, "solid color", 2 ); } );
    m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, COL_NODE, "solid color", 2 ); } );

    m_testSteps.push_back( [&]{ CopyPlugin( 2, "solid color", COL_NODE, "solid color" ); } );
    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto col = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), COL_NODE, 0.3f, 0.3f, glm::vec4( 0.f, 1.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( col->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        auto child = root->GetChild( COL_NODE );

        auto lChild = child->GetChild( "lChild" );
        auto rChild = child->GetChild( "rChild" );

        CHECK( editor->DeleteChildNode( scene, root, child ) );
        editor->AddChildNode( scene, root, col );
        editor->AddChildNode( scene, col, lChild );
        editor->AddChildNode( scene, col, rChild );
        CHECK( col->GetNumChildren() == 2 );
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto tex = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH );
        SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TEX_NODE ) );
        editor->AddChildNode( scene, root, tex );
    };

    auto add1 = [&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto tex = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TEX_NODE ) );
        editor->AddChildNode( scene, root, tex );
        
        auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
        SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
        auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
        SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
        editor->AddChildNode( scene, tex, lChild );
        editor->AddChildNode( scene, tex, rChild );

        CHECK( tex->GetNumChildren() == 2 );
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
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( TEX_NODE );
        
        //auto time = m_timeEvaluator->GetLocalTime();
        model::SetParameter( child->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "filteringMode" ), 0.f, static_cast< Int32 >( TextureFilteringMode::TFM_POINT ) );
        //model::SetParameter( child->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetParameter( "filteringMode" ), time + TimeType( 1.0f ), static_cast< Int32 >( TextureFilteringMode::TFM_LINEAR ) );
    });

    m_testSteps.push_back( [&]{ CopyPlugin( 2, "solid color", TEX_NODE, "texture" ); } );
    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( TEX_NODE );

        auto fm0 = QueryTypedValue< ValueIntPtr >( root->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetValue( "filteringMode" ) )->GetValue();
        auto fm1 = QueryTypedValue< ValueIntPtr >( child->GetPlugin( "texture" )->GetResourceStateModel( "Tex0" )->GetValue( "filteringMode" ) )->GetValue();

        CHECK( fm0 == ( int )TextureFilteringMode::TFM_POINT );
        CHECK( fm0 == fm1 );
    } );
    m_testSteps.push_back( [&]{} );
    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        
        auto desc = TextureAssetDesc::Create( ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() + "/f0.bmp", false );
        editor->LoadAsset( root->GetPlugin( "texture" ), desc );
    });
    m_testSteps.push_back( [&]{ RestoreRoot( 2, "texture" ); } );

    m_testSteps.push_back( [&]{
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto tex = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), TEX_NODE, 0.3f, 0.3f, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( tex->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.5f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        auto child = root->GetChild( TEX_NODE );
        auto lChild = child->GetChild( "lChild" );
        auto rChild = child->GetChild( "rChild" );
        CHECK( editor->DeleteChildNode( scene, root, child ) );
        editor->AddChildNode( scene, root, tex );
        editor->AddChildNode( scene, tex, lChild );
        editor->AddChildNode( scene, tex, rChild );
        CHECK( tex->GetNumChildren() == 2 );
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
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto anim = TestSceneUtils::AnimatedRectangle( editor->GetSceneDefaultTimeline( scene ), ANIM_NODE, 0.3f, 0.3f, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
        SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
        
        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( ANIM_NODE ) );
        editor->AddChildNode( scene, root, anim );
    };

    auto add1 = [&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto anim = TestSceneUtils::AnimatedRectangle( editor->GetSceneDefaultTimeline( scene ), ANIM_NODE, 0.3f, 0.3f, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str(), TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
        
        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( ANIM_NODE ) );
        editor->AddChildNode( scene, root, anim );
        
        auto lChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
        SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
        auto rChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
        SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
        editor->AddChildNode( scene, anim, lChild );
        editor->AddChildNode( scene, anim, rChild );

        CHECK( anim->GetNumChildren() == 2 );
    };

    m_testSteps.push_back( add0 );
    m_testSteps.push_back( add1 );

    m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, ANIM_NODE, "animation", 2 ); } );
    m_testSteps.push_back( [&]{ SwapPlugins( "animation", 2, ANIM_NODE, "solid color", 2 ); } );

    m_testSteps.push_back( [&]{ CopyPlugin( 2, "solid color", ANIM_NODE, "animation" ); } );
    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        
        auto desc =  ProjectManager::GetInstance()->GetAssetDesc( "", "sequences", ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
        editor->LoadAsset( root->GetPlugin( "animation" ), desc );
    });
    m_testSteps.push_back( [&]{ RestoreRoot( 2, "animation" ); } );

    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( ANIM_NODE );
        model::SetParameter( child->GetPlugin( "animation" )->GetResourceStateModel( "Tex0" )->GetParameter( "filteringMode" ), 0.0, static_cast< Int32 >( TextureFilteringMode::TFM_POINT ) );
    });

    m_testSteps.push_back( [&]{} );

    m_testSteps.push_back( [&]{
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto anim = TestSceneUtils::AnimatedRectangle( editor->GetSceneDefaultTimeline( scene ), ANIM_NODE, 0.3f, 0.3f, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str(), TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( anim->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.f, -0.5f, 0.f ) );
        
        auto root = scene->GetRootNode();
        auto child = root->GetChild( ANIM_NODE );
        auto lChild = child->GetChild( "lChild" );
        auto rChild = child->GetChild( "rChild" );
        CHECK( editor->DeleteChildNode( scene, root, child ) );
        editor->AddChildNode( scene, root, anim );
        editor->AddChildNode( scene, anim, lChild );
        editor->AddChildNode( scene, anim, rChild );
        CHECK( anim->GetNumChildren() == 2 );
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
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto grad = TestSceneUtils::GradientRectangle( editor->GetSceneDefaultTimeline( scene ), GRAD_NODE, 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( grad->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.5f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->AddChildNode( scene, root, grad );
        
        auto lChild = TestSceneUtils::AnimatedRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
        SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
        auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
        SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
        editor->AddChildNode( scene, grad, lChild );
        editor->AddChildNode( scene, grad, rChild );

        CHECK( grad->GetNumChildren() == 2 );
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
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto grad = TestSceneUtils::GradientRectangle( editor->GetSceneDefaultTimeline( scene ), GRAD_NODE, 0.3f, 0.3f, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( grad->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.5f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        auto child = root->GetChild( GRAD_NODE );
        auto lChild = child->GetChild( "lChild" );
        auto rChild = child->GetChild( "rChild" );
        CHECK( editor->DeleteChildNode( scene, root, child ) );
        editor->AddChildNode( scene, root, grad );
        editor->AddChildNode( scene, grad, lChild );
        editor->AddChildNode( scene, grad, rChild );
        CHECK( grad->GetNumChildren() == 2 );
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
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto txt = TestSceneUtils::ColoredText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 40 );
        SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );
        
        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TXT_NODE ) );
        editor->AddChildNode( scene, root, txt );
    };

    auto add1 = [&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto txt = TestSceneUtils::ColoredText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TXT_NODE ) );
        editor->AddChildNode( scene, root, txt );
        
        auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
        SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
        auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
        SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
        editor->AddChildNode( scene, txt, lChild );
        editor->AddChildNode( scene, txt, rChild );

        CHECK( txt->GetNumChildren() == 2 );
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto txt = TestSceneUtils::ColoredText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        auto child = root->GetChild( TXT_NODE );
        auto lChild = child->GetChild( "lChild" );
        auto rChild = child->GetChild( "rChild" );
        CHECK( editor->DeleteChildNode( scene, root, child ) );
        editor->AddChildNode( scene, root, txt );
        editor->AddChildNode( scene, txt, lChild );
        editor->AddChildNode( scene, txt, rChild );
        CHECK( txt->GetNumChildren() == 2 );
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
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        
        auto desc = FontAssetDesc::Create( "fonts/couri.TTF", 30, 0, 0, 0, true );
        editor->LoadAsset( root->GetPlugin( "text" ), desc );
    });
    m_testSteps.push_back( [&]{ RestoreRoot( 1, "text" ); } );

    m_testSteps.push_back( [&]
    { 
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( TXT_NODE );
        SetParameter( child->GetPlugin( "text" )->GetParameter("text"), 0.0, std::wstring( L"test0\n4321" ) );
    });

    m_testSteps.push_back( [&]
    { 
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( TXT_NODE );

        auto desc = FontAssetDesc::Create( "fonts/couri.TTF", 30, 0, 0, 0, true );
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
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto txt = TestSceneUtils::GradientText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 0.f, 1.f, 0.f, 1.f ), 40 );
        SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TXT_NODE ) );
        editor->AddChildNode( scene, root, txt );
    };

    auto add1 = [&] 
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto txt = TestSceneUtils::GradientText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 0.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TXT_NODE ) );
        editor->AddChildNode( scene, root, txt );
        
        auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
        SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
        auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
        SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
        editor->AddChildNode( scene, txt, lChild );
        editor->AddChildNode( scene, txt, rChild );

        CHECK( txt->GetNumChildren() == 2 );
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto txt = TestSceneUtils::GradientText( editor->GetSceneDefaultTimeline( scene ), TXT_NODE, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 0.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( txt->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, -0.5f, 0.f ) );

        auto root = scene->GetRootNode();
        auto lChild = root->GetChild( TXT_NODE )->GetChild( "lChild" );
        auto rChild = root->GetChild( TXT_NODE )->GetChild( "rChild" );
        CHECK( editor->DeleteChildNode( scene, root, root->GetChild( TXT_NODE ) ) );
        editor->AddChildNode( scene, root, txt );
        editor->AddChildNode( scene, txt, lChild );
        editor->AddChildNode( scene, txt, rChild );
        ( txt->GetNumChildren() == 2 );
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
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto tmr = TestSceneUtils::ColoredTimer( editor->GetSceneDefaultTimeline( scene ), TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TMR_NODE ) );
        editor->AddChildNode( scene, root, tmr );
        
        auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
        SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
        auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
        SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
        editor->AddChildNode( scene, tmr, lChild );
        editor->AddChildNode( scene, tmr, rChild );

        CHECK( tmr->GetNumChildren() == 2 );
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto tmr = TestSceneUtils::ColoredTimer( editor->GetSceneDefaultTimeline( scene ), TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

        auto root = scene->GetRootNode();
        auto child = root->GetChild( TMR_NODE );
        auto lChild = child->GetChild( "lChild" );
        auto rChild = child->GetChild( "rChild" );
        CHECK( editor->DeleteChildNode( scene, root, child ) );
        editor->AddChildNode( scene, root, tmr );
        editor->AddChildNode( scene, tmr, lChild );
        editor->AddChildNode( scene, tmr, rChild );
        CHECK( tmr->GetNumChildren() == 2 );
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
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        model::SetTimeTimerPlugin( root->GetPlugin( "timer" ), 12333.0f );
        model::StartTimerPlugin( root->GetPlugin( "timer" ) );
    } );
    Wait( 2 );
    m_testSteps.push_back( [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        
        auto desc = FontAssetDesc::Create( "Assets/Fonts/couri.TTF", 40, 0, 0, 0, true );
        editor->LoadAsset( root->GetPlugin( "timer" ), desc );
    });
    Wait( 2 );
    m_testSteps.push_back( [&]{ RestoreRoot( 1, "timer" ); } );


    m_testSteps.push_back( [&]
    { 
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( TMR_NODE );
        model::SetTimeTimerPlugin( child->GetPlugin( "timer" ), 15.0f );
        model::StartTimerPlugin( child->GetPlugin( "timer" ) );
    });
    Wait( 1 );

    m_testSteps.push_back( [&]
    { 
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( TMR_NODE );

        auto desc = FontAssetDesc::Create( "Assets/Fonts/couri.TTF", 40, 0, 0, 0, true );
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
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto tmr = TestSceneUtils::GradientTimer( editor->GetSceneDefaultTimeline( scene ), TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), 40 );
        SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TMR_NODE ) );
        editor->AddChildNode( scene, root, tmr );
    };

    auto add1 = [&]
    {
        auto editor = m_project->GetProjectEditor();
        auto scene = editor->GetModelScene( SCENE_NAME );

        auto tmr = TestSceneUtils::GradientTimer( editor->GetSceneDefaultTimeline( scene ), TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

        auto root = scene->GetRootNode();
        editor->DeleteChildNode( scene, root, root->GetChild( TMR_NODE ) );
        editor->AddChildNode( scene, root, tmr );
        
        auto lChild = TestSceneUtils::ColoredRectangle( editor->GetSceneDefaultTimeline( scene ), "lChild", 0.1f, 0.1f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
        SetParameterTranslation( lChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, 0.25f, 0.0f ) );
        auto rChild = TestSceneUtils::TexturedRectangle( editor->GetSceneDefaultTimeline( scene ), "rChild", 0.1f, 0.1f, TestSceneUtils::TEXTURE_PATH );
        SetParameterTranslation( rChild->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 0.f, -0.25f, 0.0f ) );
        editor->AddChildNode( scene, tmr, lChild );
        editor->AddChildNode( scene, tmr, rChild );

        CHECK( tmr->GetNumChildren() == 2 );
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
        auto scene = editor->GetModelScene( SCENE_NAME );
        auto tmr = TestSceneUtils::GradientTimer( editor->GetSceneDefaultTimeline( scene ), TMR_NODE, glm::vec4( 1.f, 1.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), 40, TestSceneUtils::ALPHA_MASK_PATH );
        SetParameterTranslation( tmr->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 2.0f, 0.f, 0.f ) );

        auto root = scene->GetRootNode();
        auto child = root->GetChild( TMR_NODE );
        auto lChild = child->GetChild( "lChild" );
        auto rChild = child->GetChild( "rChild" );
        CHECK( editor->DeleteChildNode( scene, root, child ) );
        editor->AddChildNode( scene, root, tmr );
        editor->AddChildNode( scene, tmr, lChild );
        editor->AddChildNode( scene, tmr, rChild );
        CHECK( tmr->GetNumChildren() == 2 );
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
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( TMR_NODE );
        model::SetTimeTimerPlugin( child->GetPlugin( "timer" ), 15.0f );
        model::StartTimerPlugin( child->GetPlugin( "timer" ) );
    });
    Wait( 1 );

    m_testSteps.push_back( [&]
    { 
        auto editor = m_project->GetProjectEditor();
        auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
        auto child = root->GetChild( TMR_NODE );

        auto desc = FontAssetDesc::Create( "Assets/Fonts/couri.TTF", 40, 0, 0, 0, true );
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
            auto scene = editor->GetModelScene( SCENE_NAME );

            auto geom = TestSceneUtils::ColoredGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
            
            auto time = m_timeEvaluator->GetLocalTime(); {time;}
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 360.f, 360.f, 360.f ) );

            auto root = scene->GetRootNode();
            editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
            editor->AddChildNode( scene, root, geom );
        });
        Wait( 1 );

        m_testSteps.push_back( [ plugin, this ]
        {
            auto editor = m_project->GetProjectEditor();
            auto scene = editor->GetModelScene( SCENE_NAME );

            auto geom = TestSceneUtils::ColoredGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
            
            auto time = m_timeEvaluator->GetLocalTime();
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 360.f, 360.f, 360.f ) );

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
                auto scene = editor->GetModelScene( SCENE_NAME );

                auto geom = TestSceneUtils::ColoredGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 1.f, 0.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
                
                auto time = m_timeEvaluator->GetLocalTime();
                SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
                SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
                SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
                SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 360.f, 360.f, 360.f ) );

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
            auto scene = editor->GetModelScene( SCENE_NAME );

            auto geom = TestSceneUtils::TexturedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, TestSceneUtils::TEXTURE_PATH );
            
            auto time = m_timeEvaluator->GetLocalTime();
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 360.f, 360.f, 360.f ) );

            auto root = scene->GetRootNode();
            editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
            editor->AddChildNode( scene, root, geom );
        });
        Wait( 1 );

        m_testSteps.push_back( [ plugin, this ]
        {
            auto editor = m_project->GetProjectEditor();
            auto scene = editor->GetModelScene( SCENE_NAME );

            auto geom = TestSceneUtils::TexturedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );
            
            auto time = m_timeEvaluator->GetLocalTime();
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 360.f, 360.f, 360.f ) );

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
            auto geom = editor->GetModelScene( SCENE_NAME )->GetRootNode()->GetChild( GEOM_NODE );

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
                auto scene = editor->GetModelScene( SCENE_NAME );

                auto geom = TestSceneUtils::TexturedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, TestSceneUtils::TEXTURE_PATH, TestSceneUtils::ALPHA_MASK_PATH );

                auto time = m_timeEvaluator->GetLocalTime();
                SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
                SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
                SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
                SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 360.f, 360.f, 360.f ) );

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
            auto scene = editor->GetModelScene( SCENE_NAME );

            auto geom = TestSceneUtils::AnimatedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str() );
            
            auto time = m_timeEvaluator->GetLocalTime();
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 360.f, 360.f, 360.f ) );

            auto root = scene->GetRootNode();
            editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
            editor->AddChildNode( scene, root, geom );
        });
        Wait( 1 );

        m_testSteps.push_back( [ plugin, this ]
        {
            auto editor = m_project->GetProjectEditor();
            auto scene = editor->GetModelScene( SCENE_NAME );

            auto geom = TestSceneUtils::AnimatedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str(), TestSceneUtils::ALPHA_MASK_PATH );
            
            auto time = m_timeEvaluator->GetLocalTime();
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 360.f, 360.f, 360.f ) );

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
                auto scene = editor->GetModelScene( SCENE_NAME );

                auto geom = TestSceneUtils::AnimatedGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, ProjectManager::GetInstance()->ToAbsPath( TestSceneUtils::ANIM_PATH ).Str(), TestSceneUtils::ALPHA_MASK_PATH );

                auto time = m_timeEvaluator->GetLocalTime();
                SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
                SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
                SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
                SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 360.f, 360.f, 360.f ) );

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
            auto scene = editor->GetModelScene( SCENE_NAME );

            auto time = m_timeEvaluator->GetLocalTime();
            auto geom = TestSceneUtils::GradientGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
            
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 1.f, glm::vec3( 360.f, 360.f, 360.f ) );

            auto root = scene->GetRootNode();
            editor->DeleteChildNode( scene, root, root->GetChild( GEOM_NODE ) );
            editor->AddChildNode( scene, root, geom );
        });
        Wait( 1 );

        m_testSteps.push_back( [ plugin, this ]
        {
            auto editor = m_project->GetProjectEditor();
            auto scene = editor->GetModelScene( SCENE_NAME );

            auto time = m_timeEvaluator->GetLocalTime();
            auto geom = TestSceneUtils::GradientGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
            
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
            SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
            SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 5.f, glm::vec3( 360.f, 360.f, 360.f ) );

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
                auto scene = editor->GetModelScene( SCENE_NAME );

                auto geom = TestSceneUtils::GradientGeometry( editor->GetSceneDefaultTimeline( scene ), GEOM_NODE, plugin, glm::vec4( 1.f, 0.f, 0.f, 1.f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), TestSceneUtils::ALPHA_MASK_PATH );
            
                auto time = m_timeEvaluator->GetLocalTime();
                SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 1.0f, -0.1f, -2.f ) );
                SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time, glm::vec3( 0.f, 0.f, 0.f ) );
                SetParameterTranslation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 1.0f, -0.1f, -2.f ) );
                SetParameterRotation( geom->GetPlugin( "transform" )->GetParameter( "simple_transform" ), time + 3.f, glm::vec3( 360.f, 360.f, 360.f ) );

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
            auto scene = editor->GetModelScene( SCENE_NAME );
            auto vsd = TestSceneUtils::VideoStreamDecoder( editor->GetSceneDefaultTimeline( scene ), VSD_NODE, plugin, TestSceneUtils::VIDEO_PATH0 );
            SetParameterTranslation( vsd->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.0f, -0.1f, -2.f ) );

            auto root = scene->GetRootNode();
            editor->DeleteChildNode( scene, root, root->GetChild( VSD_NODE ) );
            editor->AddChildNode( scene, root, vsd );

            model::SetParameter( vsd->GetPlugin( "av_decoder" )->GetParameter( "state" ), 0.0f, 1 );
        };

        auto add1 = [ plugin, this ]
        {
            auto editor = m_project->GetProjectEditor();
            auto scene = editor->GetModelScene( SCENE_NAME );
            auto vsd = TestSceneUtils::VideoStreamDecoder( editor->GetSceneDefaultTimeline( scene ), VSD_NODE, plugin, TestSceneUtils::VIDEO_PATH0, TestSceneUtils::ALPHA_MASK_PATH );
            SetParameterTranslation( vsd->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.0f, -0.1f, -2.f ) );

            auto root = scene->GetRootNode();
            editor->DeleteChildNode( scene, root, root->GetChild( VSD_NODE ) );
            editor->AddChildNode( scene, root, vsd );

            model::SetParameter( vsd->GetPlugin( "av_decoder" )->GetParameter( "state" ), 0.0f, 1 );
        };

        m_testSteps.push_back( add0 );
        Wait( 1 );
        m_testSteps.push_back( add1 );
    
        m_testSteps.push_back( [&]{ SwapPlugins( "solid color", 2, VSD_NODE, "av_decoder", 2 ); } );
        m_testSteps.push_back( [&]{ SwapPlugins( "av_decoder", 2, VSD_NODE, "solid color", 2 ); } );

        m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, VSD_NODE, "alpha_mask", 3 ); } );
        m_testSteps.push_back( [&]{ SwapPlugins( "alpha_mask", 3, VSD_NODE, "alpha_mask", 3 ); } );
    
        m_testSteps.push_back( [&]{ 
            CopyPlugin( 2, "solid color", VSD_NODE, "av_decoder" );

            auto editor = m_project->GetProjectEditor();
            auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
            model::SetParameter( root->GetPlugin( "av_decoder" )->GetParameter( "state" ), 0.0f, 1 );
        } );
        Wait( 1 );
        m_testSteps.push_back( [&]
        {
            auto editor = m_project->GetProjectEditor();
            auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();

            auto desc = AVAssetDesc::Create( TestSceneUtils::VIDEO_PATH1, TextureFormat::F_A8R8G8B8 );
            editor->LoadAsset( root->GetPlugin( "av_decoder" ), desc );

            model::SetParameter( root->GetPlugin( "av_decoder" )->GetParameter( "state" ), 0.0f, 1 );
        });
        m_testSteps.push_back( [&]{ RestoreRoot( 2, "av_decoder" ); } );

        m_testSteps.push_back( [&]{
            auto editor = m_project->GetProjectEditor();
            auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
            auto child = root->GetChild( VSD_NODE );

            auto desc = AVAssetDesc::Create( TestSceneUtils::VIDEO_PATH1, TextureFormat::F_A8R8G8B8 );
            editor->LoadAsset( root->GetPlugin( "av_decoder" ), desc );

            model::SetParameter( child->GetPlugin( "av_decoder" )->GetParameter( "state" ), 0.0f, 1 );
        });

        auto pluginName = TestSceneUtils::PluginsNameArr[ i ];
        std::string test0[] = { "alpha_mask", pluginName };
        std::string test1[] = { "av_decoder", pluginName };

        std::vector < OrderTestCase > tests;
        tests.push_back( OrderTestCase( VSD_NODE, "AmGeom", std::vector< std::string >( test0, test0 + 2 ) ) );
        tests.push_back( OrderTestCase( VSD_NODE, "VsGeom", std::vector< std::string >( test1, test1 + 2 ) ) );

        for( auto & test : tests )
        {
            m_testSteps.push_back( [ plugin, this ]
            {
                auto editor = m_project->GetProjectEditor();
                auto scene = editor->GetModelScene( SCENE_NAME );
                auto vsd = TestSceneUtils::VideoStreamDecoder( editor->GetSceneDefaultTimeline( scene ), VSD_NODE, plugin, TestSceneUtils::VIDEO_PATH0, TestSceneUtils::ALPHA_MASK_PATH );
                SetParameterTranslation( vsd->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.0f, -0.1f, -2.f ) );

                auto root = scene->GetRootNode();
                editor->DeleteChildNode( scene, root, root->GetChild( VSD_NODE ) );
                editor->AddChildNode( scene, root, vsd );

                model::SetParameter( vsd->GetPlugin( "av_decoder" )->GetParameter( "state" ), 0.0f, 1 );
            });
            InitOrderTest( test );
        }

        m_testSteps.push_back( [ plugin, this ]
        {
            auto editor = m_project->GetProjectEditor();
            auto scene = editor->GetModelScene( SCENE_NAME );
            auto vsd = TestSceneUtils::VideoStreamDecoder( editor->GetSceneDefaultTimeline( scene ), VSD_NODE, plugin, TestSceneUtils::VIDEO_PATH0, TestSceneUtils::ALPHA_MASK_PATH );
            SetParameterTranslation( vsd->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0.0f, glm::vec3( 1.0f, -0.1f, -2.f ) );

            auto root = scene->GetRootNode();
            editor->DeleteChildNode( scene, root, root->GetChild( VSD_NODE ) );
            editor->AddChildNode( scene, root, vsd );

            model::SetParameter( vsd->GetPlugin( "av_decoder" )->GetParameter( "state" ), 0.0f, 1 );
        });

        m_testSteps.push_back( [&]{
            auto editor = m_project->GetProjectEditor();
            auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
            auto child = root->GetChild( VSD_NODE );

            auto desc = AVAssetDesc::Create( TestSceneUtils::VIDEO_PATH1, TextureFormat::F_A8R8G8B8 );
            editor->LoadAsset( child->GetPlugin( "av_decoder" ), desc );

            model::SetParameter( child->GetPlugin( "av_decoder" )->GetParameter( "state" ), 0.0f, 1 );
        });

        Wait( 3 );
        
        m_testSteps.push_back( [&]
        {
            auto editor = m_project->GetProjectEditor();
            auto scene = editor->GetModelScene( SCENE_NAME );
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
            auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
            auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( node ) );
              
            CHECK( editor->DeletePlugin( child, plugin ).first != nullptr );
            CHECK( editor->DeletePlugin( child, plugin ).first == nullptr );
            CHECK( child->GetPlugin( plugin ) == nullptr );
        });
    }
}

// ****************************
//
void					TestScene::SwapPlugins			( const std::string & rootPlugin, UInt32 rootIdx, const std::string & childName, const std::string & childPlugin,  UInt32 childIdx )
{
    auto editor = m_project->GetProjectEditor();

    auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();
    auto child = std::static_pointer_cast< model::BasicNode >( root->GetChild( childName ) );

    CHECK( editor->DetachPlugin( root, rootPlugin ) );
    CHECK( editor->DetachPlugin( child, childPlugin ) );

    CHECK( editor->GetDetachedPlugin( root ) != nullptr );
    CHECK( editor->GetDetachedPlugin( root )->GetName() == rootPlugin );

    CHECK( editor->AddPlugin( child, editor->GetDetachedPlugin( root ), childIdx ) );

    CHECK( editor->GetDetachedPlugin( child ) != nullptr );
    CHECK( editor->GetDetachedPlugin( child )->GetName() == childPlugin );

    CHECK( editor->AddPlugin( root, editor->GetDetachedPlugin( child ), rootIdx ) );
}

// ****************************
//
void						TestScene::CopyPlugin			( UInt32 rootIdx, const std::string & rootPlugin, const std::string & childName, const std::string & childPlugin )
{
    auto editor = m_project->GetProjectEditor();
    auto scene = editor->GetModelScene( SCENE_NAME );
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
    auto root = editor->GetModelScene( SCENE_NAME )->GetRootNode();

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
bool					TestScene::TestEditor				( TimeType time )
{
    auto step = ( Int32 )std::floor( TestSceneUtils::SPEED * time );
    
    if( m_lastStep != step && step - m_stepOffset < ( Int32 )m_testSteps.size() )
    {
        m_testSteps[ step - m_stepOffset ]();
    }
    
    m_lastStep = step;

    if( step - m_stepOffset >= ( Int32 )m_testSteps.size() )
        return true;
    return false;

}

} // bv