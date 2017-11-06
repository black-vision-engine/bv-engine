#include "stdafx.h"
#include "ParamAddress.h"


#include "Memory/MemoryLeaks.h"



namespace bv {
namespace Convert
{

// ***********************
//
std::pair< ParameterAddress::TargetType, const char* > TargetTypeMapping[] = 
    { std::make_pair( ParameterAddress::TargetType::GlobalEffectParam, "GlobalEffectParam" )
    , std::make_pair( ParameterAddress::TargetType::PluginParam, "PluginParam" )
    , std::make_pair( ParameterAddress::TargetType::ResourceParam, "ResourceParam" )
    , std::make_pair( ParameterAddress::TargetType::LightParam, "LightParam" )
    , std::make_pair( ParameterAddress::TargetType::NodeLogicParam, "NodeLogicParam" )
    , std::make_pair( ParameterAddress::TargetType::CameraParam, "CameraParam" )
    , std::make_pair( ParameterAddress::TargetType::FailTarget, SerializationHelper::EMPTY_STRING )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( ParameterAddress::TargetType, TargetTypeMapping );

}   // Convert

// ***********************
//
void                    ParameterAddress::Serialize       ( ISerializer & ser ) const
{
    ser.SetAttribute( SerializationHelper::PARAM_TARGET_TYPE_STRING, Convert::T2String( ParamTargetType ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodeName );
    ser.SetAttribute( SerializationHelper::PLUGIN_NAME_STRING, PluginName );
    ser.SetAttribute( SerializationHelper::PARAM_NAME_STRING, ParamName );
    ser.SetAttribute( SerializationHelper::PARAM_SUB_NAME_STRING, ParamSubName );

    if( ParamTargetType == ParameterAddress::TargetType::LightParam || ParamTargetType ==  ParameterAddress::TargetType::CameraParam )
        ser.SetAttribute( SerializationHelper::PARAM_INDEX, Convert::T2String( Index ) );
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
    address.Index             = Convert::String2T< UInt32 >( deser.GetAttribute( SerializationHelper::PARAM_INDEX ), std::numeric_limits< UInt32 >::quiet_NaN() );
    address.ParamTargetType   = Convert::String2T<ParameterAddress::TargetType>( deser.GetAttribute( SerializationHelper::PARAM_TARGET_TYPE_STRING ), ParameterAddress::TargetType::FailTarget );

    return address;
}

// ***********************
// Thanks to this operator you can use ParameterAddress as map key.
bool                 operator<       ( const ParameterAddress& address1, const ParameterAddress& address2 )
{
    if( address1.ParamTargetType < address2.ParamTargetType )
        return true;
    else if( address1.ParamTargetType == address2.ParamTargetType )
    {
        if( address1.SceneName < address2.SceneName )
            return true;
        else if( address1.SceneName == address2.SceneName )
        {
            if( address1.ParamName < address2.ParamName )
                return true;
            if( address1.ParamName == address2.ParamName )
            {
                if( address1.ParamSubName < address2.ParamSubName )
                    return true;
                else if( address1.ParamSubName == address2.ParamSubName )
                {
                    if( address1.ParamTargetType == ParameterAddress::CameraParam || address1.ParamTargetType == ParameterAddress::LightParam )
                    {
                        if( address1.Index < address2.Index )   return true;
                        if( address1.Index > address2.Index )   return false;
                    }

                    if( address1.NodeName < address2.NodeName )
                        return true;
                    else if( address1.NodeName == address2.NodeName )
                    {
                        if( address1.ParamTargetType == ParameterAddress::PluginParam || address1.ParamTargetType == ParameterAddress::ResourceParam )
                        {
                            if( address1.PluginName < address2.PluginName )
                                return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

}	// bv