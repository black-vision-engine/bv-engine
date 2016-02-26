#include "NodeLogicFactory.h"


#include "Counter/Counter.h"
#include "Scroller/Scroller.h"
#include "NodeReplicator/NodeReplicator.h"



namespace bv
{

model::INodeLogicPtr        NodeLogicFactory::CreateLogic  ( const IDeserializer & deser, model::BasicNodePtr logicParent )
{
    const std::string& logicType = deser.GetAttribute( "type" );

    if( logicType == nodelogic::Scroller::Type() )
        return nodelogic::Scroller::Create( deser, logicParent );
    else if( logicType == nodelogic::WidgetCounter::Type() )
        return nodelogic::WidgetCounter::Create( deser, logicParent.get() );
    else if( logicType == model::NodeReplicator::Type() )
        return model::NodeReplicator::Create( deser, logicParent );

    return nullptr;
}


}
