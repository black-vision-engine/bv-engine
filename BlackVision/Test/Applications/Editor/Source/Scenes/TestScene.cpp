#include "TestScene.h"

#include "Engine/Models/Plugins/Simple/DefaultRectPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultColorPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTexturePlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"

#include "Engine/Models/Plugins/PluginUtils.h"

namespace bv {

// ****************************
//
						TestScene::TestScene				( Renderer * renderer )
{
    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( TestScene::DefaultBVPluginDescriptors() );
    
	auto globalTimeline = model::OffsetTimeEvaluatorPtr( new model::OffsetTimeEvaluator( "global timeline", TimeType( 0.0 ) ) );
	auto timelineManager = new model::TimelineManager();

	auto root = ColoredRectangles( timelineManager, globalTimeline );

    m_scene = BVScene::Create( root, new Camera(), "BasicScene", globalTimeline, renderer );

}

// ****************************
//
						TestScene::~TestScene				()
{
}

// ****************************
//
BVScenePtr				TestScene::GetScene					()
{
	return m_scene;
}

// ****************************
//
model::BasicNodePtr		TestScene::ColoredRectangles		( model::TimelineManager * timelineManager, model::ITimeEvaluatorPtr timeEvaluator )
{
    auto someTimelineWithEvents = timelineManager->CreateDefaultTimelineImpl( "evt timeline", TimeType( 20.0 ), TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    timelineManager->AddStopEventToTimeline( someTimelineWithEvents, "stop0", TimeType( 5.0 ) );
    timelineManager->AddStopEventToTimeline( someTimelineWithEvents, "stop1", TimeType( 10.0 ) );
    
	auto localTimeline = timelineManager->CreateOffsetTimeEvaluator( "timeline0" , TimeType( 1.0 ) );

    someTimelineWithEvents->AddChild( localTimeline );
    timeEvaluator->AddChild( someTimelineWithEvents );

    auto node = model::BasicNode::Create( "Root", timeEvaluator );

	std::vector< std::string > plugins;
	plugins.push_back( "DEFAULT_TRANSFORM" );
	plugins.push_back( "DEFAULT_RECTANGLE" );
	plugins.push_back( "DEFAULT_COLOR" );
    auto success = node->AddPlugins( plugins, localTimeline );

    assert( success );

	//auto paramTransform = node->GetPlugin( "transform" )->GetParameter( "simple_transform" );
	//SetParameterScale ( paramTransform, 0, 0.0f, glm::vec3( 1.f, 1.f, 1.f ) );
	//SetParameterRotation( paramTransform, 0, 0.f, glm::vec3( 0.f, 1.f, 0.f ), 0.f );
	//SetParameterRotation( paramTransform, 0, 5.f, glm::vec3( 0.f, 1.f, 0.f ), 70.f );
	//SetParameterRotation( paramTransform, 0, 100.f, glm::vec3( 0.f, 1.f, 0.f ), -70.f );
 //   paramTransform->SetTimeEvaluator( timeEvaluator );
 //   paramTransform->SetTimeEvaluator( timeEvaluator );

	auto paramColor = node->GetPlugin( "solid color" )->GetParameter( "color" );
    SetParameter( paramColor, 0.f,  glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    SetParameter( paramColor, 5.f,  glm::vec4( 1.f, 0.f, 0.f, 1.f ) );
    SetParameter( paramColor, 10.f, glm::vec4( 0.f, 0.f, 1.f, 1.f ) );
    SetParameter( paramColor, 15.f, glm::vec4( 1.f, 1.f, 0.f, 1.f ) );
    SetParameter( paramColor, 20.f, glm::vec4( 0.f, 1.f, 1.f, 1.f ) );
    SetParameter( paramColor, 25.f, glm::vec4( 1.f, 0.f, 1.f, 1.f ) );
    SetParameter( paramColor, 30.f, glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
    paramColor->SetTimeEvaluator( localTimeline );

	model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "height" ), TimeType( 0.f ), 0.2f );
    model::SetParameter( node->GetPlugin( "rectangle" )->GetParameter( "width" ), TimeType( 0.f ),  0.2f );

    return node;
}

// ****************************
//
std::vector< model::IPluginDescriptor * >  TestScene::DefaultBVPluginDescriptors  ()
{
    std::vector< model::IPluginDescriptor * > descriptors;

    descriptors.push_back( new model::DefaultTransformPluginDesc() );
    descriptors.push_back( new model::DefaultRectPluginDesc() );
    descriptors.push_back( new model::DefaultColorPluginDesc() );
    descriptors.push_back( new model::DefaultTexturePluginDesc() );
    descriptors.push_back( new model::DefaultTextPluginDesc() );

    for( auto descr : descriptors )
    {
        printf( "Registered plugin desc: %s\n", descr->GetPluginTypeUID().c_str() ); 
    }

    return descriptors;
}

} // bv
