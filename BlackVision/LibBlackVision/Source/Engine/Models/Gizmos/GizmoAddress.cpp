#include "stdafx.h"
#include "GizmoAddress.h"





namespace bv {
namespace Convert
{

// ***********************
//
std::pair< model::GizmoType, const char* > GizmoTypeMapping[] =
{ std::make_pair( model::GizmoType::Effect, "Effect" )
, std::make_pair( model::GizmoType::Logic, "Logic" )
, std::make_pair( model::GizmoType::Node, "Node" )
, std::make_pair( model::GizmoType::Plugin, "plugin" )
, std::make_pair( model::GizmoType::Scene, "Scene" )
, std::make_pair( model::GizmoType::Total, SerializationHelper::EMPTY_STRING )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( model::GizmoType, GizmoTypeMapping );

}   // Convert


namespace SerializationHelper
{

const std::string GIZMO_TYPE_NAME       = "GizmoType";
const std::string ELEMENT_NAME          = "ElementName";
const std::string FUNCTIONALITY_NAME    = "FunctionalityName";

}   // Convert



    // ***********************
    //
void                    GizmoAddress::Serialize       ( ISerializer & ser ) const
{
    ser.SetAttribute( SerializationHelper::GIZMO_TYPE_NAME, Convert::T2String( GizmoType ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodeName );
    ser.SetAttribute( SerializationHelper::ELEMENT_NAME, ElementName );
    ser.SetAttribute( SerializationHelper::FUNCTIONALITY_NAME, FunctionalityName );
}

// ***********************
//
GizmoAddress            GizmoAddress::Create          ( const IDeserializer & deser )
{
    GizmoAddress address;

    address.SceneName = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
    address.ElementName = deser.GetAttribute( SerializationHelper::ELEMENT_NAME );
    address.NodeName = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
    address.FunctionalityName = deser.GetAttribute( SerializationHelper::FUNCTIONALITY_NAME );
    address.GizmoType = Convert::String2T< model::GizmoType >( deser.GetAttribute( SerializationHelper::GIZMO_TYPE_NAME ), model::GizmoType::Total );

    return address;
}



}	// bv


