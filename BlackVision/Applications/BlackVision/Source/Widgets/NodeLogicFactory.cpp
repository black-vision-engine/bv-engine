#include "NodeLogicFactory.h"


#include "Counter/Counter.h"
#include "Crawler/Crawler.h"
#include "NodeReplicator/NodeReplicator.h"



namespace bv
{

model::INodeLogicPtr        NodeLogicFactory::CreateLogic  ( const IDeserializer & deser, model::BasicNode* logicParent )
{
    const std::string& logicType = deser.GetAttribute( "type" );

    if( logicType == "crawler" )
        return nodelogic::Crawler::Create( deser, logicParent );
    else if( logicType == "counter" )
        return nodelogic::WidgetCounter::Create( deser, logicParent );
    else if( logicType == "replicate" )
        return model::NodeReplicator::Create( deser, logicParent );

    return nullptr;
}


}
