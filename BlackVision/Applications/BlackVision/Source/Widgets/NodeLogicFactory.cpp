#include "stdafxBVApp.h"

#include "NodeLogicFactory.h"

#include "Counter/Counter.h"
#include "Scroller/Scroller.h"
#include "NodeReplicator/NodeReplicator.h"
#include "MeshLoader/MeshLoader.h"
#include "SmoothValueSetter/SmoothValueSetter.h"
#include "PieChart/PieChart.h"
#include "AnalogWatch/AnalogWatch.h"
#include "Arrange/Arrange.h"
#include "Follow/Follow.h"


namespace bv {


model::INodeLogicPtr        NodeLogicFactory::CreateLogic  ( const IDeserializer & deser, model::BasicNodeWeakPtr logicParent )
{
    const std::string & logicType = deser.GetAttribute( "type" );

    if( logicType == nodelogic::Scroller::Type() )
    {
        return nodelogic::Scroller::Create( deser, logicParent );
    }
    else if( logicType == nodelogic::WidgetCounter::Type() )
    {
        return nodelogic::WidgetCounter::Create( deser, logicParent );
    }
    else if( logicType == nodelogic::NodeReplicator::Type() )
    {
        return nodelogic::NodeReplicator::Create( deser, logicParent );
    }
    else if( logicType == nodelogic::SmoothValueSetter::Type() )
    {
        return nodelogic::SmoothValueSetter::Create( deser, logicParent );
    }
    else if( logicType == nodelogic::MeshLoader::Type() )
    {
        return nodelogic::MeshLoader::Create( deser, logicParent );
    }
    else if( logicType == nodelogic::SmoothValueSetter::Type() )
    {
        return nodelogic::SmoothValueSetter::Create( deser, logicParent );
    }
    else if( logicType == nodelogic::PieChart::Type() )
    {
        return nodelogic::PieChart::Create( deser, logicParent );
    }
    else if( logicType == nodelogic::AnalogWatch::Type() )
    {
        return nodelogic::AnalogWatch::Create( deser, logicParent );
    }
    else if( logicType == nodelogic::Arrange::Type() )
    {
        return nodelogic::Arrange::Create( deser, logicParent );
    }
    else if( logicType == nodelogic::Follow::Type() )
    {
        return nodelogic::Follow::Create( deser, logicParent );
    }

    return nullptr;
}

} //bv