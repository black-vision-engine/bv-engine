#include "stdafx.h"

#include "AbstractModelParameter.h"
#include "ParametersFactory.h"
#include "SimpleTypedParameters.h"
#include <sstream>

#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "ParameterSerialization.h"

#include "UseLoggerLibBlackVision.h"



#include "Memory/MemoryLeaks.h"



namespace bv {     
namespace Convert
{

// ***********************
//
std::pair< bv::ModelParamType, const char* > mpt2s[] = {
    std::make_pair( bv::ModelParamType::MPT_BOOL, "bool" ),
    std::make_pair( bv::ModelParamType::MPT_ENUM, "enum" ),
    std::make_pair( bv::ModelParamType::MPT_FLOAT, "float" ),
    std::make_pair( bv::ModelParamType::MPT_INT, "int" ),
    std::make_pair( bv::ModelParamType::MPT_MAT2, "mat2" ),
    std::make_pair( bv::ModelParamType::MPT_STRING, "string" ),
    std::make_pair( bv::ModelParamType::MPT_TRANSFORM, "transform" ),
    std::make_pair( bv::ModelParamType::MPT_VEC2, "vec2" ),
    std::make_pair( bv::ModelParamType::MPT_VEC3, "vec3" ),
    std::make_pair( bv::ModelParamType::MPT_VEC4, "vec4" ),
    std::make_pair( bv::ModelParamType::MPT_WSTRING, "wstring" ),
    std::make_pair( bv::ModelParamType::MPT_TOTAL, "" )
};

IMPLEMENT_ENUM_SERIALIZATION( bv::ModelParamType, mpt2s );


// ***********************
//
std::pair< bv::ParamType, const char* > ParamTypeStringsArray[] = {
    std::make_pair( bv::ParamType::PT_BOOL, "bool" ),
    std::make_pair( bv::ParamType::PT_ENUM, "enum" ),
    std::make_pair( bv::ParamType::PT_FLOAT1, "float" ),
    std::make_pair( bv::ParamType::PT_INT, "int" ),
    std::make_pair( bv::ParamType::PT_MAT2, "mat2" ),
    std::make_pair( bv::ParamType::PT_MAT3, "mat3" ),
    std::make_pair( bv::ParamType::PT_MAT4, "mat4" ),
    std::make_pair( bv::ParamType::PT_STRING, "string" ),
    std::make_pair( bv::ParamType::PT_FLOAT2, "vec2" ),
    std::make_pair( bv::ParamType::PT_FLOAT3, "vec3" ),
    std::make_pair( bv::ParamType::PT_FLOAT4, "vec4" ),
    std::make_pair( bv::ParamType::PT_WSTRING, "wstring" ),
    std::make_pair( bv::ParamType::PT_TOTAL, "" ),
};

IMPLEMENT_ENUM_SERIALIZATION( bv::ParamType, ParamTypeStringsArray );

}

    
namespace model
{


// ***********************
//
class KeyFrame : public ISerializable // FIXME: to remove!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
public:
    std::string time, value; // FIXME

    KeyFrame( std::string t, std::string v ) : time( t ), value( v ) {}

    virtual void                Serialize       ( ISerializer&/*doc*/ ) const {}
    static ISerializablePtr     Create          ( const IDeserializer&doc )
    {
        auto time = doc.GetAttribute( "time" );
        auto value = doc.GetAttribute( "val" );
        return std::make_shared< KeyFrame >( time, value );
    }
};


// ********************************************************************************************************************

//template< typename T >
//std::shared_ptr< T > DeserializeTypedParam( std::vector< KeyFrame >& values, std::string name, ITimeEvaluatorPtr te )
//{
//    auto param = ParametersFactory::CreateTypedSimpleParameter< T >( name, te );
//
//    for( auto value : values )
//    {
//        float val = 
//}

ISerializablePtr AbstractModelParameter::Create( const IDeserializer& deser ) // FIXME: rethink if is might be done cleaner
{
    auto name = deser.GetAttribute( "name" );
    auto type = deser.GetAttribute( "type" );
    auto timeline = deser.GetAttribute( "timeline" );

    auto bvDeserCo = Cast< BVDeserializeContext* >( deser.GetDeserializeContext() );

    auto te = bvDeserCo->GetTimeline( timeline, "param: " + name );
    assert( te );

    if( type == "float" )
    {
        auto param = ParametersFactory::CreateParameterFloat( name, te );

        auto interpolator = SerializationHelper::DeserializeObject< FloatInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "2" || type == "vec2" )
    {
        auto param = ParametersFactory::CreateParameterVec2( name, te );
        
        auto interpolator = SerializationHelper::DeserializeObject< Vec2Interpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "3" || type == "vec3" )
    {
        auto param = ParametersFactory::CreateParameterVec3( name, te );
        
        auto interpolator = SerializationHelper::DeserializeObject< Vec3Interpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "4" || type == "vec4" )
    {
        auto param = ParametersFactory::CreateParameterVec4( name, te );
        
        auto interpolator = SerializationHelper::DeserializeObject< Vec4Interpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "transform" || type == "transform_vec" /* maintain serialization backward compatibility  */ )
    {
        auto param = ParametersFactory::CreateParameterTransform( name, te );

        auto transform = SerializationHelper::DeserializeObject< CompositeTransform >( deser, "composite_transform" );

        param->Transform() = *transform;

        return param;

    }
    else if( type == "7" || type == "int" )
    {
        auto param = ParametersFactory::CreateTypedSimpleParameter< ParamInt >( name, te );
        
        auto interpolator = SerializationHelper::DeserializeObject< IntInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "bool" )
    {
        auto param = ParametersFactory::CreateTypedSimpleParameter< ParamBool >( name, te );

        auto interpolator = SerializationHelper::DeserializeObject< BoolInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "9" || type == "enum" )
    {
        auto param = ParametersFactory::CreateParameterEnum< int >( name, te );

        auto interpolator = SerializationHelper::DeserializeObject< IntInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    }
    else if( type == "wstring" ) 
    {
        auto param = ParametersFactory::CreateParameterWString( name, te );

        auto interpolator = SerializationHelper::DeserializeObject< WStringInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;

    }
    else if( type == "string" ) 
    {
        auto param = ParametersFactory::CreateParameterString( name, te );

        auto interpolator = SerializationHelper::DeserializeObject< StringInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    }

    assert( false ); // FIXME
    return ParametersFactory::CreateParameterBool( name, te );
}


} }