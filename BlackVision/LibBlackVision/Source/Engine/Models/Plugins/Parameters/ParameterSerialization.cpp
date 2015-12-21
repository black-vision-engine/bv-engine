#include "AbstractModelParameter.h"
#include "ParametersFactory.h"
#include "SimpleTypedParameters.h"
#include <sstream>

#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Serialization/BVDeserializeContext.h"

//#include "Serialization/SerializationObjects.inl"
#include "Serialization/SerializationHelper.h"
//#include "Serialization/SerializationObjects.h"
//#include "Serialization/SerializationObjects.inl"

namespace bv { 
    
namespace SerializationHelper {

std::pair< bv::ModelParamType, const char* > mpt2s[] = {
    std::make_pair( bv::ModelParamType::MPT_BOOL, "bool" ),
    std::make_pair( bv::ModelParamType::MPT_ENUM, "enum" ),
    std::make_pair( bv::ModelParamType::MPT_FLOAT, "float" ),
    std::make_pair( bv::ModelParamType::MPT_INT, "int" ),
    std::make_pair( bv::ModelParamType::MPT_MAT2, "mat2" ),
    std::make_pair( bv::ModelParamType::MPT_STRING, "string" ),
    std::make_pair( bv::ModelParamType::MPT_TRANSFORM, "transform" ),
    std::make_pair( bv::ModelParamType::MPT_TRANSFORM_VEC, "transform_vec" ),
    std::make_pair( bv::ModelParamType::MPT_VEC2, "vec2" ),
    std::make_pair( bv::ModelParamType::MPT_VEC3, "vec3" ),
    std::make_pair( bv::ModelParamType::MPT_VEC4, "vec4" ),
    std::make_pair( bv::ModelParamType::MPT_WSTRING, "wstring" )
};

template<>
std::string T2String< bv::ModelParamType>( const bv::ModelParamType& t )
{
    return SerializationHelper::Enum2String< bv::ModelParamType >( mpt2s, t );
}


}

    
namespace model {


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

	ITimeEvaluatorPtr sceneTimeline = dynamic_cast< BVDeserializeContext* >( deser.GetDeserializeContext() )->m_sceneTimeline;
	if( sceneTimeline == nullptr )
		sceneTimeline = TimelineManager::GetInstance()->GetRootTimeline();
    ITimeEvaluatorPtr te = TimelineHelper::GetTimeEvaluator( timeline, sceneTimeline );
    if( te == nullptr ) 
    {
        assert( false );
        te = sceneTimeline;
    }

    if( type == "float" )
    {
        auto param = ParametersFactory::CreateParameterFloat( name, te );

        auto interpolator = SerializationHelper::DeserializeObjectLoadImpl< FloatInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "2" || type == "vec2" )
    {
        auto param = ParametersFactory::CreateParameterVec2( name, te );
        
        auto interpolator = SerializationHelper::DeserializeObjectLoadImpl< Vec2Interpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "3" || type == "vec3" )
    {
        auto param = ParametersFactory::CreateParameterVec3( name, te );
        
        auto interpolator = SerializationHelper::DeserializeObjectLoadImpl< Vec3Interpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "4" || type == "vec4" )
    {
        auto param = ParametersFactory::CreateParameterVec4( name, te );
        
        auto interpolator = SerializationHelper::DeserializeObjectLoadImpl< Vec4Interpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "transform" )
    {
        auto param = ParametersFactory::CreateParameterTransform( name, te );

        auto transform = SerializationHelper::DeserializeObjectLoadImpl< TransformF >( deser, "composite_transform" );

        param->Transform() = *transform;

        return param;

    }
    else if( type == "6" || type == "transform_vec" ) // FIXME: this should be made a constant somewhere
    {
        auto param = ParametersFactory::CreateParameterTransformVec( name, te );

        //auto transes = SerializationHelper::DeserializeObjectLoadArrayImpl< TransformF >( deser, "composite_transform", "transform" );
        auto transes = SerializationHelper::DeserializeObjectLoadPropertiesImpl< TransformF >( deser, "composite_transform" );

        int i = 0;
        for( auto trans : transes )
        {
            param->InsertTransform( i++, *trans.get() ); // FIXME (?)
        }
        
        return param;
    } 
    else if( type == "7" || type == "int" )
    {
        auto param = ParametersFactory::CreateTypedSimpleParameter< ParamInt >( name, te );
        
        auto interpolator = SerializationHelper::DeserializeObjectLoadImpl< IntInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "bool" )
    {
        auto param = ParametersFactory::CreateTypedSimpleParameter< ParamBool >( name, te );

        auto interpolator = SerializationHelper::DeserializeObjectLoadImpl< BoolInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    } 
    else if( type == "9" || type == "enum" )
    {
        auto param = ParametersFactory::CreateParameterEnum< int >( name, te );

        auto interpolator = SerializationHelper::DeserializeObjectLoadImpl< IntInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    }
    else if( type == "wstring" ) 
    {
        auto param = ParametersFactory::CreateParameterWString( name, te );

        auto interpolator = SerializationHelper::DeserializeObjectLoadImpl< WStringInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;

    }
    else if( type == "string" ) 
    {
        auto param = ParametersFactory::CreateParameterString( name, te );

        auto interpolator = SerializationHelper::DeserializeObjectLoadImpl< StringInterpolator >( deser, "interpolator" );

        param->AccessInterpolator() = *interpolator;

        return param;
    }

    assert( false ); // FIXME
    return ParametersFactory::CreateParameterBool( name, te );
}

} }