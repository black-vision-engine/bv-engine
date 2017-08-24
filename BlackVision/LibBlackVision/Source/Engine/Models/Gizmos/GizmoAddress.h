#pragma once


#include "Engine/Models/Gizmos/GizmoLogicBase.h"

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/SerializationHelper.h"




namespace bv
{

// ***********************
//
struct GizmoAddress
{
    std::string                     SceneName;
    std::string                     NodeName;
    std::string                     ElementName;        // Plugin/Logic/Effect
    std::string                     FunctionalityName;
    model::GizmoType                GizmoType;


    // ***********************
    //
    void                        Serialize       ( ISerializer & ser ) const;
    static GizmoAddress         Create          ( const IDeserializer & deser );
};

DECLARE_ENUM_SERIALIZATION( model::GizmoType );


}	// bv

