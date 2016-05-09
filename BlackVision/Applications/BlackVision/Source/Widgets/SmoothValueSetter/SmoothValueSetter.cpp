#include "SmoothValueSetter.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Widgets/NodeLogicHelper.h"


namespace bv { namespace nodelogic
{

const std::string       SmoothValueSetter::m_type = "SmoothValueSetter";


const std::string       SmoothValueSetter::ACTION::ADD_PARAMETER_BINDING            = "AddParamBinding";
const std::string       SmoothValueSetter::ACTION::REMOVE_PARAMETER_BINDING         = "RemoveParamBinding";

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


// ========================================================================= //
// Helpers
// ========================================================================= //

// ***********************
//
const ParameterBinding *        SmoothValueSetter::FindSource      ( const std::string & bindingSource )
{
    for( auto & binding : m_paramBindings )
    {
        if( binding.SourceName == bindingSource )
            return &binding;
    }

    return nullptr;
}

// ***********************
//
ParameterBinding                SmoothValueSetter::TargetBindingData       ( IDeserializer & eventDeser, ISerializer & response )
{
    auto nodePath       = eventDeser.GetAttribute( "NodeName" );
    auto pluginName     = eventDeser.GetAttribute( "PluginName" );
    auto paramName      = eventDeser.GetAttribute( "ParamName" );

    auto node = m_parentNode->GetNode( nodePath );
    if( node == nullptr )
    {
        response.SetAttribute( "ErrorInfo", "Node: " + nodePath + " not found." );
        return ParameterBinding();
    }

    auto plugin = node->GetPlugin( pluginName );
    if( plugin == nullptr )
    {
        response.SetAttribute( "ErrorInfo", "Plugin: " + pluginName + " not found." );
        return ParameterBinding();
    }

    auto param = plugin->GetParameter( paramName );
    if( param == nullptr )
    {
        response.SetAttribute( "ErrorInfo", "Parameter: " + paramName + " not found." );
        return ParameterBinding();
    }

    ParameterBinding newBinding;
    newBinding.Node = std::static_pointer_cast< model::BasicNode >( node );
    newBinding.Plugin = plugin;
    newBinding.Parameter = param;    

    return newBinding;
}

// ***********************
//
model::IParameterPtr            SmoothValueSetter::CreateSrcParameter      ( ModelParamType type, const std::string & name )
{
    switch( type )
    {
        case ModelParamType::MPT_FLOAT:
        {
            auto param = AddFloatParam( m_paramValModel, m_timeEval, name, 0.0f );
            return param->Parameter();
        }
        case ModelParamType::MPT_BOOL:
        {
            auto param = AddBoolParam( m_paramValModel, m_timeEval, name, false );
            return param->Parameter();
        }
    }

    return nullptr;
}

// ***********************
//
SmoothValueSetter::SmoothValueSetter( bv::model::BasicNodePtr parent, model::ITimeEvaluatorPtr timeEvaluator )
    :   m_parentNode( parent )
    ,   m_timeEval( timeEvaluator )
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

// ========================================================================= //
// Serialization
// ========================================================================= //

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

// ========================================================================= //
// Handling events
// ========================================================================= //

// ***********************
//
bool                    SmoothValueSetter::HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    if( action == SmoothValueSetter::ACTION::ADD_PARAMETER_BINDING )
    {
        AddBinding( eventDeser, response, editor );
    }
    else if( action == SmoothValueSetter::ACTION::REMOVE_PARAMETER_BINDING )
    {


    }

    return false;
}

// ***********************
//
void                    SmoothValueSetter::AddBinding      ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * /*editor*/ )
{
    ParameterBinding newBinding = TargetBindingData( eventDeser, response );
    if( newBinding.Parameter == nullptr )
        return;
 
    newBinding.SourceName   = eventDeser.GetAttribute( "SourceName" );
    ModelParamType type     = SerializationHelper::String2T( eventDeser.GetAttribute( "SourceParamType" ), ModelParamType::MPT_TOTAL );


    auto existingSource = FindSource( newBinding.SourceName );
    if( existingSource == nullptr )
    {
        auto newParam = CreateSrcParameter( type, newBinding.SourceName );
        newBinding.Parameter = newParam;
    }
    else
    {
        if( existingSource->ValueSrc->GetType() != type )
        {
            response.SetAttribute( "ErrorInfo", "Source type: " + SerializationHelper::T2String( type ) + " is different then existing source type: " + SerializationHelper::T2String< bv::ModelParamType >( existingSource->ValueSrc->GetType() ) );
            return;
        }

        newBinding.ValueSrc = existingSource->ValueSrc;
    }

    m_paramBindings.push_back( newBinding );
}


}   // nodelogic
}	// bv

