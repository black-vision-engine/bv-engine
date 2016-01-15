#include "NodeLogicFactory.h"


#include "Counter/Counter.h"
#include "Crawler/Crawler.h"
#include "NodeReplicator/NodeReplicator.h"



namespace bv
{

model::INodeLogicPtr        NodeLogicFactory::CreateLogic  ( model::BasicNodePtr logicParent, const IDeserializer & deser )
{
    logicParent;
    deser;
    return nullptr;
}


}
