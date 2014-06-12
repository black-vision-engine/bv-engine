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


namespace {
    std::string GSimplePlugins0[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_COLOR" };
    std::string GSimplePlugins1[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_TEXTURE" };
    std::string GSimplePlugins2[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_ANIMATION" };
    std::string GSimplePlugins3[] = { "DEFAULT_TRANSFORM", "DEFAULT_TEXT" };

    // *****************************
    //
    void SetDefaultColorChangeAnim( bv::model::IPlugin * plugin )
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
    void SetDefaultTransformAnim( bv::model::IPlugin * plugin )
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
model::BasicNode *  SimpleNodesFactory::CreateSolidRectNode      ( const std::string & name, float w, float h, const glm::vec3 & pos, const glm::vec4 col, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNode * root = new model::BasicNode( name, timeEvaluator );
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
model::BasicNode *  SimpleNodesFactory::CreateTexturedRectNode   ( const std::string & name, float w, float h, const glm::vec3 & pos, const std::string & txFileName, model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    //Plugin stuff
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins1, GSimplePlugins1 + 3 );

    auto root = new model::BasicNode( name, timeEvaluator );
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
model::BasicNode *  SimpleNodesFactory::CreateOverrideAlphaTest  ( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto root = CreateTexturedRectNode( ".", 3.4f, 0.7f, glm::vec3( 0.f, -.6f, 0.f ), "simless_01.jpg", timelineManager, timeEvaluator );
    auto c0 = CreateSolidRectNode( "node0", .85f, 0.31f, glm::vec3( 0.f, -0.17f, .01f ), glm::vec4( 0.f, 1.f, 1.f, 1.f ), timelineManager, timeEvaluator );
    auto c1 = CreateSolidRectNode( "node1", .85f, 0.31f, glm::vec3( 0.f, 0.17f, 0.01f ), glm::vec4( 1.f, 0.f, 1.f, 1.f ), timelineManager, timeEvaluator );

    auto c00 = CreateSolidRectNode( "node00", .3f, 0.2f, glm::vec3( -0.2f, 0.0f, 0.01f ), glm::vec4( 1.f, 0.f, 0.f, 1.f ), timelineManager, timeEvaluator );
    auto c01 = CreateTexturedRectNode( "node01", .3f, 0.23f, glm::vec3( 0.25f, 0.0f, 0.01f ), "simless_00.jpg", timelineManager, timeEvaluator );
    auto c10 = CreateTexturedRectNode( "node10", .15f, 0.2f, glm::vec3( -0.32f, .0f, 0.01f ), "simless_00.jpg", timelineManager, timeEvaluator );
    auto c11 = CreateSolidRectNode( "node11", .2f, 0.2f, glm::vec3( -0.1f, .0f, 0.01f ), glm::vec4( 0.f, 1.f, 0.f, 1.f ), timelineManager, timeEvaluator );
    auto c12 = CreateSolidRectNode( "node12", .15f, 0.15f, glm::vec3( 0.12f, .0f, 0.01f ), glm::vec4( 0.f, 0.f, 1.f, 1.f ), timelineManager, timeEvaluator );
    auto c13 = CreateTexturedRectNode( "node13", .15f, 0.15f, glm::vec3( 0.32f, .0f, 0.01f ), "simless_00.jpg", timelineManager, timeEvaluator );

    root->AddChild( c0 );
    root->AddChild( c1 );
    c0->AddChild( c00 );
    c0->AddChild( c01 );
    c1->AddChild( c10 );
    c1->AddChild( c11 );
    c1->AddChild( c12 );
    c1->AddChild( c13 );

    auto c0tr = c0->GetPlugin( "transform" )->GetParameter( "simple_transform" );
    auto c1tr = c1->GetPlugin( "transform" )->GetParameter( "simple_transform" );

    SetParameterTranslation( c0tr, 0, 0.0f, glm::vec3( -1.f, -0.17f, 0.f ) );
    SetParameterTranslation( c0tr, 0, 2.0f, glm::vec3( -1.1f, -0.17f, 0.f ) );
    SetParameterTranslation( c0tr, 0, 4.0f, glm::vec3( -0.2f, -0.17f, 0.f ) );
    SetParameterTranslation( c0tr, 0, 6.0f, glm::vec3( 0.5f, -0.17f, 0.f ) );
    SetParameterTranslation( c0tr, 0, 9.0f, glm::vec3( 1.2f, -0.17f, 0.f ) );

    SetParameterTranslation( c1tr, 0, 0.0f, glm::vec3( 1.3f, 0.17f, 0.f ) );
    SetParameterTranslation( c1tr, 0, 4.0f, glm::vec3( 0.6f, 0.17f, 0.f ) );
    SetParameterTranslation( c1tr, 0, 7.0f, glm::vec3( -0.5f, 0.17f, 0.f ) );
    SetParameterTranslation( c1tr, 0, 8.0f, glm::vec3( -1.2f, 0.17f, 0.f ) );
    SetParameterTranslation( c1tr, 0, 9.0f, glm::vec3( -0.3f, 0.17f, 0.f ) );
    SetParameterTranslation( c1tr, 0, 14.0f, glm::vec3( 1.2f, 0.17f, 0.f ) );

    return root;
}

// *****************************
//
model::BasicNode *  SimpleNodesFactory::CreateGreenRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator, const std::string & nodeName )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNode * root = new model::BasicNode( nodeName, timeEvaluator );

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
model::BasicNode *  SimpleNodesFactory::CreateGreenRectNodeNoAssert( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins0, GSimplePlugins0 + 3 );

    auto node = new model::BasicNode( "Root", timeEvaluator );
    node->AddPlugins( GSimplePluginsUIDS, timeEvaluator );

    SetDefaultColorChangeAnim( node->GetPlugin( "solid color" ) );

    auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 15.0 ) );
    node->GetPlugin( "solid color" )->GetParameter( "color" )->SetTimeEvaluator( localTimeline );

    SetDefaultTransformAnim( node->GetPlugin( "transform" ) );

    return node;
}

// *****************************
//
model::BasicNode *  SimpleNodesFactory::CreateTexturedRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
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

    auto node = new model::BasicNode( "Root", timeEvaluator );

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
model::BasicNode *  SimpleNodesFactory::CreateTextureAnimationRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
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

    auto node = new model::BasicNode( "Root", timeEvaluator );

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

model::BasicNode *  SimpleNodesFactory::CreateTextNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    //Timeline stuff
    auto someTimelineWithEvents = timelineManager->CreateDefaultTimelineImpl( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    timelineManager->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    timelineManager->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
    auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 3.0 ) );

    someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( someTimelineWithEvents );

    //Plugin stuff
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins3, GSimplePlugins3 + 2 );

    auto node = new model::BasicNode( "Root", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );

    SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    SetParameter( node->GetPlugin( "text" )->GetParameter( "fontSize" ), TimeType( 0.0 ), 60.f );
    //node->GetPlugin( "transform" )->GetParameter( "simple_transform" )->SetTimeEvaluator( localTimeline );

    success = model::LoadFont( node->GetPlugin( "text" ), "../dep/Media/fonts/ARIALUNI.TTF" );
    assert( success );

    model::SetTextPluginContent( node->GetPlugin( "text" ), L"bla bla" );

    auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    ai->SetTimeline( someTimelineWithEvents );

    return node;    
}

} //bv
