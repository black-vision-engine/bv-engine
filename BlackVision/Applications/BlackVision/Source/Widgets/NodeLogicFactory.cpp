#include "NodeLogicFactory.h"


#include "Counter/Counter.h"
#include "Crawler/Crawler.h"
#include "NodeReplicator/NodeReplicator.h"



namespace bv
{

model::INodeLogicPtr        NodeLogicFactory::CreateLogic  ( const std::string& logicType, model::BasicNodePtr logicParent, bv::model:: ITimeEvaluatorPtr timeEvaluator, const IDeserializer & deser )
{
    if( logicType == "crawler" )
        return widgets::Crawler::Create( deser, logicParent.get() );
    else if( logicType == "counter" )
        return widgets::WidgetCounter::Create( deser, logicParent.get(), timeEvaluator );
    else if( logicType == "replicate" )
        return model::NodeReplicator::Create( deser, logicParent.get() );

    return nullptr;
}


}
