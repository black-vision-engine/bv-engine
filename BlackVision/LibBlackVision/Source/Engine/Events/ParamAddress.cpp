#include "stdafx.h"
#include "ParamAddress.h"






#include "Memory/MemoryLeaks.h"



namespace bv
{

namespace SerializationHelper
{

std::pair< ParameterAddress::TargetType, const char* > TargetTypeMapping[] = 
    { std::make_pair( ParameterAddress::TargetType::GlobalEffectParam, "GlobalEffectParam" )
    , std::make_pair( ParameterAddress::TargetType::PluginParam, "PluginParam" )
    , std::make_pair( ParameterAddress::TargetType::ResourceParam, "ResourceParam" )
    , std::make_pair( ParameterAddress::TargetType::LightParam, "LightParam" )
    , std::make_pair( ParameterAddress::TargetType::NodeLogicParam, "NodeLogicParam" )
    , std::make_pair( ParameterAddress::TargetType::CameraParam, "CameraParam" )
    , std::make_pair( ParameterAddress::TargetType::FailTarget, SerializationHelper::EMPTY_STRING )      // default
};

template<> ParameterAddress::TargetType String2T            ( const std::string& s, const ParameterAddress::TargetType& defaultVal )       { return String2Enum( TargetTypeMapping, s, defaultVal ); }
template<> std::string                  T2String            ( const ParameterAddress::TargetType & t )                                     { return Enum2String( TargetTypeMapping, t ); }

}   // SerializationHelper

// ***********************
//
void                    ParameterAddress::Serialize       ( ISerializer & ser ) const
{
    ser.SetAttribute( SerializationHelper::PARAM_TARGET_TYPE_STRING, SerializationHelper::T2String( ParamTargetType ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodeName );
    ser.SetAttribute( SerializationHelper::PLUGIN_NAME_STRING, PluginName );
    ser.SetAttribute( SerializationHelper::PARAM_NAME_STRING, ParamName );
    ser.SetAttribute( SerializationHelper::PARAM_SUB_NAME_STRING, ParamSubName );

    if( ParamTargetType == ParameterAddress::TargetType::LightParam || ParamTargetType ==  ParameterAddress::TargetType::CameraParam )
        ser.SetAttribute( SerializationHelper::PARAM_INDEX, SerializationHelper::T2String( Index ) );
}

// ***********************
//
ParameterAddress        ParameterAddress::Create          ( const IDeserializer & deser )
{
    ParameterAddress address;

    address.SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING);
    address.PluginName        = deser.GetAttribute( SerializationHelper::PLUGIN_NAME_STRING );
    address.NodeName          = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
    address.ParamName         = deser.GetAttribute( SerializationHelper::PARAM_NAME_STRING );
    address.ParamSubName      = deser.GetAttribute( SerializationHelper::PARAM_SUB_NAME_STRING );
    address.Index             = SerializationHelper::String2T< UInt32 >( deser.GetAttribute( SerializationHelper::PARAM_INDEX ), std::numeric_limits< UInt32 >::quiet_NaN() );
    address.ParamTargetType   = SerializationHelper::String2T<ParameterAddress::TargetType>( deser.GetAttribute( SerializationHelper::PARAM_TARGET_TYPE_STRING ), ParameterAddress::TargetType::FailTarget );

    return address;
}

// ***********************
// Thanks to this operator you can use ParameterAddress as map key.
bool                 operator<       ( const ParameterAddress& address1, const ParameterAddress& address2 )
{
    if( address1.ParamTargetType < address2.ParamTargetType )
        return true;
    if( address1.SceneName < address2.SceneName )
        return true;
    if( address1.ParamName < address2.ParamName )
        return true;
    if( address1.ParamSubName < address2.ParamSubName )
        return true;

    if( address1.ParamTargetType == ParameterAddress::CameraParam || address1.ParamTargetType == ParameterAddress::LightParam )
    {
        return address1.Index < address2.Index;
    }

    if( address1.NodeName < address2.NodeName )
        return true;

    if( address1.ParamTargetType == ParameterAddress::PluginParam || address1.ParamTargetType == ParameterAddress::ResourceParam )
    {
        if( address1.PluginName < address2.PluginName )
            return true;
    }

    return false;
}

}	// bv