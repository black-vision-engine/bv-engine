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
const std::string       SmoothValueSetter::ACTION::SET_PARAMETER                    = "SetParameter";

const std::string       SmoothValueSetter::PARAMETERS::SMOOTH_TIME                  = "SmoothTime";

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
void                    SmoothValueSetter::Update			( TimeType time )
{
    NodeLogicBase::Update( time );

    for( auto & binding : m_paramBindings )
    {
        UpdateParameter( binding.ValueSrc, binding.Parameter );
    }
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
            NodeLogicBase::Serialize( ser );    // Serialize parameters.

            ser.EnterArray( "bindings" );
            for( auto & binding : m_paramBindings )
            {
                SerializeBinding( ser, binding );
            }
            ser.ExitChild();    // bindings
        }

    ser.ExitChild();    // logic
}

// ***********************
//
void                    SmoothValueSetter::Deserialize     ( const IDeserializer & deser )
{
    if( deser.EnterChild( "bindings" ) )
    {
        if( deser.EnterChild( "binding" ) )
        {
            do
            {
                DeserializeBinding( deser );

            } while( deser.NextChild() );
            deser.ExitChild();  // binding
        }
        deser.ExitChild();  // bindings
    }

    NodeLogicBase::Deserialize( deser );    // Deserialize parameters.
}

// ***********************
//
SmoothValueSetterPtr    SmoothValueSetter::Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto smoothValueSetter = std::make_shared< SmoothValueSetter >( parentNode, timeline );

    smoothValueSetter->Deserialize( deser );

    return smoothValueSetter;
}

// ***********************
//
void                    SmoothValueSetter::SerializeBinding        ( ISerializer & ser, const ParameterBinding & binding ) const
{
    ser.EnterChild( "binding" );

    ser.SetAttribute( "NodePath", binding.Node );
    ser.SetAttribute( "PluginName", binding.Plugin );
    ser.SetAttribute( "ParamName", binding.Parameter->GetName() );
    ser.SetAttribute( "SourceName", binding.ValueSrc->GetName() );
    ser.SetAttribute( "SourceParamType", SerializationHelper::T2String( GetParameter( binding.ValueSrc->GetName() )->GetType() ) );

    ser.ExitChild();    // binding
}

// ***********************
//
void                    SmoothValueSetter::DeserializeBinding      ( const IDeserializer & deser )
{
    auto nodePath       = deser.GetAttribute( "NodePath" );
    auto pluginName     = deser.GetAttribute( "PluginName" );
    auto paramName      = deser.GetAttribute( "ParamName" );
    auto sourceName     = deser.GetAttribute( "SourceName" );
    auto sourceType     = SerializationHelper::String2T( deser.GetAttribute( "SourceParamType" ), ModelParamType::MPT_TOTAL );

    ParameterBinding newBinding = FillTargetData( nodePath, pluginName, paramName );
    CreateAndAddSourceData( newBinding, sourceName, sourceType );
}

// ========================================================================= //
// Handling events
// ========================================================================= //

// ***********************
//
bool                    SmoothValueSetter::HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    if( action == SmoothValueSetter::ACTION::SET_PARAMETER )
    {
        return SetParameter( eventDeser, response, editor );
    }
    else if( action == SmoothValueSetter::ACTION::ADD_PARAMETER_BINDING )
    {
        return AddBinding( eventDeser, response, editor );
    }
    else if( action == SmoothValueSetter::ACTION::REMOVE_PARAMETER_BINDING )
    {
        return RemoveBinding( eventDeser, response, editor );
    }

    return false;
}

// ***********************
//
bool                    SmoothValueSetter::AddBinding      ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * /*editor*/ )
{
    ParameterBinding newBinding = TargetBindingData( eventDeser, response );
    if( newBinding.Parameter == nullptr )
        return false;

    auto sourceName     = eventDeser.GetAttribute( "SourceName" );
    ModelParamType type = SerializationHelper::String2T( eventDeser.GetAttribute( "SourceParamType" ), ModelParamType::MPT_TOTAL );

    CreateAndAddSourceData( newBinding, sourceName, type );

    return true;
}

// ***********************
//
bool                    SmoothValueSetter::RemoveBinding   ( IDeserializer & /*eventDeser*/, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    return false;
}


// ***********************
//
bool                    SmoothValueSetter::SetParameter    ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * /*editor*/ )
{
    auto srcParamName = eventDeser.GetAttribute( "SourceName" );
    auto paramValue = eventDeser.GetAttribute( "Value" );

    // FIXME: @todo This code shouldn't allow to change SmoothValueSetter parameters, for example SmoothTime.
    auto param = GetParameter( srcParamName );
    if( !param )
    {
        response.SetAttribute( "ErrorInfo", "Parameter [" + srcParamName + "] doesn't exists or it is build in SmoothValueSetter parameter and can't be changed." );
        return false;
    }

    auto smoothTimeParam = GetParameter( srcParamName + "_" + PARAMETERS::SMOOTH_TIME );
    assert( smoothTimeParam );

    auto deltaTime = model::QueryTypedParam< model::ParamFloatPtr >( smoothTimeParam )->Evaluate();

    if( param->GetType() == ModelParamType::MPT_FLOAT )
    {
        auto typedeParam = model::QueryTypedParam< model::ParamFloatPtr >( param );
        return SetSmoothParam( typedeParam, deltaTime, response, srcParamName, paramValue );
    }
    else if( param->GetType() == ModelParamType::MPT_VEC2 )
    {
        auto typedeParam = model::QueryTypedParam< model::ParamVec2Ptr >( param );
        return SetSmoothParam( typedeParam, deltaTime, response, srcParamName, paramValue );
    }
    else if( param->GetType() == ModelParamType::MPT_VEC3 )
    {
        auto typedeParam = model::QueryTypedParam< model::ParamVec3Ptr >( param );
        return SetSmoothParam( typedeParam, deltaTime, response, srcParamName, paramValue );
    }
    else if( param->GetType() == ModelParamType::MPT_VEC4 )
    {
        auto typedeParam = model::QueryTypedParam< model::ParamVec4Ptr >( param );
        return SetSmoothParam( typedeParam, deltaTime, response, srcParamName, paramValue );
    }

    response.SetAttribute( "ErrorInfo", "Parameter type [" + SerializationHelper::T2String( param->GetType() ) + "] not supported" );
    return false;
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
        if( binding.ValueSrc->GetName() == bindingSource )
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

    ParameterBinding newBinding = FillTargetData( nodePath, pluginName, paramName );

    if( newBinding.Parameter == nullptr )
    {
        response.SetAttribute( "ErrorInfo", "Node: " + nodePath + "] Plugin: [" + pluginName + "] Parameter: [" + paramName + " not found." );
    }

    return newBinding;
}

// ***********************
//
ParameterBinding                SmoothValueSetter::FillTargetData          ( const std::string & nodePath, const std::string & pluginName, const std::string & paramName )
{
    auto node = m_parentNode->GetNode( nodePath );
    if( node == nullptr )
    {
        return ParameterBinding();
    }

    auto plugin = node->GetPlugin( pluginName );
    if( plugin == nullptr )
    {
        return ParameterBinding();
    }

    auto param = plugin->GetParameter( paramName );
    if( param == nullptr )
    {
        return ParameterBinding();
    }

    ParameterBinding newBinding;
    newBinding.Node = nodePath;
    newBinding.Plugin = pluginName;
    newBinding.Parameter = param;

    return newBinding;
}

// ***********************
//
void                            SmoothValueSetter::CreateAndAddSourceData   ( ParameterBinding & srcBindingData, const std::string & sourceName, ModelParamType type )
{
    auto existingSource = FindSource( sourceName );
    if( existingSource == nullptr )
    {
        auto param = srcBindingData.Parameter;
        auto newParam = CreateSrcParameter( type, sourceName, param->EvaluateToString( param->GetTimeEvaluator()->GetLocalTime() ) );
        srcBindingData.ValueSrc = newParam;
        AddFloatParam( m_paramValModel, m_timeEval, sourceName + "_" + PARAMETERS::SMOOTH_TIME, 2.0f );
    }
    else
    {
        //if( GetParameter( existingSource->ValueSrc->GetName() )->GetType() != type )
        //{
        //    response.SetAttribute( "ErrorInfo", "Source type: " + SerializationHelper::T2String( type ) + " is different then existing source type: " + SerializationHelper::T2String< bv::ParamType >( existingSource->ValueSrc->GetType() ) );
        //    return;
        //}

        srcBindingData.ValueSrc = existingSource->ValueSrc;
    }

    m_paramBindings.push_back( std::move( srcBindingData ) );
}

// ***********************
//
IValuePtr                       SmoothValueSetter::CreateSrcParameter      ( ModelParamType type, const std::string & name, const std::string & initValue )
{
    switch( type )
    {
        case ModelParamType::MPT_FLOAT:
        {
            auto paramEval = AddFloatParam( m_paramValModel, m_timeEval, name, SerializationHelper::String2T( initValue, 0.0f ) );
            return paramEval->Value();
        }
        case ModelParamType::MPT_BOOL:
        {
            auto paramEval = AddBoolParam( m_paramValModel, m_timeEval, name, SerializationHelper::String2T( initValue, false ) );
            return paramEval->Value();
        }
        case ModelParamType::MPT_VEC2:
        {
            auto paramEval = AddVec2Param( m_paramValModel, m_timeEval, name, SerializationHelper::String2T( initValue, glm::vec2( 0.0f, 0.0f ) ) );
            return paramEval->Value();
        }
        case ModelParamType::MPT_VEC3:
        {
            auto paramEval = AddVec3Param( m_paramValModel, m_timeEval, name, SerializationHelper::String2T( initValue, glm::vec3( 0.0f, 0.0f, 0.0f ) ) );
            return paramEval->Value();
        }
        case ModelParamType::MPT_VEC4:
        {
            auto paramEval = AddVec4Param( m_paramValModel, m_timeEval, name, SerializationHelper::String2T( initValue, glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f ) ) );
            return paramEval->Value();
        }
    }

    return nullptr;
}


// ***********************
//
template< typename Value, typename Param >
void        UpdateParam         ( Value paramSource, Param boundParam )
{
    boundParam->SetVal( paramSource->GetValue(), 0.0f );
}

// ***********************
//
void                            SmoothValueSetter::UpdateParameter         ( IValuePtr sourceParam, model::IParameterPtr boundParam )
{
    if( sourceParam->GetType() == ParamType::PT_FLOAT1 && boundParam->GetType() == ModelParamType::MPT_FLOAT )
    {
        UpdateParam( QueryTypedValue< ValueFloatPtr >( sourceParam ), model::QueryTypedParam< model::ParamFloatPtr >( boundParam ) );
    }
    else if( sourceParam->GetType() == ParamType::PT_FLOAT2 && boundParam->GetType() == ModelParamType::MPT_VEC2 )
    {
        UpdateParam( QueryTypedValue< ValueVec2Ptr >( sourceParam ), model::QueryTypedParam< model::ParamVec2Ptr >( boundParam ) );
    }
    else if( sourceParam->GetType() == ParamType::PT_FLOAT3 && boundParam->GetType() == ModelParamType::MPT_VEC3 )
    {
        UpdateParam( QueryTypedValue< ValueVec3Ptr >( sourceParam ), model::QueryTypedParam< model::ParamVec3Ptr >( boundParam ) );
    }
    else if( sourceParam->GetType() == ParamType::PT_FLOAT4 && boundParam->GetType() == ModelParamType::MPT_VEC4 )
    {
        UpdateParam( QueryTypedValue< ValueVec3Ptr >( sourceParam ), model::QueryTypedParam< model::ParamVec3Ptr >( boundParam ) );
    }
}

// ***********************
//
template< typename InterpolatorType, typename ValType, ModelParamType type >
bool                    SmoothValueSetter::SetSmoothParam      ( std::shared_ptr< model::SimpleParameterImpl< InterpolatorType, ValType, type > > & param, float deltaTime, ISerializer & response, const std::string & srcParamName, const std::string & paramValue )
{
    Expected< ValType > newValue = SerializationHelper::String2T< ValType >( paramValue );

    if( !newValue.isValid )
    {
        response.SetAttribute( "ErrorInfo", "Error parsing [" + srcParamName + "]. Value [" + paramValue + "] is invalid " + SerializationHelper::T2String( param->GetType() )  );
        return false;
    }

    // Add key with current time and current value.
    auto currentVal = param->Evaluate();
    auto currentTime = param->GetLocalEvaluationTime();

    param->SetVal( currentVal, currentTime );

    // Remove all keys after current key.
    auto interpolator = param->AccessInterpolator();
    auto keys = interpolator.GetKeys();

    if( keys.size() > 1 )
    {
        SizeType keyIdx = 0;
        for( ; keyIdx < keys.size(); keyIdx++ )
        {
            if( keys[ keyIdx ].t > currentTime )
                break;
        }

        keys.erase( keys.begin() + keyIdx, keys.end() ); 
    }

    // Add new value key. Smoothing lasts m_smoothTime.
    param->SetVal( newValue.ham, currentTime + deltaTime );
    
    return true;
}

}   // nodelogic
}	// bv

