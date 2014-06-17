#include "NewModelTestNodes.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/PluginUtils.h"

#include "Engine/Models/BasicNode.h"

#include "testai/TestAIManager.h"
#include "Helpers/RectNodeBuilder.h"

namespace {
    std::string GSimplePlugins0[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_COLOR" };
    std::string GSimplePlugins1[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_TEXTURE" };
    std::string GSimplePlugins2[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_ANIMATION" };
    std::string GSimplePlugins3[] = { "DEFAULT_TRANSFORM", "DEFAULT_COLOR", "DEFAULT_TEXT" };
    std::string GSimplePlugins4[] = { "DEFAULT_TRANSFORM", "DEFAULT_TEXT" };

    // *****************************
    //
    void SetDefaultColorChangeAnim( bv::model::IPluginPtr plugin )
    {
        auto param = plugin->GetParameter( "color" );
        assert( param );

        SetParameter( param, 0.f,  glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
        SetParameter( param, 5.f,  glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
        SetParameter( param, 10.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
        SetParameter( param, 15.f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
        SetParameter( param, 20.f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
        SetParameter( param, 25.f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
        SetParameter( param, 30.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    }

    // *****************************
    //
    void SetDefaultTransformAnim( bv::model::IPluginPtr plugin )
    {
        auto param = plugin->GetParameter( "simple_transform" );
        assert( param );

        SetParameterRotation ( param, 0, 0.0f, glm::vec3( 0.f, 0.f, 1.f ), 0.f );
        SetParameterRotation ( param, 0, 30.0f, glm::vec3( 0.f, 0.f, 1.f ), 360.f );
    }

} //anonymous

namespace bv {

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateSolidRectNode      ( const std::string & name, float w, float h, const glm::vec3 & pos, const glm::vec4 col, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNodePtr root = std::make_shared< model::BasicNode >( name, timeEvaluator );
    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    auto wp = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    auto hp = root->GetPlugin( "rectangle" )->GetParameter( "height" );
    auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
    auto tr = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    success &= SetParameter( wp, 0.f, w );
    success &= SetParameter( hp, 0.f, h );
    success &= SetParameter( color, 0.f, col );
    success &= SetParameterTranslation( tr, 0, 0.0f, pos );

    assert( success );

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateTexturedRectNode   ( const std::string & name, float w, float h, const glm::vec3 & pos, const std::string & txFileName, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    //Plugin stuff
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins1, GSimplePlugins1 + 3 );

    auto root = std::make_shared< model::BasicNode >( name, timeEvaluator );
    auto success = root->AddPlugins( GSimplePluginsUIDS, timeEvaluator );
    assert( success );

    auto wp = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    auto hp = root->GetPlugin( "rectangle" )->GetParameter( "height" );
    auto tr = root->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    success &= SetParameter( wp, 0.f, w );
    success &= SetParameter( hp, 0.f, h );
    success &= SetParameterTranslation( tr, 0, 0.0f, pos );

    success = model::LoadTexture( root->GetPlugin( "texture" ), txFileName );
    assert( success );

    return root;    
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateOverrideAlphaTest  ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    TexturedRectNodeBuilder bTex( timeEvaluator, "simless_01.jpg", false, 3.4f, 0.7f );
    SolidRectNodeBuilder bSolid( timeEvaluator, glm::vec4( 0.f, 1.f, 1.f, 1.f ), .85f, 0.31f );

    bTex.SetPosition( 0.f, -.6f, 0.f );
    auto root = bTex.CreateNode( ".", true );

    bTex.SetW( .85f );
    bTex.SetH( 0.31f );
    bTex.SetPosition( 1.3f, -0.17f, 0.01f, 0.0f );
    bTex.SetTextureFile( "simless_00.jpg", false );
    bTex.SetPosition( -1.f,  -0.17f, 0.01f, 0.0f );
    bTex.SetPosition( -1.1f, -0.17f, 0.01f, 2.0f );
    bTex.SetPosition( -0.2f, -0.17f, 0.01f, 4.0f );
    bTex.SetPosition( 0.5f,  -0.17f, 0.01f, 6.0f );
    bTex.SetPosition( 1.2f,  -0.17f, 0.01f, 9.0f );
    auto c0 = bTex.CreateNode( "node0", true );

    bSolid.SetColor( 1.f, 0.f, 1.f, 0.2f, 0.f );
    bSolid.SetColor( 1.f, 0.f, 1.f, 1.0f, 5.f );
    bSolid.SetColor( 1.f, 0.f, 1.f, 0.1f, 10.f );
    bSolid.SetColor( 1.f, 0.f, 1.f, 0.8f, 15.f );
    bSolid.SetPosition( 1.3f, 0.17f, 0.01f, 0.0f );
    bSolid.SetPosition( 0.6f, 0.17f, 0.01f, 4.0f );
    bSolid.SetPosition( -0.5f, 0.17f, 0.01f, 7.0f );
    bSolid.SetPosition( -1.2f, 0.17f, 0.01f, 8.0f );
    bSolid.SetPosition( -0.3f, 0.17f, 0.01f, 9.0f );
    bSolid.SetPosition(  1.2f, 0.17f, 0.01f, 14.0f );
    auto c1 = bSolid.CreateNode( "node1" );

    bSolid.SetColor( 0.f, 1.f, 0.f, 1.0f, 0.f );
    bSolid.SetW( 0.25f );
    bSolid.SetH( 0.25f );
    bSolid.SetPosition( -0.2f, 0.f, 0.01f );
    auto c00 = bSolid.CreateNode( "node01" );

    bSolid.SetColor( 0.f, 0.f, 1.f, 1.0f, 0.f );
    bSolid.SetW( 0.28f );
    bSolid.SetH( 0.28f );
    bSolid.SetPosition( 0.2f, 0.f, 0.01f );
    auto c01 = bSolid.CreateNode( "node02" );

    bTex.SetTextureFile( "simless_01.jpg", false );
    bTex.SetW( .15f );
    bTex.SetH( 0.15f );
    bTex.SetPosition( 0.f, 0.f, 0.01f );
    bTex.SetRotation( 0.f, 0.f, 1.f, 0.0f, 0.f );
    bTex.SetRotation( 0.f, 0.f, 1.f, 360.0f, 15.f );
    auto c010 = bTex.CreateNode( "node010", true );

    root->AddChild( c0 );
    root->AddChild( c1 );
    c0->AddChild( c00 );
    c0->AddChild( c01 );
    c01->AddChild( c010 );

    auto ai = TestAIManager::Instance().GetAIPreset( 3, root );

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateGreenRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & nodeName )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNodePtr root = std::make_shared< model::BasicNode >( nodeName, timeEvaluator );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    //Set some values to make it look like a scene
    SetDefaultTransformAnim( root->GetPlugin( "transform" ) );

    auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
    assert( color );

    auto w = root->GetPlugin( "rectangle" )->GetParameter( "width" );
    auto h = root->GetPlugin( "rectangle" )->GetParameter( "height" );

    success &= SetParameter( w, 0.f, 2.f );
    success &= SetParameter( h, 0.f, 1.f );

    success &= SetParameter( w, 20.f, 1.f );
    success &= SetParameter( h, 20.f, 2.f );

    success &= SetParameter( color, 0.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );

    assert( success );

    return root;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateGreenRectNodeNoAssert( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins0, GSimplePlugins0 + 3 );

    auto node = std::make_shared< model::BasicNode >( "Root", timeEvaluator );
    node->AddPlugins( GSimplePluginsUIDS, timeEvaluator );

    SetDefaultColorChangeAnim( node->GetPlugin( "solid color" ) );

    auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 15.0 ) );
    node->GetPlugin( "solid color" )->GetParameter( "color" )->SetTimeEvaluator( localTimeline );

    SetDefaultTransformAnim( node->GetPlugin( "transform" ) );

    return node;
}

// *****************************
//
model::BasicNodePtr  SimpleNodesFactory::CreateTexturedRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    //Timeline stuff
    auto someTimelineWithEvents = timelineManager->CreateDefaultTimelineImpl( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    timelineManager->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    timelineManager->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 3.0 ) );

    someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( someTimelineWithEvents );

    //Plugin stuff
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins1, GSimplePlugins1 + 3 );

    auto node = std::make_shared< model::BasicNode >( "Root", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );

    SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    //node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->SetTimeEvaluator( localTimeline );

    success = model::LoadTexture( node->GetPlugin( "texture" ), "pliczek_z_kwiatkiem.jpg" );
    assert( success );

    auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    ai->SetTimeline( someTimelineWithEvents );

    return node;    
}

// *****************************
//
model::BasicNodePtr SimpleNodesFactory::CreateTextureAnimationRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    //Timeline stuff
    auto someTimelineWithEvents = timelineManager->CreateDefaultTimelineImpl( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    timelineManager->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    timelineManager->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 3.0 ) );

    someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( someTimelineWithEvents );

    //Plugin stuf
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins2, GSimplePlugins2 + 3 );

    auto node = std::make_shared< model::BasicNode >( "Root", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, timeEvaluator );
    assert( success );

    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), TimeType( 0.f ), 0.f );
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), TimeType( 10.f ), 99.f );

    node->GetPlugin( "animation" )->GetParameter( "frameNum" )->SetTimeEvaluator( someTimelineWithEvents );

    SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    success = model::LoadAnimation( node->GetPlugin( "animation" ), "../../media/sequences/FullHD/alfai", "*.tga" );
    assert( success );

    auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    ai->SetTimeline( someTimelineWithEvents );

    return node;    
}

model::BasicNodePtr  SimpleNodesFactory::CreateTextNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, unsigned int blurSize )
{
    //Timeline stuff
    auto someTimelineWithEvents = timelineManager->CreateDefaultTimelineImpl( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    timelineManager->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    timelineManager->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 3.0 ) );

    someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( someTimelineWithEvents );

    //Plugin stuff
    //std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins4, GSimplePlugins4 + 2 );
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins3, GSimplePlugins3 + 3 );

    auto node = std::make_shared< model::BasicNode >( "Root", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );

    //SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    auto plugin = node->GetPlugin( "transform" );
    auto param = plugin->GetParameter( "simple_transform" );

    SetParameterTranslation( param, 0, 0.0f, glm::vec3( -12.f, 0.f, 0.f ) );

    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), TimeType( 0.0 ), glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "fontSize" ), TimeType( 0.0 ), 60.f );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "blurSize" ), TimeType( 0.0 ), float( blurSize ) );

    success = model::LoadFont( node->GetPlugin( "text" ), "../dep/Media/fonts/ARIALUNI.TTF" );
    assert( success );

    model::SetTextPluginContent( node->GetPlugin( "text" ), L"bla bla" );

    auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    ai->SetTimeline( someTimelineWithEvents );

    return node;    
}

model::BasicNodePtr  SimpleNodesFactory::CreateTextWithShadowNode(   model::TimelineManager * timelineManager,
                                                                    model::ITimeEvaluatorPtr timeEvaluator,
                                                                    unsigned int blurSize,
                                                                    const glm::vec3 shadowTranslation )
{
    auto shadowNode =  SimpleNodesFactory::CreateTextNode( timelineManager, timeEvaluator, blurSize );
    auto transPlugin = shadowNode->GetPlugin( "transform" );

    auto param = transPlugin->GetParameter( "simple_transform" );
    assert( param );

    model::SetParameterTranslation ( param, 0, 0.0f, shadowTranslation );

    auto node =  SimpleNodesFactory::CreateTextNode( timelineManager, timeEvaluator, 0 );

    transPlugin = node->GetPlugin( "transform" );

    param = transPlugin->GetParameter( "simple_transform" );
    assert( param );

    model::SetParameterTranslation ( param, 0, 0.0f, -shadowTranslation );

    auto tx =  SimpleNodesFactory::CreateTextNode( timelineManager, timeEvaluator, 0 );

    auto plugin = tx->GetPlugin( "transform" );
    param = plugin->GetParameter( "simple_transform" );
    auto plo = tx->GetPlugin( "solid color" );
    auto sp = plo->GetParameter( "color" );
    assert( sp );

    SetParameter( sp, TimeType( 0.0 ), glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );

    auto st = -shadowTranslation;
    st.z += 0.001f;
    SetParameterTranslation( param, 0, 0.0f, st );

    shadowNode->AddChild( node );
    shadowNode->AddChild( tx );

    return shadowNode;
}

} //bv
