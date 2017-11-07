#include "stdafx.h"

#include "NodeLogicHelper.h"
#include "Engine/Types/Values/ValuesFactory.h"
#include "Engine/Models/Plugins/Parameters/ParameterSerialization.h"


#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Tools/StringHeplers.h"

namespace bv {
namespace SerializationHelper
{


// ========================================================================= //
// IValue serialization
// ========================================================================= //
// In general values shouldn't be serialize. There're few cases, in which 
// serialization is needed ( NodeReplicator NodeLogic )
// Thats the main reason, why it wasn't done by inheiting from ISerializable.


// ***********************
//
IValuePtr CreateValue( const IDeserializer & deser, const std::string & name )
{
    auto valType = Convert::String2T( deser.GetAttribute( "type" ), ParamType::PT_FLOAT1 );

    switch( valType )
    {
    case ParamType::PT_FLOAT1:
    {
        auto iValuePtr = ValuesFactory::CreateValueFloat( name );
        float value = Convert::String2T( deser.GetAttribute( "value" ), 0.f );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    case ParamType::PT_FLOAT2:
    {
        auto iValuePtr = ValuesFactory::CreateValueVec2( name );
        glm::vec2 value = Convert::String2T( deser.GetAttribute( "value" ), glm::vec2( 0.f ) );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    case ParamType::PT_FLOAT3:
    {
        auto iValuePtr = ValuesFactory::CreateValueVec3( name );
        glm::vec3 value = Convert::String2T( deser.GetAttribute( "value" ), glm::vec3( 0.f ) );
        iValuePtr->SetValue( value );
        return iValuePtr;

    }
    case ParamType::PT_FLOAT4:
    {
        auto iValuePtr = ValuesFactory::CreateValueVec4( name );
        glm::vec4 value = Convert::String2T( deser.GetAttribute( "value" ), glm::vec4( 0.f ) );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    case ParamType::PT_INT:
    {
        auto iValuePtr = ValuesFactory::CreateValueInt( name );
        int value = Convert::String2T( deser.GetAttribute( "value" ), 0 );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    case ParamType::PT_MAT2:
    {
        assert( !"Implement" );
        auto iValuePtr = ValuesFactory::CreateValueMat2( name );
        return iValuePtr;
    }
    case ParamType::PT_MAT4:
    {
        assert( !"Implement" );
        auto iValuePtr = ValuesFactory::CreateValueMat4( name );
        return iValuePtr;
    }
    case ParamType::PT_STRING:
    {
        auto iValuePtr = ValuesFactory::CreateValueString( name );
        std::string value = deser.GetAttribute( "value" );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    case ParamType::PT_WSTRING:
    {
        auto iValuePtr = ValuesFactory::CreateValueWString( name );
        std::wstring value = StringToWString( deser.GetAttribute( "value" ) );
        iValuePtr->SetValue( value );
        return iValuePtr;
    }
    default:
        assert( !"Implement" );
        return nullptr;
    }
}


// ***********************
//
void                SerializeValue                      ( ISerializer & ser, IValueConstPtr val )
{
    auto valType = val->GetType();
    ser.SetAttribute( "type", Convert::T2String( valType ) );

    switch( valType )
    {
    case ParamType::PT_FLOAT1:
    {
        auto iValuePtr = QueryTypedValue< ValueFloatPtr> ( val );
        float value = iValuePtr->GetValue();
        ser.SetAttribute( "value", Convert::T2String( value ) );
        break;
    }
    case ParamType::PT_FLOAT2:
    {
        auto iValuePtr = QueryTypedValue< ValueVec2Ptr> ( val );
        glm::vec2 value = iValuePtr->GetValue();
        ser.SetAttribute( "value", Convert::T2String( value ) );
        break;
    }
    case ParamType::PT_FLOAT3:
    {
        auto iValuePtr = QueryTypedValue< ValueVec3Ptr> ( val );
        glm::vec3 value = iValuePtr->GetValue();
        ser.SetAttribute( "value", Convert::T2String( value ) );
        break;
    }
    case ParamType::PT_FLOAT4:
    {
        auto iValuePtr = QueryTypedValue< ValueVec4Ptr> ( val );
        glm::vec4 value = iValuePtr->GetValue();
        ser.SetAttribute( "value", Convert::T2String( value ) );
        break;
    }
    case ParamType::PT_INT:
    {
        auto iValuePtr = QueryTypedValue< ValueIntPtr> ( val );
        int value = iValuePtr->GetValue();
        ser.SetAttribute( "value", Convert::T2String( value ) );
        break;
    }
    case ParamType::PT_MAT2:
    {
        assert( !"Implement" );
        break;
    }
    case ParamType::PT_MAT4:
    {
        assert( !"Implement" );
        break;
    }
    case ParamType::PT_STRING:
    {
        auto iValuePtr = QueryTypedValue< ValueStringPtr> ( val );
        std::string value = iValuePtr->GetValue();
        ser.SetAttribute( "value", value );
        break;
    }
    case ParamType::PT_WSTRING:
    {
        auto iValuePtr = QueryTypedValue< ValueWStringPtr> ( val );
        std::wstring value = iValuePtr->GetValue();
        ser.SetAttribute( "value", Convert::T2String( value ) );
        break;
    }
    default:
        assert( !"Implement" );
    }

}

} // SerializationHelper

} // bv

