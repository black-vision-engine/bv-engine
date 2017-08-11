#include "stdafx.h"

#include "GizmoLogicBase.h"

#include "Serialization/SerializationHelper.h"
#include "Engine/Models/Plugins/Parameters/AbstractModelParameter.h"

#include "UseLoggerLibBlackVision.h"


namespace bv {
namespace model {

// ***********************
//
GizmoLogicBase::GizmoLogicBase( model::BasicNodeWeakPtr gizmoRoot, model::BasicNodeWeakPtr gizmoOwner )
    : m_gizmoOwner( gizmoOwner )
    , m_gizmoRoot( gizmoRoot )
{
    m_paramValModel = std::make_shared< model::DefaultParamValModel >();
}

// ***********************
//
GizmoLogicBase::~GizmoLogicBase()
{}

// ***********************
//
model::IParameterPtr                            GizmoLogicBase::GetParameter        ( const std::string & name ) const
{
    return m_paramValModel->GetParameter( name );
}

// ***********************
//
const std::vector< model::IParameterPtr > &     GizmoLogicBase::GetParameters    () const
{
    return m_paramValModel->GetParameters();
}

// ***********************
//
void                                            GizmoLogicBase::Serialize       ( ISerializer & /*ser*/ ) const
{
    // Don't serialize gizmo. If You really want to have gizmo serialized, it isn't default implementation for sure.
}

// ***********************
//
void                                            GizmoLogicBase::Deserialize     ( const IDeserializer & /*deser */)
{
    // Don't deserialize gizmo. If You really want to have gizmo deserialized, it isn't default implementation for sure.
}

// ***********************
//
void	                                        GizmoLogicBase::Update			( TimeType )
{
    m_paramValModel->Update();
}

// ***********************
//
bool                                            GizmoLogicBase::ParameterChanged ( const std::string & name ) const
{
    auto state = m_paramValModel->GetState( name );
    assert( state );
    return state->StateChanged();

}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                                            GizmoLogicBase::HandleEvent     ( IDeserializer & /*eventDeser*/, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    return false;
}

}   // model
}	// bv
