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
        UpdateParameter( binding.ValueSrc, binding.Parameter, binding.TransformKind, binding.Component );
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
    
    if( binding.TransformKind != TransformKind::invalid )
        ser.SetAttribute( "Kind", SerializationHelper::T2String( binding.TransformKind ) );
    if( binding.Component != ParameterBinding::VectorComponent::Invalid )
        ser.SetAttribute( "Component", SerializationHelper::T2String( binding.Component ) );

    ser.ExitChild();    // binding
}

// ***********************
//
bool                    SmoothValueSetter::DeserializeBinding      ( const IDeserializer & deser )
{
    auto nodePath       = deser.GetAttribute( "NodePath" );
    auto pluginName     = deser.GetAttribute( "PluginName" );
    auto paramName      = deser.GetAttribute( "ParamName" );
    auto component      = SerializationHelper::String2T( deser.GetAttribute( "Component" ), ParameterBinding::VectorComponent::Invalid );
    auto transformKind  = SerializationHelper::String2T( deser.GetAttribute( "Kind" ), TransformKind::invalid );

    auto sourceName     = deser.GetAttribute( "SourceName" );
    auto sourceType     = SerializationHelper::String2T( deser.GetAttribute( "SourceParamType" ), ModelParamType::MPT_TOTAL );

    ParameterBinding newBinding = FillTargetData( nodePath, pluginName, paramName, transformKind, component );
    if( newBinding.Parameter == nullptr )
        return false;

    CreateAndAddSourceData( newBinding, sourceName, sourceType );
    return true;
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
bool                    SmoothValueSetter::AddBinding      ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    return DeserializeBinding( eventDeser );
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
template< typename VectorType >
bool        HasComponent                ( ParameterBinding::VectorComponent component )
{    return false;  }

template<>
bool        HasComponent< glm::vec2 >   ( ParameterBinding::VectorComponent component )
{   return (int)component < 2;  }

template<>
bool        HasComponent< glm::vec3 >   ( ParameterBinding::VectorComponent component )
{   return (int)component < 3;  }

template<>
bool        HasComponent< glm::vec4 >   ( ParameterBinding::VectorComponent component )
{   return (int)component < 4;  }

// ***********************
//
template< typename VectorType >
void        SetComponent            ( VectorType & vector, float value, ParameterBinding::VectorComponent component )
{
    assert( HasComponent< VectorType >( component ) );
    vector[ static_cast< int >( component ) ] = value; 
}

// ***********************
//
template< typename VectorType >
void        GetComponent            ( VectorType & vector, ParameterBinding::VectorComponent component )
{
    assert( HasComponent< VectorType >( component ) );
    return vector[ component ];
}

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
ParameterBinding                SmoothValueSetter::FillTargetData          ( const std::string & nodePath, const std::string & pluginName, const std::string & paramName, TransformKind transformKind, ParameterBinding::VectorComponent component )
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
    newBinding.Component = component;
    newBinding.TransformKind = transformKind;

    return newBinding;
}

// ***********************
//
void                            SmoothValueSetter::CreateAndAddSourceData   ( ParameterBinding & srcBindingData, const std::string & sourceName, ModelParamType type )
{
    auto existingSource = FindSource( sourceName );
    if( existingSource == nullptr )
    {
        auto newParam = CreateSrcParameter( type, sourceName );
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
IValuePtr                       SmoothValueSetter::CreateSrcParameter      ( ModelParamType type, const std::string & name )
{
    switch( type )
    {
        case ModelParamType::MPT_FLOAT:
        {
            auto paramEval = AddFloatParam( m_paramValModel, m_timeEval, name, 0.0f );
            return paramEval->Value();
        }
        case ModelParamType::MPT_VEC2:
        {
            auto paramEval = AddVec2Param( m_paramValModel, m_timeEval, name, glm::vec2( 0.0f, 0.0f ) );
            return paramEval->Value();
        }
        case ModelParamType::MPT_VEC3:
        {
            auto paramEval = AddVec3Param( m_paramValModel, m_timeEval, name, glm::vec3( 0.0f, 0.0f, 0.0f ) );
            return paramEval->Value();
        }
        case ModelParamType::MPT_VEC4:
        {
            auto paramEval = AddVec4Param( m_paramValModel, m_timeEval, name, glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
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
template< typename Param >
void        UpdateParam         ( ValueFloatPtr paramSource, Param boundParam, ParameterBinding::VectorComponent component )
{
    auto value = boundParam->Evaluate();
    SetComponent( value, paramSource->GetValue(), component );
    boundParam->SetVal( value, TimeType( 0.0f ) );
}

// ***********************
//
void        UpdateParam         ( ValueFloatPtr paramSource, model::ParamTransformPtr boundParam, TransformKind kind, ParameterBinding::VectorComponent component )
{
    auto & transform = boundParam->Transform();
    auto time = boundParam->GetLocalEvaluationTime();

    if( kind == TransformKind::translation )
    {
        glm::vec3 trans = transform.GetTranslation( time );
        SetComponent( trans, paramSource->GetValue(), component );
        transform.SetTranslation( trans, time );
    }
    else if( kind == TransformKind::rotation )
    {
        glm::vec3 trans = transform.GetRotation( time );
        SetComponent( trans, paramSource->GetValue(), component );
        transform.SetRotation( trans, time );
    }
    else if( kind == TransformKind::scale )
    {
        glm::vec3 trans = transform.GetScale( time );
        SetComponent( trans, paramSource->GetValue(), component );
        transform.SetScale( trans, time );
    }
    else if( kind == TransformKind::center )
    {
        glm::vec3 trans = transform.GetCenter( time );
        SetComponent( trans, paramSource->GetValue(), component );
        transform.SetCenter( trans, time );
    }
}

// ***********************
//
void                            SmoothValueSetter::UpdateParameter         ( IValuePtr & sourceParam, model::IParameterPtr & boundParam, TransformKind kind, ParameterBinding::VectorComponent component )
{
    if( sourceParam->GetType() == ParamType::PT_FLOAT1 && boundParam->GetType() == ModelParamType::MPT_FLOAT )
    {
        UpdateParam( QueryTypedValue< ValueFloatPtr >( sourceParam ), model::QueryTypedParam< model::ParamFloatPtr >( boundParam ) );
    }
    else if( sourceParam->GetType() == ParamType::PT_FLOAT1 && boundParam->GetType() == ModelParamType::MPT_VEC3 )
    {
        UpdateParam( QueryTypedValue< ValueFloatPtr >( sourceParam ), model::QueryTypedParam< model::ParamVec3Ptr >( boundParam ), component );
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
    else if( sourceParam->GetType() == ParamType::PT_FLOAT1 && boundParam->GetType() == ModelParamType::MPT_TRANSFORM )
    {
        UpdateParam( QueryTypedValue< ValueFloatPtr >( sourceParam ), model::QueryTypedParam< model::ParamTransformPtr >( boundParam ), kind, component );
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

