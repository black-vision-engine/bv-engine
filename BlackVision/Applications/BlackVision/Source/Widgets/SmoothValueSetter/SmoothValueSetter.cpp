#include "SmoothValueSetter.h"

#include "Engine/Models/BasicNode.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Widgets/NodeLogicHelper.h"


namespace bv { namespace nodelogic
{


const std::string       SmoothValueSetter::m_type = "SmoothValueSetter";

// ***********************
//
const std::string &     SmoothValueSetter::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     SmoothValueSetter::GetType             () const
{
    return Type();
}



// ***********************
//
SmoothValueSetter::SmoothValueSetter( bv::model::BasicNodePtr parent, model::ITimeEvaluatorPtr /*timeEvaluator*/ )
    : m_parentNode( parent )
{}

// ***********************
//
SmoothValueSetter::~SmoothValueSetter()
{}

// ***********************
//
void                    SmoothValueSetter::Update			( TimeType )
{

}

// ***********************
//
void                    SmoothValueSetter::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );
        ser.SetAttribute( "type", m_type );

        if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
        {

        }

    ser.ExitChild();    // logic
}

// ***********************
//
SmoothValueSetterPtr    SmoothValueSetter::Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );

    return std::make_shared< SmoothValueSetter >( parentNode, timeline );
}

// ***********************
//
bool                    SmoothValueSetter::HandleEvent     ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * editor )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    if( action == "AddParamBinding" )
    {
        auto nodePath = eventDeser.GetAttribute( "NodeName" );
        auto pluginName = eventDeser.GetAttribute( "PluginName" );
        auto paramName = eventDeser.GetAttribute( "ParamName" );
        auto srcName = eventDeser.GetAttribute( "SourceName" );

        AddBinding( nodePath, pluginName, paramName, srcName, editor );
    }
    else if( action == "RemoveParamBinding" )
    {


    }

    return false;
}


void                    SmoothValueSetter::AddBinding      ( const std::string & nodePath, const std::string & pluginName, const std::string & paramName, const std::string & srcName, BVProjectEditor * editor )
{
    //m_parentNode->
    nodePath;
    pluginName;
    paramName;
    srcName;
    editor;
}


}   // nodelogic
}	// bv

