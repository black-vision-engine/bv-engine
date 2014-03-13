#include "NewModelTestNodes.h"

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/Plugins/PluginsFactory.h"

#include "Engine/Models/Timeline/TimelineManager.h"


#include "Engine/Models/BasicNode.h"


namespace {
    std::string GSimplePlugins[] = { "DEFAULT_TRANSFORM", "DEFAULT_RECTANGLE", "DEFAULT_COLOR" };
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
    std::vector< std::string > GSimplePluginsUIDS( GSimplePlugins, GSimplePlugins + 3 );

    auto node = new model::BasicNode( "Root" );
    node->AddPlugins( GSimplePluginsUIDS );

    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 0.f,  glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 5.f,  glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 10.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 15.f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 20.f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 25.f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
    SetParameter( node->GetPlugin( "solid color" )->GetParameter( "color" ), 30.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );

    timelineManager->RegisterDefaultTimeline( nullptr, 15.0f, 45.0f, "timeline0" );
    timelineManager->AddParamToTimeline( node->GetPlugin( "solid color" )->GetParameter( "color" ), "timeline0" );

    SetParameterRotation ( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 0.0f, glm::vec3( 0.f, 0.f, 1.f ), 0.f );
    SetParameterRotation ( node->GetPlugin( "transform" )->GetParameter( "simple_transform" ), 0, 30.0f, glm::vec3( 0.f, 0.f, 1.f ), 360.f );

    return node;
}

} //bv
