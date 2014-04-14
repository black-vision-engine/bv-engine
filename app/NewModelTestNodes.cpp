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
model::BasicNode *  SimpleNodesFactory::CreateGreenRectNode( model::TimelineManager * timelineManager )
{
    //Plugin list
    std::vector< std::string > uids;

    uids.push_back( "DEFAULT_TRANSFORM" );
    uids.push_back( "DEFAULT_RECTANGLE" );
    uids.push_back( "DEFAULT_COLOR" );

    //Create a model
    model::BasicNode * root = new model::BasicNode( "Root" );

    bool success = root->AddPlugins( uids );
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
model::BasicNode *  SimpleNodesFactory::CreateGreenRectNodeNoAssert( model::TimelineManager * timelineManager )
{
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins0, GSimplePlugins0 + 3 );

    auto node = new model::BasicNode( "Root" );
    node->AddPlugins( GSimplePluginsUIDS );

    SetDefaultColorChangeAnim( node->GetPlugin( "solid color" ) );

    timelineManager->RegisterDefaultTimeline( nullptr, 15.0f, 45.0f, "timeline0" );
    timelineManager->AddParamToTimeline( node->GetPlugin( "solid color" )->GetParameter( "color" ), "timeline0" );

    SetDefaultTransformAnim( node->GetPlugin( "transform" ) );

    return node;
}

// *****************************
//
model::BasicNode *  SimpleNodesFactory::CreateTexturedRectNode( model::TimelineManager * timelineManager )
{
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins1, GSimplePlugins1 + 3 );

    auto node = new model::BasicNode( "Root" );

    auto success = node->AddPlugins( GSimplePluginsUIDS );
    assert( success );

    SetDefaultColorChangeAnim   ( node->GetPlugin( "solid color" ) );
    SetDefaultTransformAnim     ( node->GetPlugin( "transform" ) );

    success = model::LoadTexture( node->GetPlugin( "texture" ), "simless_01.jpg" );
    assert( success );

    return node;    
}

} //bv
