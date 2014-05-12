#include "NewModelTestNodes.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/PluginsFactory.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/PluginUtils.h"

#include "Engine/Models/BasicNode.h"


namespace {
    std::string GSimplePlugins0[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_COLOR" };
    std::string GSimplePlugins1[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_TEXTURE" };
    std::string GSimplePlugins2[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_ANIMATION" };

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
model::BasicNode *  SimpleNodesFactory::CreateGreenRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNode * root = new model::BasicNode( "Root" );

    bool success = root->AddPlugins( uids, timeEvaluator );
    assert( success );

    //Set some values to make it look like a scene
    auto color = root->GetPlugin( "solid color" )->GetParameter( "color" );
    assert( color );

    success = SetParameter( color, 0.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    assert( success );

    return root;
}

// *****************************
//
model::BasicNode *  SimpleNodesFactory::CreateGreenRectNodeNoAssert( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins0, GSimplePlugins0 + 3 );

    auto node = new model::BasicNode( "Root" );
    node->AddPlugins( GSimplePluginsUIDS, timeEvaluator );

    SetDefaultColorChangeAnim( node->GetPlugin( "solid color" ) );

    //FIXME: do poprawy
    //timelineManager->RegisterDefaultTimeline( nullptr, 15.0f, 45.0f, "timeline0" );
    timelineManager->AddParamToTimeline( node->GetPlugin( "solid color" )->GetParameter( "color" ), "timeline0" );

    SetDefaultTransformAnim( node->GetPlugin( "transform" ) );

    return node;
}

// *****************************
//
model::BasicNode *  SimpleNodesFactory::CreateTexturedRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins1, GSimplePlugins1 + 3 );

    auto node = new model::BasicNode( "Root" );

    auto success = node->AddPlugins( GSimplePluginsUIDS, timeEvaluator );
    assert( success );

    SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    success = model::LoadTexture( node->GetPlugin( "texture" ), "simless_00.jpg" );
    assert( success );

    return node;    
}

// *****************************
//
model::BasicNode *  SimpleNodesFactory::CreateTextureAnimationRectNode( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins2, GSimplePlugins2 + 3 );

    auto node = new model::BasicNode( "Root" );

    auto success = node->AddPlugins( GSimplePluginsUIDS, timeEvaluator );
    assert( success );

    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), TimeType( 0.f ), 0.f );
    SetParameter( node->GetPlugin( "animation" )->GetParameter( "frameNum" ), TimeType( 5.f ), 99.f );

    SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    success = model::LoadAnimation( node->GetPlugin( "animation" ), "../../media/sequences/FullHD/alfai", "*.tga" );
    assert( success );

    return node;    
}

} //bv
