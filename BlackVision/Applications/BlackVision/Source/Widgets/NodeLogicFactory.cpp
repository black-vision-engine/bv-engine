#include "NodeLogicFactory.h"


#include "Counter/Counter.h"
#include "Crawler/Crawler.h"
#include "NodeReplicator/NodeReplicator.h"



namespace bv
{

model::INodeLogicPtr        NodeLogicFactory::CreateLogic  ( const IDeserializer & deser, model::BasicNodePtr logicParent, bv::model:: ITimeEvaluatorPtr timeEvaluator )
{
    const std::string& logicType = deser.GetAttribute( "type" );

    if( logicType == "crawler" )
        return nodelogic::Crawler::Create( deser, logicParent.get() );
    else if( logicType == "counter" )
        return nodelogic::WidgetCounter::Create( deser, logicParent.get(), timeEvaluator );
    else if( logicType == "replicate" )
        return model::NodeReplicator::Create( deser, logicParent.get() );

    return nullptr;
}


}
