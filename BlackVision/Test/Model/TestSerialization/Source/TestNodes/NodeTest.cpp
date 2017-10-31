#include "gtest/gtest.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "DefaultPlugins.h"
#include "Engine/Models/Plugins/Simple/DefaultTransformPlugin.h"

#include "Utils/Serialization/Serialize.h"
#include "Utils/Comparators/NodeComparator.h"

using namespace bv;

// ***********************
//
TEST( Serialization_Node, SerializaDeserialize )
{
    model::PluginsManager::DefaultInstanceRef().RegisterDescriptors( model::DefaultBVPluginDescriptors() );

    auto timeline = model::TimelineHelper::CreateOffsetTimeEvaluator( "default", 444. );

    auto tm = model::TimelineManager();
    tm.AddTimeline( timeline );
    model::TimelineManager::SetInstance( & tm );

    auto expectedNode = model::BasicNode::Create( "SerDeserTest" );
    expectedNode->AddPlugin( "DEFAULT_TRANSFORM", "transform", timeline );

    Serialize( *expectedNode, "SerDeserNode.xml" );

    auto context = BVDeserializeContext::CreateContextFromEmptiness();
    auto actualNode = Deserialize< model::BasicNode >( "SerDeserNode.xml", "node", context );

    EXPECT_TRUE( model::NodeComparator::CompareStructure( *expectedNode, *actualNode ) );
}