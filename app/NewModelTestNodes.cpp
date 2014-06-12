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
    std::string GSimplePlugins3[] = { "DEFAULT_TRANSFORM", "DEFAULT_COLOR", "DEFAULT_TEXT" };
    std::string GSimplePlugins4[] = { "DEFAULT_TRANSFORM", "DEFAULT_TEXT" };

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
    //std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins4, GSimplePlugins4 + 2 );
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins3, GSimplePlugins3 + 3 );

    auto node = new model::BasicNode( "Root", timeEvaluator );

    auto success = node->AddPlugins( GSimplePluginsUIDS, localTimeline );
    assert( success );

    SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), TimeType( 0.0 ), glm::vec4( 1.0f, 0.0f, 1.0f, 1.0f ) );
    SetParameter( node->GetPlugin( "text" )->GetParameter( "fontSize" ), TimeType( 0.0 ), 60.f );

    success = model::LoadFont( node->GetPlugin( "text" ), "../dep/Media/fonts/ARIALUNI.TTF" );
    assert( success );

    model::SetTextPluginContent( node->GetPlugin( "text" ), L"bla bla" );

    auto ai = TestAIManager::Instance().GetAIPreset( 2 );
    ai->SetTimeline( someTimelineWithEvents );

    return node;    
}

} //bv
